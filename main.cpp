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

	myfb.fill( 0, 0, myfb.screen_width(), myfb.screen_height(), 0xff, 0xff, 0xff, 0xff );
	myfb.fill( 100, 100, 200, 50, 0x00, 0xff, 0x00, 0xff );

    return 0;
}
