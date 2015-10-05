//
//  window.cpp
//  winner
//
//  Created by Uli Kusterer on 04/10/15.
//  Copyright © 2015 Uli Kusterer. All rights reserved.
//

#include "window.hpp"


namespace winner
{

using namespace std;

void	window_list::rebuild_sys_mask_for_all()
{
	for( auto currWindow = mWindows.begin(); currWindow != mWindows.end(); currWindow++ )
		rebuild_sys_mask_for_window( *currWindow );
}


void	window_list::move_window( winner::window* inWindow, coordinate_t x, coordinate_t y, vector<window*>	*outAffectedWindows )
{
	coordinate_t	oldX = inWindow->image().x_offset(),
					oldY = inWindow->image().y_offset(),
					oldR = inWindow->image().x_offset() +inWindow->image().width() ,
					oldB = inWindow->image().y_offset() +inWindow->image().height(),
					r = x + inWindow->image().width(),
					b = y + inWindow->image().height();
	
	for( auto currWindow = mWindows.rbegin(); currWindow != mWindows.rend(); currWindow++ )
	{
		coordinate_t	currX = (**currWindow).image().x_offset(),
						currY = (**currWindow).image().y_offset(),
						currR = (**currWindow).image().x_offset() +(**currWindow).image().width(),
						currB = (**currWindow).image().y_offset() +(**currWindow).image().height();
		
		if( (currX <= oldR && currR >= oldX && currY <= oldB && currB >= oldY)	// Overlaps old position?
			|| (currX <= r && currR >= x && currY <= b && currB >= y) )			// Overlaps new position?
		{
			outAffectedWindows->push_back( *currWindow );	// Mark for redraw!
		}
		
		if( *currWindow == inWindow )
			break;
	}
	
	inWindow->image().set_x_offset( x );
	inWindow->image().set_y_offset( y );

	for( auto currWindow = outAffectedWindows->begin(); currWindow != outAffectedWindows->end(); currWindow++ )
	{
		rebuild_sys_mask_for_window( *currWindow );
	}
}


void	window_list::rebuild_sys_mask_for_window( window* inWindow )
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

} /* namespace winner */