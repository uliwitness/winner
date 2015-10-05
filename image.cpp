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

void	image::draw_image( coordinate_t x, coordinate_t y, const image& inImage )
{
	coordinate_t maxX = (x +inImage.width()), maxY = (y +inImage.height());
	
	if( maxX > mWidth )
		maxX = mWidth;
	if( maxY > mHeight )
		maxY = mHeight;
	coordinate_t	xclip = (x > 0)? x : 0, yclip = (y > 0)? y : 0;
	coordinate_t	imgX = xclip, imgY = yclip;
	
	for( coordinate_t currY = yclip; currY < maxY; currY++ )
	{
		for( coordinate_t currX = xclip; currX < maxX; currX++ )
		{
			color_component_t		r, g, b, a;
			inImage.get_pixel( imgX, imgY, &r, &g, &b, &a );
			if( a == 0xff )
				set_pixel( currX, currY, r, g, b, a );
			imgX++;
		}

		imgX = xclip;
		imgY++;
	}
}


void	image::clear_image( coordinate_t x, coordinate_t y, const image& inImage )
{
	coordinate_t maxX = (x +inImage.width()), maxY = (y +inImage.height());
	
	if( maxX > mWidth )
		maxX = mWidth;
	if( maxY > mHeight )
		maxY = mHeight;
	coordinate_t	xclip = (x > 0)? x : 0, yclip = (y > 0)? y : 0;
	coordinate_t	imgX = xclip, imgY = yclip;
	
	for( coordinate_t currY = yclip; currY < maxY; currY++ )
	{
		for( coordinate_t currX = xclip; currX < maxX; currX++ )
		{
			color_component_t		r, g, b, a;
			inImage.get_pixel( imgX, imgY, &r, &g, &b, &a );
			if( a == 0xff )
				set_pixel( currX, currY, 0x00, 0x00, 0x00, 0x00 );
			imgX++;
		}

		imgX = xclip;
		imgY++;
	}
}


void	image::fill_rect( coordinate_t x, coordinate_t y, coordinate_t w, coordinate_t h, color_component_t r, color_component_t g, color_component_t b, color_component_t a )
{
	coordinate_t	maxX = (x +w), maxY = (y +h);
	coordinate_t	xclip = (x > 0)? x : 0, yclip = (y > 0)? y : 0;
	
	if( maxX > mWidth )
		maxX = mWidth;
	if( maxY > mHeight )
		maxY = mHeight;
	
	for( coordinate_t currY = yclip; currY < maxY; currY++ )
	{
		for( coordinate_t currX = xclip; currX < maxX; currX++ )
		{
			set_pixel( currX, currY, r, g, b, a );
		}
	}
}

void	image::stroke_rect( coordinate_t x, coordinate_t y, coordinate_t w, coordinate_t h, color_component_t r, color_component_t g, color_component_t b, color_component_t a, coordinate_t lineWidth )
{
	coordinate_t	maxX = (x +w), maxY = (y +h);
	coordinate_t	xclip = (x > 0)? x : 0, yclip = (y > 0)? y : 0;
	coordinate_t	currY = yclip;
	
	if( maxX >= mWidth )
		maxX = mWidth;
	if( maxY >= mHeight )
		maxY = mHeight;
	
	while( currY < (y +lineWidth) && currY < maxY )
	{
		for( coordinate_t currX = xclip; currX < maxX; currX++ )
		{
			set_pixel( currX, currY, r, g, b, a );
		}
		currY++;
	}
	
	for( ; currY < (maxY -lineWidth); currY++ )
	{
		for( coordinate_t currX = xclip; currX < (x+lineWidth); currX++ )
			set_pixel( currX, currY, r, g, b, a );
		for( coordinate_t currX = x +w -lineWidth; currX < (x+w); currX++ )
			set_pixel( currX, currY, r, g, b, a );
	}
	
	while( currY < maxY )
	{
		for( coordinate_t currX = xclip; currX < maxX; currX++ )
		{
			set_pixel( currX, currY, r, g, b, a );
		}
		currY++;
	}
}


void	image::fill_circle( coordinate_t x, coordinate_t y, coordinate_t radius, color_component_t r, color_component_t g, color_component_t b, color_component_t a )
{
	coordinate_t	radiusSquared = radius * radius;
	
	coordinate_t	minX = ((x >= radius) ? (x -radius) : 0), maxX = (x +radius);
	if( maxX > mWidth )
		maxX = mWidth -1;
	coordinate_t	minY = ((y >= radius) ? (y -radius) : 0), maxY = (y +radius);
	if( maxY > mHeight )
		maxY = mHeight -1;
	
	for( coordinate_t currY = minY; currY <= maxY; currY++ )
	{
		for( coordinate_t currX = minX; currX <= maxX; currX++ )
		{
			if( radiusSquared > ((currX-x)*(currX-x)+(currY-y)*(currY-y)) )
				set_pixel( currX, currY, r, g, b, a );
		}
	}
}


void	image::stroke_circle( coordinate_t x, coordinate_t y, coordinate_t radius, color_component_t r, color_component_t g, color_component_t b, color_component_t a, coordinate_t lineWidth )
{
	coordinate_t	radiusSquared = radius * radius;
	coordinate_t	innerRadiusSquared = (radius -lineWidth) * (radius -lineWidth);
	
	coordinate_t	minX = ((x >= radius) ? (x -radius) : 0), maxX = (x +radius);
	if( maxX > mWidth )
		maxX = mWidth -1;
	coordinate_t	minY = ((y >= radius) ? (y -radius) : 0), maxY = (y +radius);
	if( maxY > mHeight )
		maxY = mHeight -1;
	
	for( coordinate_t currY = minY; currY <= maxY; currY++ )
	{
		for( coordinate_t currX = minX; currX <= maxX; currX++ )
		{
			if( radiusSquared > ((currX-x)*(currX-x)+(currY-y)*(currY-y))
				&& innerRadiusSquared <= ((currX-x)*(currX-x)+(currY-y)*(currY-y)) )
				set_pixel( currX, currY, r, g, b, a );
		}
	}
}


void	image::stroke_line( coordinate_t startX, coordinate_t startY, coordinate_t endX, coordinate_t endY, color_component_t r, color_component_t g, color_component_t b, color_component_t a )
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


void	image::stroke_line( coordinate_t startX, coordinate_t startY, coordinate_t endX, coordinate_t endY, color_component_t r, color_component_t g, color_component_t b, color_component_t a, coordinate_t lineWidth )
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