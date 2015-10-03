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


namespace winner
{
	using namespace std;
	
    class image
    {
    public:
		image() : mPixelData(nullptr) {}	// Call init() after this to set up the object.
		
		image( size_t inWidth, size_t inHeight ) : mPixelData(nullptr)
        {
			init( inWidth, inHeight, nullptr, true, 0, 0, 0, 0 );
		}
		
		~image()
        {
            if( mPixelData && mFreesPixelData )
            {
                delete [] mPixelData;
            }
        }
		
		void		init( size_t inWidth, size_t inHeight, uint8_t* inPixelData = nullptr, bool inFreePixelData = false, size_t inBitsPerPixel = 32, size_t inRowBytes = 0, size_t inXOffset = 0, size_t inYOffset = 0 )
        {
			mWidth = inWidth;
			mHeight = inHeight;
			mBitsPerPixel = (inBitsPerPixel != 0) ? inBitsPerPixel : 32;
			mXOffset = inXOffset;
			mYOffset = inYOffset;
			if( inRowBytes == 0 )
				mRowBytes = mWidth * (mBitsPerPixel / 8);
			else
				mRowBytes = inRowBytes;
			
            size_t	frameBufferSize = size_t( mWidth * mHeight * (mBitsPerPixel / 8));
			
			if( inPixelData )
			{
				mPixelData = inPixelData;
				mFreesPixelData = inFreePixelData;
			}
			else
			{
				mPixelData = new uint8_t[ frameBufferSize ];
				mFreesPixelData = true;
			}
        }

        bool		valid() const							{ return mPixelData != nullptr; }

        size_t		bits_per_pixel() const					{ return mBitsPerPixel; }
        size_t		width() const							{ return mWidth; }
        size_t		height() const							{ return mHeight; }
		uint8_t*	pixel_data() const						{ return mPixelData; }
		void		set_frees_pixel_data( bool inState )	{ mFreesPixelData = inState; }
		bool		get_frees_pixel_data( bool inState )	{ return mFreesPixelData; }
		size_t		row_bytes() const						{ return mRowBytes; }

        uint8_t*	pixel_at( size_t x, size_t y ) const
        {
            return mPixelData +((x + mXOffset) * (mBitsPerPixel / 8) + (y + mYOffset) * mRowBytes);
        }
		
        void		set_pixel( size_t x, size_t y, int r, int g, int b, int a )
        {
            uint8_t*	currPixel = pixel_at( x, y );
            if( mBitsPerPixel == 32 )
            {
                uint32_t*	pixel = (uint32_t*)currPixel;
                *pixel = ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
            }
            else if( mBitsPerPixel == 16 )
            {
                uint16_t*	pixel = (uint16_t*)currPixel;
                *pixel = (((r & 0xff) >> 3) << 10) | (((g & 0xff) >> 3) << 5) | ((b & 0xff) >> 3);
            }
            else
                assert(mBitsPerPixel == 16 || mBitsPerPixel == 32);
        }

		void	fill_rect( size_t x, size_t y, size_t w, size_t h, int r, int g, int b, int a );
		void	stroke_rect( size_t x, size_t y, size_t w, size_t h, int r, int g, int b, int a, size_t lineWidth );
		void	fill_circle( size_t x, size_t y, size_t radius, int r, int g, int b, int a );
		void	stroke_circle( size_t x, size_t y, size_t radius, int r, int g, int b, int a, size_t lineWidth );
		void	stroke_line( size_t startX, size_t startY, size_t endX, size_t endY, int r, int g, int b, int a );
		void	stroke_line( size_t startX, size_t startY, size_t endX, size_t endY, int r, int g, int b, int a, size_t lineWidth );
		void	draw_image( size_t x, size_t y, const image& inImage );
		
    protected:
        uint8_t*					mPixelData;
		size_t						mWidth;
		size_t						mHeight;
		size_t						mBitsPerPixel;
		size_t						mXOffset;
		size_t						mYOffset;
		size_t						mRowBytes;
		bool						mFreesPixelData;
    };

}   /* namespace winner */
