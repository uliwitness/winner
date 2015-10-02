//
//  AppDelegate.m
//  winner
//
//  Created by Uli Kusterer on 01/10/15.
//  Copyright © 2015 Uli Kusterer. All rights reserved.
//

#import "AppDelegate.h"


extern "C" void	UpdateFrameBuffer( size_t w, size_t h, size_t bpp, uint8_t* buffer )
{
	NSImage*			img = [[NSImage alloc] initWithSize: NSMakeSize(w, h)];
	NSBitmapImageRep*	bir = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes: &buffer pixelsWide: w pixelsHigh: h bitsPerSample: bpp / 4 samplesPerPixel: 4 hasAlpha: YES isPlanar: NO colorSpaceName:NSCalibratedRGBColorSpace bitmapFormat: 0 bytesPerRow: (w * bpp) / 8 bitsPerPixel: bpp];
	[img addRepresentation: bir];
	
	[(AppDelegate*)[[NSApplication sharedApplication] delegate] imageView].image = img;
}


#define main	RunThing
#include "main.cpp"
#undef main


@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	RunThing();
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	// Insert code here to tear down your application
}

@end
