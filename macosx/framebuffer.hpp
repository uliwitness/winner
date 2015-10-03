#pragma once

#include "image.hpp"


extern "C" void	UpdateFrameBuffer( size_t w, size_t h, size_t bpp, uint8_t* buffer );


namespace winner
{
    class framebuffer
    {
    public:
        framebuffer()
        {
			mImage.init( 640, 480 );
        }
        ~framebuffer()
        {
			
        }
		
		bool			valid() { return mImage.valid(); }
		
		winner::image&	image()	{ return mImage; }

		void			flush()	{ UpdateFrameBuffer( mImage.width(), mImage.height(), mImage.bits_per_pixel(), mImage.pixel_data() ); };
		
    protected:
        winner::image		mImage;
    };

}   /* namespace winner */


