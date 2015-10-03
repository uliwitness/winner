//
//  image.cpp
//  winner
//
//  Created by Uli Kusterer on 03/10/15.
//  Copyright © 2015 Uli Kusterer. All rights reserved.
//

#include "image.hpp"


namespace winner
{

void	image::draw_image( size_t x, size_t y, const image& inImage )
{
	assert(inImage.bits_per_pixel() == 32);	// Need to implement a get_pixel() that returns the components in a format-agnostic way.
	assert(inImage.row_bytes() == (inImage.width() * 4));

	size_t maxX = (x +inImage.width()), maxY = (y +inImage.height());
	
	if( maxX >= mWidth )
		maxX = mWidth;
	if( maxY >= mHeight )
		maxY = mHeight;
	
	uint8_t*	currPx = inImage.pixel_data();
	
	for( size_t currY = y; currY < maxY; currY++ )
	{
		for( size_t currX = x; currX < maxX; currX++ )
		{
			if( currPx[3] == 0xff )
				set_pixel( currX, currY, currPx[0], currPx[1], currPx[2], currPx[3] );
			currPx += 4;
		}
	}
}


void	image::fill_rect( size_t x, size_t y, size_t w, size_t h, int r, int g, int b, int a )
{
	size_t maxX = (x +w), maxY = (y +h);
	
	if( maxX > mWidth )
		maxX = mWidth;
	if( maxY > mHeight )
		maxY = mHeight;
	
	for( size_t currY = y; currY < maxY; currY++ )
	{
		for( size_t currX = x; currX < maxX; currX++ )
		{
			set_pixel( currX, currY, r, g, b, a );
		}
	}
}

void	image::stroke_rect( size_t x, size_t y, size_t w, size_t h, int r, int g, int b, int a, size_t lineWidth )
{
	size_t currY = y;
	
	size_t maxX = (x +w), maxY = (y +h);
	
	if( maxX >= mWidth )
		maxX = mWidth;
	if( maxY >= mHeight )
		maxY = mHeight;
	
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
}


void	image::fill_circle( size_t x, size_t y, size_t radius, int r, int g, int b, int a )
{
	size_t	radiusSquared = radius * radius;
	
	size_t	minX = ((x >= radius) ? (x -radius) : 0), maxX = (x +radius);
	if( maxX > mWidth )
		maxX = mWidth -1;
	size_t	minY = ((y >= radius) ? (y -radius) : 0), maxY = (y +radius);
	if( maxY > mHeight )
		maxY = mHeight -1;
	
	for( size_t currY = minY; currY <= maxY; currY++ )
	{
		for( size_t currX = minX; currX <= maxX; currX++ )
		{
			if( radiusSquared > ((currX-x)*(currX-x)+(currY-y)*(currY-y)) )
				set_pixel( currX, currY, r, g, b, a );
		}
	}
}


void	image::stroke_circle( size_t x, size_t y, size_t radius, int r, int g, int b, int a, size_t lineWidth )
{
	size_t	radiusSquared = radius * radius;
	size_t	innerRadiusSquared = (radius -lineWidth) * (radius -lineWidth);
	
	size_t	minX = ((x >= radius) ? (x -radius) : 0), maxX = (x +radius);
	if( maxX > mWidth )
		maxX = mWidth -1;
	size_t	minY = ((y >= radius) ? (y -radius) : 0), maxY = (y +radius);
	if( maxY > mHeight )
		maxY = mHeight -1;
	
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


void	image::stroke_line( size_t startX, size_t startY, size_t endX, size_t endY, int r, int g, int b, int a )
{
	float		x, y,
				dx, dy,
				xDirection, yDirection,
				errorTerm = 0,
				i;
	
	// x distance:
	dx = endX -startX;
	if( dx < 0 )
		dx = -dx;
	
	// y distance:
	dy = endY -startY;
	if( dy < 0 )
		dy = -dy;
	
	// Horizontal line direction:
	if( endX < startX )
		xDirection = -1;
	else if( endX -startX > 0 )
		xDirection = 1;
	else
		xDirection = 0;
	
	// Vertical line direction:
	if( endY < startY )
		yDirection = -1;
	else if( endY -startY > 0 )
		yDirection = 1;
	else
		yDirection = 0;
	
	// Calculate first pixel:
	x = startX;
	y = startY;
	
	if( dx > dy )
	{
		// More horizontal than vertical:
		for( i = 0; i <= dx; i++ )
		{
			set_pixel( x, y, r, g, b, a );
			
			// Move horizontally:
			x += xDirection;
			
			// Check to see whether we need to move vertically:
			errorTerm += dy;
			
			if( errorTerm > dx )
			{
				errorTerm -= dx;	// Reset error term.
				y += yDirection; 	// Move one pixel up. 
			}
		}
	}
	else
	{
		// More vertical than horizontal:
		for( i = 0; i <= dy; i++ )
		{
			set_pixel( x, y, r, g, b, a );
			
			// Move vertically:
			y += yDirection;
			
			// Need to move horizontally?
			errorTerm += dx;
			
			if( errorTerm > dy )
			{
				errorTerm -= dy;	// Reset error term.
				x += xDirection; 	// Move one pixel right. 
			}
		}
	}
}


void	image::stroke_line( size_t startX, size_t startY, size_t endX, size_t endY, int r, int g, int b, int a, size_t lineWidth )
{
	if( startY == endY || startX == endX )	// horizontal or vertical
	{
		fill_rect( startX, startY, endX -startX +lineWidth, endY -startY +lineWidth, r, g, b, a );
	}
	else
	{
		float		x, y,
					dx, dy,
					xDirection, yDirection,
					errorTerm = 0,
					i;
		
		// x distance:
		dx = endX -startX;
		if( dx < 0 )
			dx = -dx;
		
		// y distance:
		dy = endY -startY;
		if( dy < 0 )
			dy = -dy;
		
		// Horizontal line direction:
		if( endX < startX )
			xDirection = -1;
		else if( endX -startX > 0 )
			xDirection = 1;
		else
			xDirection = 0;
		
		// Vertical line direction:
		if( endY < startY )
			yDirection = -1;
		else if( endY -startY > 0 )
			yDirection = 1;
		else
			yDirection = 0;
		
		// Calculate first pixel:
		x = startX;
		y = startY;
		
		if( dx > dy )
		{
			// More horizontal than vertical:
			for( i = 0; i <= dx; i++ )
			{
				fill_rect( x, y, 1, lineWidth, r, g, b, a );
				
				// Move horizontally:
				x += xDirection;
				
				// Check to see whether we need to move vertically:
				errorTerm += dy;
				
				if( errorTerm > dx )
				{
					errorTerm -= dx;	// Reset error term.
					y += yDirection; 	// Move one pixel up. 
				}
			}
		}
		else
		{
			// More vertical than horizontal:
			for( i = 0; i <= dy; i++ )
			{
				fill_rect( x, y, lineWidth, 1, r, g, b, a );
				
				// Move vertically:
				y += yDirection;
				
				// Need to move horizontally?
				errorTerm += dx;
				
				if( errorTerm > dy )
				{
					errorTerm -= dy;	// Reset error term.
					x += xDirection; 	// Move one pixel right. 
				}
			}
		}
	}
}

} /* namespace winner */