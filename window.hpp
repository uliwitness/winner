//
//  window.hpp
//  winner
//
//  Created by Uli Kusterer on 04/10/15.
//  Copyright © 2015 Uli Kusterer. All rights reserved.
//

#pragma once


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
	
	window( size_t x, size_t y, size_t w, size_t h, winner::image& inFramebufferImage ) : mFramebufferImage(inFramebufferImage)
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
	
	window*		create_window( size_t x, size_t y, size_t w, size_t h )
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
		inWindow->image().sys_mask().clear( 0xff );	// Window's whole surface can be drawn on.
		for( auto currWindow = mWindows.begin(); currWindow != mWindows.end(); currWindow++ )
		{
			if( *currWindow == inWindow )
				break;
			
			size_t l, t, r, b;
			
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
			
			// Remove that chunk from our mask.
			inWindow->image().sys_mask().fill_rect( l, t, r -l, b -t, 0x00, 0x00, 0x00, 0x00 );
		}
	}
	
protected:
	std::vector<window*>	mWindows;
	winner::image&			mFramebufferImage;	// The actual screen framebuffer's image.
};

} /* namespace winner */
