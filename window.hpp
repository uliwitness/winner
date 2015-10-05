//
//  window.hpp
//  winner
//
//  Created by Uli Kusterer on 04/10/15.
//  Copyright © 2015 Uli Kusterer. All rights reserved.
//

#pragma once

/*
	The window class describes a sub-area of the screen that may be drawn to. It is affected by
	both of the image's masks:
	
	The regular mask is used to designate the shape of the window. E.g. if you wanted a circular window,
	you'd mask off everything outside the circle. If we had MacOS-Classic-style single memory space,
	we'd want three masks, one for cutting off windows above, one for the window shape, and one for
	drawing. But since this is for a window server running as a separate process, we can get away with
	two because the drawing of window contents happens in another process, and all we get is a bitmap.
	
	The system mask of the window is used to restrict drawing even further, by removing the area
	occupied by all other windows above ours from the regular mask. That way, you can draw even into
	an obscured window without having to redraw any of the other windows. This happens in
	rebuild_sys_mask_for_window().
	
	If we wanted to support alpha blending (i.e. more than 100% opaque and 100% transparent) we would
	have to overdraw those areas. I.e. every time they are changed, windows below us would need to be
	redrawn, too.
*/


#include "image.hpp"
#include <vector>


namespace winner
{

using namespace std;

class window
{
public:
	image&	image() { return mImage; }
	
protected:
	friend class window_list;
	
	window( coordinate_t x, coordinate_t y, coordinate_t w, coordinate_t h, winner::image& inFramebufferImage ) : mFramebufferImage(inFramebufferImage)
	{
		mImage.init( w, h, mFramebufferImage.pixel_data(), false, mFramebufferImage.bits_per_pixel(), mFramebufferImage.row_bytes(), x, y );
	}
	
	winner::image		mImage;				// A sub-image covering a smaller section of mFramebufferImage.
	winner::image&		mFramebufferImage;	// The actual screen framebuffer's image.
};


class window_list
{
public:
	window_list( winner::image& inFramebufferImage ) : mFramebufferImage(inFramebufferImage) {}
	~window_list()
	{
		for( auto currWindow = mWindows.begin(); currWindow != mWindows.end(); currWindow++ )
			delete *currWindow;
	}
	
	window*		create_window( coordinate_t x, coordinate_t y, coordinate_t w, coordinate_t h )
	{
		window*	theWindow = new window( x, y, w, h, mFramebufferImage );
		mWindows.insert( mWindows.begin(), theWindow );
		
		return theWindow;
	}
	
	void	delete_window( window* inWindow )
	{
		for( auto currWindow = mWindows.begin(); currWindow != mWindows.end(); currWindow++ )
		{
			if( *currWindow == inWindow )
			{
				delete inWindow;
				mWindows.erase( currWindow );
				break;
			}
		}
	}
	
	void	rebuild_sys_mask_for_window( window* inWindow )
	{
		bool		didMakeMask = false;
		for( auto currWindow = mWindows.begin(); currWindow != mWindows.end(); currWindow++ )
		{
			if( *currWindow == inWindow )
				break;
			
			coordinate_t l, t, r, b;
			
			// Calculate rectangle where currWindow overlaps with inWindow:
			if( (**currWindow).image().x_offset() < inWindow->image().x_offset() )
				l = inWindow->image().x_offset();
			else
				l = (**currWindow).image().x_offset();
			
			if( (**currWindow).image().y_offset() < inWindow->image().y_offset() )
				t = inWindow->image().y_offset();
			else
				t = (**currWindow).image().y_offset();
			
			if( ((**currWindow).image().x_offset() +(**currWindow).image().width()) > (inWindow->image().x_offset() +inWindow->image().width()) )
				r = (inWindow->image().x_offset() +inWindow->image().width());
			else
				r = ((**currWindow).image().x_offset() +(**currWindow).image().width());
			
			if( ((**currWindow).image().y_offset() +(**currWindow).image().height()) > (inWindow->image().y_offset() +inWindow->image().height()) )
				b = (inWindow->image().y_offset() +inWindow->image().height());
			else
				b = ((**currWindow).image().y_offset() +(**currWindow).image().height());
			
			if( l >= r || t >= r )	// Completely outside.
				continue;
			
			// Convert to inWindow.image's coordinate system:
			l -= inWindow->image().x_offset();
			t -= inWindow->image().y_offset();
			r -= inWindow->image().x_offset();
			b -= inWindow->image().y_offset();
			
			// Lazily create our mask if needed (unobscured windows can then skip the memory
			//	overhead and the slowdown of even checking the mask)
			if( !didMakeMask )
			{
				inWindow->image().sys_mask().clear( 0xff );	// Window's whole surface can be drawn on.
				didMakeMask = true;
			}
			
			// Remove that chunk from our mask.
			if( (**currWindow).image().has_mask() )
			{
				inWindow->image().sys_mask().clear_image( (**currWindow).image().x_offset() -inWindow->image().x_offset(), (**currWindow).image().y_offset() -inWindow->image().y_offset(), (**currWindow).image().mask() );
			}
			else
			{
				inWindow->image().sys_mask().fill_rect( l, t, r - l, b - t, 0x00, 0x00, 0x00, 0x00 );
			}
		}
	}
	
protected:
	std::vector<window*>	mWindows;
	winner::image&			mFramebufferImage;	// The actual screen framebuffer's image.
};

} /* namespace winner */
