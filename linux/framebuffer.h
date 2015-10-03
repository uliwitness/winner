#pragma once

#include <iostream>
#include <cstdint>
#include <cassert>
#include <linux/fb.h>	// FBIOGET_FSCREENINFO etc.
#include <unistd.h>		// open/close etc.
#include <fcntl.h>		// O_RDWR etc.
#include <sys/mman.h>	// mmap
#include <sys/ioctl.h>
#include "image.hpp"


namespace winner
{

    class framebuffer
    {
    public:
        framebuffer() : mFrameBufferDevice(-1), mFramebuffer(nullptr)
        {
			struct fb_var_screeninfo	variableScreenProperties;
			struct fb_fix_screeninfo	fixedScreenProperties;

            mFrameBufferDevice = open( "/dev/fb0", O_RDWR );
            if( mFrameBufferDevice < 0 )
            {
                perror( "Couldn't open frame buffer device." );
                close(mFrameBufferDevice);
                mFrameBufferDevice = -1;
                return;
            }

            if( ioctl( mFrameBufferDevice, FBIOGET_FSCREENINFO, &fixedScreenProperties ) == -1 )
            {
                perror( "Couldn't determine fixed screen properties." );
                close(mFrameBufferDevice);
                mFrameBufferDevice = -1;
                return;
            }

            if( ioctl( mFrameBufferDevice, FBIOGET_VSCREENINFO, &variableScreenProperties ) == -1 )
            {
                perror( "Couldn't determine variable screen properties." );
                close(mFrameBufferDevice);
                mFrameBufferDevice = -1;
                return;
            }

            std::cout << variableScreenProperties.xres << "x" << variableScreenProperties.yres << ", " << variableScreenProperties.bits_per_pixel << "bpp" << std::endl;

            size_t	frameBufferSize = size_t(variableScreenProperties.xres * variableScreenProperties.yres * (variableScreenProperties.bits_per_pixel / 8));

            mFramebuffer = (uint8_t *)mmap( 0, frameBufferSize, PROT_READ | PROT_WRITE, MAP_SHARED, mFrameBufferDevice, 0 );
            if( mFramebuffer == (uint8_t*)-1 )
            {
                perror("Couldn't map framebuffer into memory");
                close(mFrameBufferDevice);
                mFrameBufferDevice = -1;
                return;
            }
			
			mImage.init( variableScreenProperties.xres, variableScreenProperties.yres, mFramebuffer, false, size_t variableScreenProperties.bits_per_pixel, fixedScreenProperties.line_length, variableScreenProperties.xoffset, variableScreenProperties.yoffset );
        }
        ~framebuffer()
        {
            if( mImage.pixel_data() )
            {
                size_t	frameBufferSize = mImage.width() * mImage.height() * (mImage.bits_per_pixel() / 8);
                munmap( mImage.pixel_data(), frameBufferSize );
            }
            if( mFrameBufferDevice != -1 )
            {
                close( mFrameBufferDevice );
            }
        }

        bool    valid() { return mFrameBufferDevice != -1 && mImage.valid(); }
		
    protected:
        int							mFrameBufferDevice;
        uint8_t*					mFramebuffer;
    };

}   /* namespace winner */


