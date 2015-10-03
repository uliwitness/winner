#pragma once

#include <iostream>
#include <cstdint>
#include <cassert>
#include <linux/fb.h>	// FBIOGET_FSCREENINFO etc.
#include <unistd.h>		// open/close etc.
#include <fcntl.h>		// O_RDWR etc.
#include <sys/mman.h>	// mmap
#include <sys/ioctl.h>


#define UpdateFrameBuffer(...)	do{}while(0)


namespace winner
{

    class framebuffer
    {
    public:
        framebuffer() : mFrameBufferDevice(-1), mFramebuffer(nullptr)
        {
            mFrameBufferDevice = open( "/dev/fb0", O_RDWR );
            if( mFrameBufferDevice < 0 )
            {
                perror( "Couldn't open frame buffer device." );
                close(mFrameBufferDevice);
                mFrameBufferDevice = -1;
                return;
            }

            if( ioctl( mFrameBufferDevice, FBIOGET_FSCREENINFO, &mFixedScreenProperties ) == -1 )
            {
                perror( "Couldn't determine fixed screen properties." );
                close(mFrameBufferDevice);
                mFrameBufferDevice = -1;
                return;
            }

            if( ioctl( mFrameBufferDevice, FBIOGET_VSCREENINFO, &mVariableScreenProperties ) == -1 )
            {
                perror( "Couldn't determine variable screen properties." );
                close(mFrameBufferDevice);
                mFrameBufferDevice = -1;
                return;
            }

            std::cout << mVariableScreenProperties.xres << "x" << mVariableScreenProperties.yres << ", " << mVariableScreenProperties.bits_per_pixel << "bpp" << std::endl;

            size_t	frameBufferSize = size_t(mVariableScreenProperties.xres * mVariableScreenProperties.yres * (mVariableScreenProperties.bits_per_pixel / 8));

            mFramebuffer = (uint8_t *)mmap( 0, frameBufferSize, PROT_READ | PROT_WRITE, MAP_SHARED, mFrameBufferDevice, 0 );
            if( mFramebuffer == (uint8_t*)-1 )
            {
                perror("Couldn't map framebuffer into memory");
                close(mFrameBufferDevice);
                mFrameBufferDevice = -1;
                return;
            }
        }
        ~framebuffer()
        {
            if( mFramebuffer )
            {
                size_t	frameBufferSize = mVariableScreenProperties.xres * mVariableScreenProperties.yres * (mVariableScreenProperties.bits_per_pixel / 8);
                munmap( mFramebuffer, frameBufferSize );
            }
            if( mFrameBufferDevice != -1 )
            {
                close( mFrameBufferDevice );
            }
        }

        bool    valid() { return mFrameBufferDevice != -1; }

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
		void	stroke_line( size_t startX, size_t startY, size_t endX, size_t endY, int r, int g, int b, int a, size_t lineWidth );
		void	draw_image_data( size_t x, size_t y, size_t w, size_t h, uint8_t* data );
		
    protected:
        int							mFrameBufferDevice;
        uint8_t*					mFramebuffer;
        struct fb_var_screeninfo	mVariableScreenProperties;
        struct fb_fix_screeninfo	mFixedScreenProperties;
    };

}   /* namespace winner */


