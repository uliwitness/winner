//
//  image.hpp
//  winner
//
//  Created by Uli Kusterer on 03/10/15.
//  Copyright © 2015 Uli Kusterer. All rights reserved.
//

#pragma once

#include <cstdint>
#include <cassert>
#include <cstring>


namespace winner
{
	using namespace std;
	
	typedef int32_t	coordinate_t;
	typedef uint8_t	color_component_t;
	
    class image
    {
    public:
		image() : mPixelData(nullptr), mMask(nullptr), mSysMask(nullptr), mFreesPixelData(false) {}	// Call init() after this to set up the object.
		
		image( coordinate_t inWidth, coordinate_t inHeight ) : mPixelData(nullptr), mMask(nullptr), mSysMask(nullptr), mFreesPixelData(false)
        {
			init( inWidth, inHeight, nullptr, true, 0, 0, 0, 0 );
		}
		
		~image()
        {
            if( mPixelData && mFreesPixelData )
            {
                delete [] mPixelData;
            }
			
			if( mMask )
				delete mMask;
			
			if( mSysMask )
				delete mSysMask;
        }
		
		void		init( coordinate_t inWidth, coordinate_t inHeight, uint8_t* inPixelData = nullptr, bool inFreePixelData = false, size_t inBitsPerPixel = 32, size_t inRowBytes = 0, coordinate_t inXOffset = 0, coordinate_t inYOffset = 0 )	// May be called several times to re-allocate a buffer.
        {
			if( mPixelData && mFreesPixelData )
				delete [] mPixelData;
			
			mWidth = inWidth;
			mHeight = inHeight;
			mBitsPerPixel = (inBitsPerPixel != 0) ? inBitsPerPixel : 32;
			mXOffset = inXOffset;
			mYOffset = inYOffset;
			if( inRowBytes == 0 && mBitsPerPixel >= 8 )
				mRowBytes = mWidth * mBitsPerPixel / 8;
			else if( inRowBytes == 0 )
			{
				mRowBytes = mWidth;
				if( mWidth % 8 != 0 )
					mRowBytes += 8 -(mWidth % 8);
				mRowBytes /= 8;
			}
			else
				mRowBytes = inRowBytes;
			
			if( inPixelData )
			{
				mPixelData = inPixelData;
				mFreesPixelData = inFreePixelData;
			}
			else
			{
				size_t	frameBufferSize = size_t( mHeight * mRowBytes );
			
				mPixelData = new uint8_t[ frameBufferSize ];
				mFreesPixelData = true;
			}
        }

        bool			valid() const							{ return mPixelData != nullptr; }

        size_t			bits_per_pixel() const					{ return mBitsPerPixel; }
        coordinate_t	width() const							{ return mWidth; }
        coordinate_t	height() const							{ return mHeight; }
		uint8_t*		pixel_data() const						{ return mPixelData; }
		void			set_frees_pixel_data( bool inState )	{ mFreesPixelData = inState; }
		bool			get_frees_pixel_data( bool inState )	{ return mFreesPixelData; }
		size_t			row_bytes() const						{ return mRowBytes; }
		coordinate_t	x_offset() const						{ return mXOffset; }
		coordinate_t	y_offset() const						{ return mYOffset; }

        uint8_t*	pixel_at( coordinate_t x, coordinate_t y ) const
        {
			if( x < 0 || x >= mWidth || y < 0 || y >= mHeight )
				return nullptr;
            return mPixelData +((x + mXOffset) * (mBitsPerPixel / 8) + (y + mYOffset) * mRowBytes);
        }
		
        void		set_pixel( coordinate_t x, coordinate_t y, color_component_t r, color_component_t g, color_component_t b, color_component_t a )
        {
			if( x < 0 || x >= mWidth || y < 0 || y >= mHeight )
				return;
			
			if( mMask )
			{
				color_component_t		r, g, b, a;
				mMask->get_pixel( x, y, &r, &g, &b, &a );
				if( a != 0xff )
					return;
			}
			
			if( mSysMask )
			{
				color_component_t		r, g, b, a;
				mSysMask->get_pixel( x, y, &r, &g, &b, &a );
				if( a != 0xff )
					return;
			}
			
			if( mBitsPerPixel == 1 )
			{
				uint8_t*	currPixel = mPixelData +(mRowBytes * y) + x / 8;
				size_t		pixelBit = x % 8;
				if( r == g && g == b && r == 0xff )
					*currPixel |= (1 << pixelBit);
				else
					*currPixel &= ~(1 << pixelBit);
				return;
			}
			
            uint8_t*	currPixel = pixel_at( x, y );
            if( mBitsPerPixel == 32 )
            {
                uint32_t*	pixel = (uint32_t*)currPixel;
                *pixel = ((a & 0xff) << 24) | ((b & 0xff) << 16) | ((g & 0xff) << 8) | (r & 0xff);
            }
            else if( mBitsPerPixel == 16 )
            {
                uint16_t*	pixel = (uint16_t*)currPixel;
                *pixel = (((r & 0xff) >> 3) << 11) | (((g & 0xff) >> 3) << 6) | (((b & 0xff) >> 2) << 0);
            }
            else
                assert(mBitsPerPixel == 16 || mBitsPerPixel == 32);
        }
		
		void		get_pixel( coordinate_t x, coordinate_t y, color_component_t* r, color_component_t* g, color_component_t *b, color_component_t *a ) const
		{
			if( mBitsPerPixel == 1 )
			{
				if( x < 0 || y < 0 )
					return;
				
				uint8_t*	currPixel = mPixelData +(mRowBytes * y) + x / 8;
				size_t		pixelBit = x % 8;
				if( (*currPixel) & (1 << pixelBit) )
					*r = *g = *b = *a = 0xff;
				else
					*r = *g = *b = *a = 0x00;
				return;
			}
			
            uint8_t*	currPixel = pixel_at( x, y );
			if( !currPixel )
				return;
            if( mBitsPerPixel == 32 )
            {
                uint32_t	pixel = *(uint32_t*)currPixel;
				*a = (pixel >> 24) & 0xff;
				*b = (pixel >> 16) & 0xff;
				*g = (pixel >> 8) & 0xff;
				*r = pixel & 0xff;
            }
            else if( mBitsPerPixel == 16 )
            {
                uint16_t	pixel = *(uint16_t*)currPixel;
				
				*a = 0xff;	// Don't have alpha in 16 bit. We natively use 16 bits, so the user is reading the screen here, which is always opaque.
				*r = ((pixel >> 11) << 3) & 0xff;
				*r |= (*r & 0xE0) >> 5;	// Duplicate the top few bits at the bottom, otherwise 11111 turns into 11111000 instead of 11111111.
				*g = ((pixel >> 6) << 3) & 0xff;
				*g |= (*g & 0xE0) >> 5;	// Duplicate the top few bits at the bottom, otherwise 11111 turns into 11111000 instead of 11111111.
				*b = (pixel << 2) & 0xff;
				*b |= (*b & 0xC0) >> 6;	// Duplicate the top few bits at the bottom, otherwise 11111 turns into 11111000 instead of 11111111.
            }
            else
                assert(mBitsPerPixel == 16 || mBitsPerPixel == 32);
		}
		
		void	clear( uint8_t alpha )
		{
			if( !mFreesPixelData )
			{
				fill_rect( 0, 0, mWidth, mHeight, alpha, alpha, alpha, alpha );
			}
			else
			{
				size_t	frameBufferSize = size_t( mHeight * mRowBytes );
				memset( mPixelData, alpha, frameBufferSize );
			}
		}

		void	fill_rect( coordinate_t x, coordinate_t y, coordinate_t w, coordinate_t h, color_component_t r, color_component_t g, color_component_t b, color_component_t a );
		void	stroke_rect( coordinate_t x, coordinate_t y, coordinate_t w, coordinate_t h, color_component_t r, color_component_t g, color_component_t b, color_component_t a, coordinate_t lineWidth );
		void	fill_circle( coordinate_t x, coordinate_t y, coordinate_t radius, color_component_t r, color_component_t g, color_component_t b, color_component_t a );
		void	stroke_circle( coordinate_t x, coordinate_t y, coordinate_t radius, color_component_t r, color_component_t g, color_component_t b, color_component_t a, coordinate_t lineWidth );
		void	stroke_line( coordinate_t startX, coordinate_t startY, coordinate_t endX, coordinate_t endY, color_component_t r, color_component_t g, color_component_t b, color_component_t a );
		void	stroke_line( coordinate_t startX, coordinate_t startY, coordinate_t endX, coordinate_t endY, color_component_t r, color_component_t g, color_component_t b, color_component_t a, coordinate_t lineWidth );
		void	draw_image( coordinate_t x, coordinate_t y, const image& inImage );
		void	clear_image( coordinate_t x, coordinate_t y, const image& inImage );
		
		image&	mask()
		{
			if( !mMask )
			{
				mMask = new image;
				mMask->init( mWidth, mHeight, nullptr, true, 1 );
				mMask->clear( 0x00 );
			}
			return *mMask;
		}
		void	remove_mask()	{ if( mMask ) { delete mMask; mMask = nullptr; } };
		bool	has_mask()		{ return mMask != nullptr; }
		
		image&	sys_mask()
		{
			if( !mSysMask )
			{
				mSysMask = new image;
				mSysMask->init( mWidth, mHeight, nullptr, true, 1 );
			}
			return *mSysMask;
		}
		void	remove_sys_mask()	{ if( mSysMask ) { delete mSysMask; mSysMask = nullptr; } };
		bool	has_sys_mask()		{ return mMask != nullptr; }
		
    protected:
        uint8_t*					mPixelData;
		coordinate_t				mWidth;
		coordinate_t				mHeight;
		size_t						mBitsPerPixel;
		coordinate_t				mXOffset;
		coordinate_t				mYOffset;
		size_t						mRowBytes;
		bool						mFreesPixelData;
		image*						mMask;
		image*						mSysMask;
    };

}   /* namespace winner */
