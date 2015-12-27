//
//  EngResources.h
//
//  Created by Pascal Viguié on 22/01/14.
//  Copyright (c) 2014 Pascal Viguié. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CGImage.h>


@interface EngResources : NSObject {

    NSDictionary* deviceDpi;
}

@property unsigned int pngWidth;
@property unsigned int pngHeight;

@property unsigned int oggLength;

-(id)init;

+(unsigned char*)genBufferIMG:(CGImageRef)image;

-(float)getDpi;
-(unsigned char*)getBufferPNG:(NSString*)Name inGrayScale:(BOOL)grayScale;
-(unsigned char*)getBufferOGG:(NSString*)Name;

@end //