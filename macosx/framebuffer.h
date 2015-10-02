#pragma once

#include <iostream>
#include <cstdint>
#include <cassert>


extern "C" void	UpdateFrameBuffer( size_t w, size_t h, size_t bpp, uint8_t* buffer );


namespace winner
{
	struct bufferprops
	{
		size_t		xres;
		size_t		yres;
		size_t		bits_per_pixel;
		size_t		xoffset;
		size_t		yoffset;
	};
	
	struct bufferinfo
	{
		size_t		line_length;
	};
	
    class framebuffer
    {
    public:
        framebuffer() : mFramebuffer(nullptr)
        {
			mVariableScreenProperties.xres = 640;
			mVariableScreenProperties.yres = 480;
			mVariableScreenProperties.bits_per_pixel = 32;
			mVariableScreenProperties.xoffset = 0;
			mVariableScreenProperties.yoffset = 0;
			mFixedScreenProperties.line_length = mVariableScreenProperties.xres * (mVariableScreenProperties.bits_per_pixel / 8);
			
			std::cout << mVariableScreenProperties.xres << "x" << mVariableScreenProperties.yres << ", " << mVariableScreenProperties.bits_per_pixel << "bpp" << std::endl;

            size_t	frameBufferSize = size_t(mVariableScreenProperties.xres * mVariableScreenProperties.yres * (mVariableScreenProperties.bits_per_pixel / 8));

            mFramebuffer = (uint8_t*) malloc( frameBufferSize );
        }
        ~framebuffer()
        {
            if( mFramebuffer )
            {
                free( mFramebuffer );
            }
        }

        bool    valid() { return mFramebuffer != nullptr; }

        size_t		bits_per_pixel()	{ return mVariableScreenProperties.bits_per_pixel; }
        size_t		screen_width()		{ return mVariableScreenProperties.xres; }
        size_t		screen_height()		{ return mVariableScreenProperties.yres; }

        uint8_t*	pixel_at( size_t x, size_t y )
        {
            return mFramebuffer +((x + mVariableScreenProperties.xoffset) * (mVariableScreenProperties.bits_per_pixel / 8) + (y + mVariableScreenProperties.yoffset) * mFixedScreenProperties.line_length);
        }
		
        void		set_pixel( size_t x, size_t y, int r, int g, int b, int a )
        {
            uint8_t*	currPixel = pixel_at( x, y );
            if( mVariableScreenProperties.bits_per_pixel == 32 )
            {
                uint32_t*	pixel = (uint32_t*)currPixel;
                *pixel = ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
            }
            else if( mVariableScreenProperties.bits_per_pixel == 16 )
            {
                uint16_t*	pixel = (uint16_t*)currPixel;
                *pixel = ((a & 0x0f) << 12) | ((r & 0x0f) << 8) | ((g & 0x0f) << 4) | (b & 0x0f);
            }
            else
                std::cerr << "Unsupported depth " << mVariableScreenProperties.bits_per_pixel << "bpp" << std::endl;
        }

		void	fill_rect( size_t x, size_t y, size_t w, size_t h, int r, int g, int b, int a );
		void	stroke_rect( size_t x, size_t y, size_t w, size_t h, int r, int g, int b, int a, size_t lineWidth );
		void	fill_circle( size_t x, size_t y, size_t radius, int r, int g, int b, int a );
		void	stroke_circle( size_t x, size_t y, size_t radius, int r, int g, int b, int a, size_t lineWidth );
		void	stroke_line( size_t startX, size_t startY, size_t endX, size_t endY, int r, int g, int b, int a );
		
    protected:
        int							mFrameBufferDevice;
        uint8_t*					mFramebuffer;
        struct bufferprops			mVariableScreenProperties;
        struct bufferinfo			mFixedScreenProperties;
    };

}   /* namespace winner */


