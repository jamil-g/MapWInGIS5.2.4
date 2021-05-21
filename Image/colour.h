#ifndef COLORS_H
#define COLORS_H

#ifdef RELEASE_MODE
	# pragma pack(push,1)
#endif

class colour
{
public:
	colour();
	colour(long oleColor);
	colour(unsigned char r, unsigned char g, unsigned char b);

	unsigned char	blue;
	unsigned char	green;
	unsigned char	red;	
	unsigned char   alpha;

	bool operator==( const colour & c );

	OLE_COLOR ToOleColorNoAlpha();
	OLE_COLOR ToOleColor();
	void FromOleColor(long color);
	void FromOleColorNoAlpha(long color);
};

#ifdef RELEASE_MODE
	# pragma pack(pop)
#endif

#define C_MAROON		colour(127,0,0);
#define C_RED			colour(255,0,0);
#define	C_FOREST		colour(0,127,0);
#define C_BROWN		colour(127,127,0);
#define C_ORANGE		colour(255,127,0);
#define C_GREEN		colour(0,255,0);
#define C_LTGREEN		colour(127,255,0);
#define C_YELLOW		colour(255,255,);
#define C_NAVY		colour(0,0,100);
#define C_PURPLE		colour(127,0,100);
#define C_GRAY		colour(127,127,127);
#define C_PINK		colour(255,127,127);
#define C_TURQUOISE	colour(0,255,127);
#define C_LIME		colour(127,255,127);
#define C_BLUE		colour(0,0,255);
#define C_LTBLUE		colour(127,127,255);
#define C_VIOLET		colour(255,0,255);

#endif 