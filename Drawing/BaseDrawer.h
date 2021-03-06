#pragma once

class CBaseDrawer
{
protected:	
	Extent* _extents; 
	double _pixelPerProjectionX;
	double _pixelPerProjectionY;
	CDC* _dc;
	Gdiplus::Graphics* _graphics;

public:
	CBaseDrawer(void) {
		_extents = NULL;
		_pixelPerProjectionX = 0.0;
		_pixelPerProjectionY = 0.0;
		_dc = NULL;
		_graphics = NULL;
	};
	virtual ~CBaseDrawer(void) {

	};

	void PixelToProjection( double piX, double piY, double& prX, double& prY )
	{
		prX = _extents->left + piX* 1.0/_pixelPerProjectionX;
		prY = _extents->top - piY*1.0/_pixelPerProjectionY;
	}

	void ProjectionToPixel( double prX, double prY, double& piX, double& piY )
	{
		piX = (prX - _extents->left)* _pixelPerProjectionX;
		piY = (_extents->top - prY) * _pixelPerProjectionY;
	}

	Gdiplus::ColorMatrix GetColorMatrix(byte alpha)
	{
		Gdiplus::ColorMatrix matrix;

		ZeroMemory((void*)&matrix.m, 25 * sizeof(Gdiplus::REAL));
		for (int i = 0; i < 5; i++) matrix.m[i][i] = 1.0f;

		matrix.m[3][3] = alpha / 255.0f;

		return matrix;
	}
};
