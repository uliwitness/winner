#include <iostream>
#include "framebuffer.h"

using namespace std;
using namespace winner;

int main()
{
    framebuffer myfb;
    if( !myfb.valid() )
    {
		cerr << "Couldn't open frame buffer." << endl;
		return 1;
    }

	myfb.fill_rect( 0, 0, myfb.screen_width(), myfb.screen_height(), 0xff, 0xff, 0xff, 0xff );
	myfb.fill_rect( 100, 100, 200, 50, 0x00, 0xff, 0x00, 0xff );
	myfb.stroke_rect( 100, 100, 200, 50, 0xff, 0x00, 0x00, 0xff, 1 );
	myfb.stroke_rect( 0, 0, myfb.screen_width(), myfb.screen_height(), 0x00, 0x00, 0xff, 0xff, 2 );
	myfb.fill_circle( 170, 170, 50, 0xff, 0x00, 0x00, 0xff );
	myfb.stroke_circle( 30, 30, 40, 0xff, 0xee, 0xee, 0xff, 4 );

    return 0;
}
