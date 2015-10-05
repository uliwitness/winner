#include <iostream>
#include <ios>
#include "framebuffer.hpp"
#include "window.hpp"

using namespace std;
using namespace winner;


void	DrawDesktopWindow( window* desktopWindow )
{
	desktopWindow->image().fill_rect(0, 0, desktopWindow->image().width(), desktopWindow->image().height(), 0xff, 0xff, 0xff, 0xff );
}


void	DrawAWindow( window* aWindow )
{
	aWindow->image().fill_rect(0, 0, aWindow->image().width(), aWindow->image().height(), 0xff, 0x00, 0x00, 0xff );
}


void	DrawBWindow( window* bWindow )
{
	bWindow->image().fill_rect(0, 0, bWindow->image().width(), bWindow->image().height(), 0x00, 0x00, 0xff, 0xff );
	bWindow->image().fill_circle( 170, 170, 50, 0x00, 0xff, 0x00, 0xff );
}


int main()
{
    framebuffer myfb;
    if( !myfb.valid() )
	{
		return 1;
	}
	
	window_list		windowList( myfb.image() );

	window*	desktopWindow = windowList.create_window( 0, 0, 640, 480 );
	window*	aWindow = windowList.create_window( 100, 100, 320, 200 );
	window*	bWindow = windowList.create_window( 116, 116, 320, 200 );
	
	windowList.rebuild_sys_mask_for_all();
	
	DrawDesktopWindow( desktopWindow );
	DrawBWindow( bWindow );
	DrawAWindow( aWindow );
	
	myfb.flush();
	
	sleep(3);
	
	vector<window*>	affectedWindows;
	windowList.move_window( bWindow, 200, 116, &affectedWindows );
	
	for( window* currWindow : affectedWindows )
	{
		if( currWindow == desktopWindow )
			DrawDesktopWindow( desktopWindow );
		if( currWindow == aWindow )
			DrawAWindow( aWindow );
		if( currWindow == bWindow )
			DrawBWindow( bWindow );
	}
	
	myfb.flush();
	
	sleep(3);
	
//	myfb.image().mask().clear( 0xff );
//	myfb.image().mask().fill_circle( 1, 240, 240, 0, 0, 0, 0 );
//	
//	myfb.image().fill_rect( 100, 100, 200, 50, 0x00, 0xff, 0x00, 0xff );
//	
//	myfb.image().remove_mask();
//	
//	myfb.image().stroke_rect( 100, 100, 200, 50, 0xff, 0x00, 0x00, 0xff, 1 );
//	myfb.image().stroke_rect( 0, 0, myfb.image().width(), myfb.image().height(), 0x00, 0x00, 0xff, 0xff, 2 );
//	myfb.image().fill_circle( 170, 170, 50, 0xff, 0x00, 0x00, 0xff );
//	myfb.image().stroke_circle( 170, 170, 50, 0xff, 0x00, 0xff, 0xff, 1 );
//	myfb.image().stroke_circle( 30, 30, 40, 0xff, 0xee, 0xee, 0xff, 4 );
//	
//	myfb.image().stroke_line( 40, 80, 600, 400, 0x00, 0xff, 0xee, 0xff, 3 );
//	myfb.image().stroke_line( 40, 80, 600, 80, 0x00, 0x00, 0x00, 0xff, 4 );
//	myfb.image().stroke_line( 40, 80, 40, 400, 0x00, 0x00, 0x00, 0xff, 4 );
//	
//	uint8_t		letterA[] =
//						{
//							0xff, 0xff, 0xff, 0x00,  0xff, 0xff, 0xff, 0x00,  0x00, 0x00, 0x00, 0xff,  0xff, 0xff, 0xff, 0x00,  0xff, 0xff, 0xff, 0x00,
//							0xff, 0xff, 0xff, 0x00,  0x00, 0x00, 0x00, 0xff,  0xff, 0xff, 0xff, 0x00,  0x00, 0x00, 0x00, 0xff,  0xff, 0xff, 0xff, 0x00,
//							0xff, 0xff, 0xff, 0x00,  0x00, 0x00, 0x00, 0xff,  0xff, 0xff, 0xff, 0x00,  0x00, 0x00, 0x00, 0xff,  0xff, 0xff, 0xff, 0x00,
//							0x00, 0x00, 0x00, 0xff,  0x00, 0x00, 0x00, 0xff,  0x00, 0x00, 0x00, 0xff,  0x00, 0x00, 0x00, 0xff,  0x00, 0x00, 0x00, 0xff,
//							0x00, 0x00, 0x00, 0xff,  0xff, 0xff, 0xff, 0x00,  0xff, 0xff, 0xff, 0x00,  0xff, 0xff, 0xff, 0x00,  0x00, 0x00, 0x00, 0xff,
//							0x00, 0x00, 0x00, 0xff,  0xff, 0xff, 0xff, 0x00,  0xff, 0xff, 0xff, 0x00,  0xff, 0xff, 0xff, 0x00,  0x00, 0x00, 0x00, 0xff,
//						};
//	image		letterAImage;
//	letterAImage.init( 5, 6, letterA, false, 32 );
//	
//	myfb.image().draw_image( 110, 110, letterAImage );
//	
//	myfb.flush();	// Currently a no-op on Linux, but needed on Mac for drawing to actually be shown in UI.
//	
//	int		r,g,b,a;
//	myfb.image().set_pixel( 4, 4, 0xff, 0x00, 0x00, 0xee );
//	myfb.image().get_pixel( 4, 4, &r, &g, &b, &a );
//	
//	image	mask;
//	mask.init( 33, 12, nullptr, true, 1 );
//	mask.fill_rect( 0, 0 ,mask.width(), mask.height(), 0, 0, 0, 0 );
//	mask.set_pixel( 2, 2, 0xff, 0xff, 0xff, 0xff );
//	mask.get_pixel( 2, 2, &r, &g, &b, &a );
//	if( r != g || g != b || b != 0xff )
//		cout << "Ouch!" << endl;
//	mask.get_pixel( 0, 2, &r, &g, &b, &a );
//	if( r != g || g != b || b != 0x00 )
//		cout << "Ouch too!" << endl;
	
    return 0;
}
