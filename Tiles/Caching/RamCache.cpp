/**************************************************************************************
 * Project: MapWindow Open Source (MapWinGis ActiveX control) 
 **************************************************************************************
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * (the "License"); you may not use this file except in compliance with 
 * the License. You may obtain a copy of the License at http://www.mozilla.org/mpl/ 
 * See the License for the specific language governing rights and limitations
 * under the License.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ************************************************************************************** 
 * Contributor(s): 
 * (Open source contributors should list themselves and their modifications here). */
// Paul Meems August 2018: Modernized the code as suggested by CLang and ReSharper

#include "StdAfx.h"
#include "RamCache.h"

// *********************************************************
//	     Close()
// *********************************************************
void RamCache::Close()
{
    // release tiles first
    Clear();

    // then release dynamically allocated containers
    for (TilesCache::iterator it = _tilesMap.begin(); it != _tilesMap.end(); ++it)
    {
        vector<TilePoints*>* list = it->second;
        if (list)
        {
            for (size_t i = 0; i < (int)list->size(); i++)
            {
                TilePoints* points = (*list)[i];
                if (points)
                {
                    TilePoints::iterator it2 = points->begin();
                    while (it2 != points->end())
                    {
                        TilePositions* tiles = it2->second;
                        if (tiles)
                        {
                            delete tiles;
                        }
                        ++it2;
                    }
                    delete points;
                }
            }
            delete list;
        }
    }
}

// *********************************************************
//	     AddTile()
// *********************************************************
void RamCache::AddTile(TileCore* tile)
{
    if (tile->zoom() < 0 || tile->zoom() >= 25)
    {
        return;
    }

    _section.Lock();

    // seeking provider
    std::vector<TilePoints*>* list;
    if (_tilesMap.find(tile->get_ProviderId()) != _tilesMap.end())
    {
        list = _tilesMap[tile->get_ProviderId()];
    }
    else
    {
        list = new std::vector<TilePoints*>();
        list->resize(25, nullptr);
        _tilesMap[tile->get_ProviderId()] = list;
    }

    // seeking scale
    TilePoints* points = (*list)[tile->zoom()];
    if (!points)
    {
        points = new TilePoints();
        (*list)[tile->zoom()] = points;
    }

    // seeking X
    std::map<int, TileCore*>* map;
    if (points->find(tile->tileX()) != points->end())
    {
        map = (*points)[tile->tileX()];
    }
    else
    {
        map = new std::map<int, TileCore*>();
        (*points)[tile->tileX()] = map;
    }

    // seeking Y
    if (map->find(tile->tileY()) == map->end())
    {
        tile->AddRef();
        (*map)[tile->tileY()] = tile;
        _size += tile->get_ByteSize();
        _list.push_back(tile);
    }
    _section.Unlock();

    // automatically clear the cache if it exceeds the maximum size
    if ((double)_size / (double)(0x1 << 20) > _maxSize)
    {
        ClearOldest(TILE_CACHE_SIZE_TO_CLEAR);
    }
}

// *********************************************************
//	     ClearOldest()
// *********************************************************
void RamCache::ClearOldest(int sizeToClearBytes)
{
    int size = 0;

    for (size_t i = 0; i < _list.size(); i++)
    {
        if (!_list[i]->inBuffer())
        {
            _list[i]->toDelete(true);

            size += _list[i]->get_ByteSize();
            if (size > TILE_CACHE_SIZE_TO_CLEAR)
                break;
        }
    }

    DeleteMarkedTiles();
}

// **********************************************************
//		ClearByProvider()
// **********************************************************
// Removes tiles of the specified provider
void RamCache::Clear(int provider, int fromScale, int toScale)
{
    _section.Lock();
    for (TilesCache::iterator it = _tilesMap.begin(); it != _tilesMap.end(); ++it)
    {
        // if provider is equal -1, then all tiles are to be removed
        if (provider != (int)tkTileProvider::ProviderNone && provider != it->first)
            continue;

        vector<TilePoints*>* list = it->second;
        if (list)
        {
            for (int i = 0; i < (int)list->size(); i++)
            {
                if (i < fromScale || i > toScale)
                    continue;

                TilePoints* points = (*list)[i];
                if (points)
                {
                    TilePoints::iterator it2 = points->begin();
                    while (it2 != points->end())
                    {
                        TilePositions* tiles = it2->second;
                        TilePositions::iterator it3 = tiles->begin();

                        while (it3 != tiles->end())
                        {
                            TileCore* tile = it3->second;

                            if (tile->inBuffer())
                            {
                                ++it3; // it doesn't make sense to delete tiles which are 
                                // currently displayed; as they will be requested again on simple refresh of the map
                            }
                            else
                            {
                                tiles->erase(it3++);
                                _size -= tile->get_ByteSize();

                                // marks it to be removed from chronological vector
                                tile->toDelete(true);
                            }
                        }

                        // if all the Y's for given X are removed - delete the container
                        if (tiles->empty())
                        {
                            points->erase(it2++);
                            delete tiles;
                        }
                        else
                            ++it2;
                    }

                    // if all the X's for a given scale are removed - delete the container,
                    // but don't delete scale entry - 25 scales must preserved as they are accessed by index
                    if (points->empty())
                    {
                        delete points;
                        (*list)[i] = nullptr;
                    }
                }
            }
        }
    }

    _tilesMap.clear();

    // now remove tiles from chronological vector
    DeleteMarkedTiles();

    _section.Unlock();
}

// *********************************************************
//		DeleteMarkedTiles()
// *********************************************************
// Removes tiles marked as "to delete" from chronological vector.
// The tiles should removed from fast access storage before this operation.
// It's done by either calling ClearAll(), ClearByProvider() or ClearOldest.
// It's responsibility of caller to lock the the cache to make it thread safe
void RamCache::DeleteMarkedTiles()
{
    vector<TileCore*>::iterator it = _list.begin();
    while (it != _list.end())
    {
        TileCore* tile = *it;
        if (tile->toDelete())
        {
            tile->Release();
            it = _list.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// ********************************************************
//		get_Tile()
// ********************************************************
// Extracts a single tile from the cache
TileCore* RamCache::get_Tile(BaseProvider* provider, int scale, int tileX, int tileY)
{
    if (!provider) return nullptr;

    TileCore* tile = get_TileCore(provider->Id, scale, tileX, tileY);

    if (tile)
    {
        // original instance of projection might be closed if WMS layer was reopened;
        // so get_Projection() pointer is invalid, assign a new one
        tile->set_Projection(provider->get_Projection());
    }

    return tile;
}

// ********************************************************
//		OnProviderClosed()
// ********************************************************
void RamCache::OnProviderClosed(int providerId)
{
    // set projection for all tiles of this provider to NULL
    if (_tilesMap.find(providerId) != _tilesMap.end())
    {
        std::vector<TilePoints*>* scales = _tilesMap[providerId];
        for (size_t i = 0; i < scales->size(); i++)
        {
            TilePoints* points = (*scales)[i];
            if (points)
            {
                TilePoints::iterator it = points->begin();
                while (it != points->end())
                {
                    TilePositions* positions = it->second;
                    if (positions)
                    {
                        TilePositions::iterator it2 = positions->begin();
                        while (it2 != positions->end())
                        {
                            TileCore* tile = it2->second;
                            if (tile)
                            {
                                tile->set_Projection(nullptr);
                            }
                            ++it2;
                        }
                    }
                    ++it;
                }
            }
        }
    }
}

// ********************************************************
//		get_TileCore()
// ********************************************************
TileCore* RamCache::get_TileCore(int providerId, int scale, int tileX, int tileY)
{
    _section.Lock();

    TileCore* tile = nullptr;

    if (_tilesMap.find(providerId) != _tilesMap.end())
    {
        std::vector<TilePoints*>* scales = _tilesMap[providerId];
        if (scale >= 0 && scale < (int)scales->size())
        {
            TilePoints* points = (*scales)[scale];
            if (points != nullptr)
            {
                if (points->find(tileX) != points->end())
                {
                    std::map<int, TileCore*>* map = (*points)[tileX];
                    if (map->find(tileY) != map->end())
                    {
                        tile = (*map)[tileY];
                    }
                }
            }
        }
    }

    _section.Unlock();
    return tile;
}

// ***********************************************************
//		get_Size()
// ***********************************************************
// Use -1 for provider and scale to retrieve all the values
double RamCache::get_SizeMB(int provider, int scale)
{
    _section.Lock();
    int sum = 0;
    for (TilesCache::iterator it = _tilesMap.begin(); it != _tilesMap.end(); ++it)
    {
        if (provider != (int)tkTileProvider::ProviderNone && provider != it->first)
            continue;

        std::vector<TilePoints*>* scales = it->second;
        for (size_t i = 0; i < scales->size(); i++)
        {
            if (scale != -1 && i != scale)
                continue;

            TilePoints* points = (*scales)[i];
            if (points)
            {
                for (TilePoints::iterator it2 = points->begin(); it2 != points->end(); ++it2)
                {
                    TilePositions* tiles = it2->second;
                    if (tiles)
                    {
                        for (TilePositions::iterator it3 = tiles->begin(); it3 != tiles->end(); ++it3)
                        {
                            TileCore* tile = it3->second;
                            sum += tile->get_ByteSize();
                        }
                    }
                }
            }
        }
    }
    _section.Unlock();
    return (double)sum / (double)(0x1 << 20);
}

// ***********************************************************
//		get_Size()
// ***********************************************************
bool RamCache::get_TileExists(BaseProvider* provider, int scale, int x, int y)
{
    return get_Tile(provider, scale, x, y) != nullptr;
}

// ***********************************************************
//		get_TileCount()
// ***********************************************************
long RamCache::get_TileCount(int providerId, int zoom, CRect indices)
{
    long count = 0;

    for (int x = indices.left; x <= indices.right; x++)
    {
        for (int y = indices.bottom; y <= indices.top; y++)
        {
            TileCore* tile = get_TileCore(providerId, zoom, x, y);
            if (tile)
            {
                count++;
            }
        }
    }

    return count;
}
