#include "framebuffer.h"


namespace winner
{


void	framebuffer::fill_rect( size_t x, size_t y, size_t w, size_t h, int r, int g, int b, int a )
{
	size_t maxX = (x +w), maxY = (x +h);
	
	if( maxX >= mVariableScreenProperties.xres )
		maxX = mVariableScreenProperties.xres;
	if( maxY >= mVariableScreenProperties.yres )
		maxY = mVariableScreenProperties.yres;
	
	for( size_t currY = y; currY < maxY; currY++ )
	{
		for( size_t currX = x; currX < maxX; currX++ )
		{
			set_pixel( currX, currY, r, g, b, a );
		}
	}
	
	UpdateFrameBuffer( mVariableScreenProperties.xres, mVariableScreenProperties.yres, mVariableScreenProperties.bits_per_pixel, mFramebuffer );
}

void	framebuffer::stroke_rect( size_t x, size_t y, size_t w, size_t h, int r, int g, int b, int a, size_t lineWidth )
{
	size_t currY = y;
	
	size_t maxX = (x +w), maxY = (x +h);
	
	if( maxX >= mVariableScreenProperties.xres )
		maxX = mVariableScreenProperties.xres;
	if( maxY >= mVariableScreenProperties.yres )
		maxY = mVariableScreenProperties.yres;
	
	while( currY < (y +lineWidth) && currY < maxY )
	{
		for( size_t currX = x; currX < maxX; currX++ )
		{
			set_pixel( currX, currY, r, g, b, a );
		}
		currY++;
	}
	
	for( ; currY < (maxY -lineWidth); currY++ )
	{
		for( size_t currX = x; currX < (x+lineWidth); currX++ )
			set_pixel( currX, currY, r, g, b, a );
		for( size_t currX = x +w -lineWidth; currX < (x+w); currX++ )
			set_pixel( currX, currY, r, g, b, a );
	}
	
	while( currY < maxY )
	{
		for( size_t currX = x; currX < maxX; currX++ )
		{
			set_pixel( currX, currY, r, g, b, a );
		}
		currY++;
	}
	
	UpdateFrameBuffer( mVariableScreenProperties.xres, mVariableScreenProperties.yres, mVariableScreenProperties.bits_per_pixel, mFramebuffer );
}


void	framebuffer::fill_circle( size_t x, size_t y, size_t radius, int r, int g, int b, int a )
{
	size_t	radiusSquared = radius * radius;
	
	size_t	minX = ((x >= radius) ? (x -radius) : 0), maxX = (x +radius);
	if( maxX > mVariableScreenProperties.xres )
		maxX = mVariableScreenProperties.xres -1;
	size_t	minY = ((y >= radius) ? (y -radius) : 0), maxY = (y +radius);
	if( maxY > mVariableScreenProperties.yres )
		maxY = mVariableScreenProperties.yres -1;
	
	for( size_t currY = minY; currY <= maxY; currY++ )
	{
		for( size_t currX = minX; currX <= maxX; currX++ )
		{
			if( radiusSquared > ((currX-x)*(currX-x)+(currY-y)*(currY-y)) )
				set_pixel( currX, currY, r, g, b, a );
		}
	}
}


void	framebuffer::stroke_circle( size_t x, size_t y, size_t radius, int r, int g, int b, int a, size_t lineWidth )
{
	size_t	radiusSquared = radius * radius;
	size_t	innerRadiusSquared = (radius -lineWidth) * (radius -lineWidth);
	
	size_t	minX = ((x >= radius) ? (x -radius) : 0), maxX = (x +radius);
	if( maxX > mVariableScreenProperties.xres )
		maxX = mVariableScreenProperties.xres -1;
	size_t	minY = ((y >= radius) ? (y -radius) : 0), maxY = (y +radius);
	if( maxY > mVariableScreenProperties.yres )
		maxY = mVariableScreenProperties.yres -1;
	
	for( size_t currY = minY; currY <= maxY; currY++ )
	{
		for( size_t currX = minX; currX <= maxX; currX++ )
		{
			if( radiusSquared > ((currX-x)*(currX-x)+(currY-y)*(currY-y))
				&& innerRadiusSquared <= ((currX-x)*(currX-x)+(currY-y)*(currY-y)) )
				set_pixel( currX, currY, r, g, b, a );
		}
	}
}


} /* namespace winner */