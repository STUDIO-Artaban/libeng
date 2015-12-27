//
//  EngResources.mm
//
//  Created by Pascal Viguié on 22/01/14.
//  Copyright (c) 2014 Pascal Viguié. All rights reserved.
//

#import "EngResources.h"
#import <sys/utsname.h>

#import <CoreGraphics/CGContext.h>
#import <CoreGraphics/CGBitmapContext.h>
#import <UIKit/UIImage.h>


NSString* deviceName()
{
    struct utsname systemInfo;
    if (uname(&systemInfo) < 0)
        return nil;

    return [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
}

//////
@implementation EngResources

-(id)init {
    if (self == [super init]) {

        _pngWidth = 0;
        _pngHeight = 0;
        _oggLength = 0;

        deviceDpi = [[NSDictionary alloc] initWithObjectsAndKeys:
            [[NSNumber alloc] initWithShort:163], @"iPod1,1", // on iPod Touch
            [[NSNumber alloc] initWithShort:163], @"iPod2,1", // on iPod Touch Second Generation
            [[NSNumber alloc] initWithShort:163], @"iPod3,1", // on iPod Touch Third Generation
            [[NSNumber alloc] initWithShort:326], @"iPod4,1", // on iPod Touch Fourth Generation
            [[NSNumber alloc] initWithShort:326], @"iPod5,1", // on iPod Touch Fifth Generation
            [[NSNumber alloc] initWithShort:163], @"iPhone1,1", // on 1st Generation iPhone
            [[NSNumber alloc] initWithShort:163], @"iPhone1,2", // on 1st Generation iPhone 3G
            [[NSNumber alloc] initWithShort:163], @"iPhone2,1", // on 1st Generation iPhone 3GS
            [[NSNumber alloc] initWithShort:132], @"iPad1,1", // on iPad
            [[NSNumber alloc] initWithShort:132], @"iPad2,1", // on iPad 2
            [[NSNumber alloc] initWithShort:132], @"iPad2,2", // on iPad 2 3G
            [[NSNumber alloc] initWithShort:132], @"iPad2,3", // on iPad 2 3G
            [[NSNumber alloc] initWithShort:132], @"iPad2,4", // on iPad 2 16GB
            [[NSNumber alloc] initWithShort:264], @"iPad3,1", // on 3rd Generation iPad
            [[NSNumber alloc] initWithShort:264], @"iPad3,2", // on 3rd Generation iPad
            [[NSNumber alloc] initWithShort:264], @"iPad3,3", // on 3rd Generation iPad
            [[NSNumber alloc] initWithShort:326], @"iPhone3,1", // on iPhone 4
            [[NSNumber alloc] initWithShort:326], @"iPhone3,2", // on iPhone 4
            [[NSNumber alloc] initWithShort:326], @"iPhone3,3", // on iPhone 4
            [[NSNumber alloc] initWithShort:326], @"iPhone4,1", // on iPhone 4S
            [[NSNumber alloc] initWithShort:326], @"iPhone5,1", // on iPhone 5 (model A1428, AT&T/Canada)
            [[NSNumber alloc] initWithShort:326], @"iPhone5,2", // on iPhone 5 (model A1429, everything else)
            [[NSNumber alloc] initWithShort:264], @"iPad3,4", // on 4th Generation iPad (iPad 4)
            [[NSNumber alloc] initWithShort:264], @"iPad3,5", // on 4th Generation iPad (iPad 4)
            [[NSNumber alloc] initWithShort:264], @"iPad3,6", // on 4th Generation iPad (iPad 4)
            [[NSNumber alloc] initWithShort:163], @"iPad2,5", // on iPad Mini 1G
            [[NSNumber alloc] initWithShort:163], @"iPad2,6", // on iPad Mini 1G
            [[NSNumber alloc] initWithShort:163], @"iPad2,7", // on iPad Mini 1G
            [[NSNumber alloc] initWithShort:326], @"iPhone5,3", // on iPhone 5c (model A1456, A1532 | GSM)
            [[NSNumber alloc] initWithShort:326], @"iPhone5,4", // on iPhone 5c (model A1507, A1516, A1526 (China), A1529 | Global)
            [[NSNumber alloc] initWithShort:326], @"iPhone6,1", // on iPhone 5s (model A1433, A1533 | GSM)
            [[NSNumber alloc] initWithShort:326], @"iPhone6,2", // on iPhone 5s (model A1457, A1518, A1528 (China), A1530 | Global)
            [[NSNumber alloc] initWithShort:264], @"iPad4,1", // on 5th Generation iPad (iPad Air) - Wifi
            [[NSNumber alloc] initWithShort:264], @"iPad4,2", // on 5th Generation iPad (iPad Air) - Cellular
            [[NSNumber alloc] initWithShort:264], @"iPad4,3", // on 5th Generation iPad (iPad Air) - Cellular
            [[NSNumber alloc] initWithShort:326], @"iPad4,4", // on 2nd Generation iPad Mini - Wifi
            [[NSNumber alloc] initWithShort:326], @"iPad4,5", // on 2nd Generation iPad Mini - Cellular
            [[NSNumber alloc] initWithShort:326], @"iPad4,6", // on 2nd Generation iPad Mini
            [[NSNumber alloc] initWithShort:326], @"iPad4,7", // on 3nd Generation iPad Mini
            [[NSNumber alloc] initWithShort:326], @"iPad4,8", // on 3nd Generation iPad Mini
            [[NSNumber alloc] initWithShort:326], @"iPad4,9", // on 3nd Generation iPad Mini
            [[NSNumber alloc] initWithShort:264], @"iPad5,3", // on iPad Air 2 - Wifi
            [[NSNumber alloc] initWithShort:264], @"iPad5,4", // on iPad Air 2 - Wifi
            [[NSNumber alloc] initWithShort:401], @"iPhone7,1", // on iPhone 6 Plus
            [[NSNumber alloc] initWithShort:326], @"iPhone7,2", nil]; // on iPhone 6
    }
    return self;
}

+(unsigned char*)genBufferIMG:(CGImageRef)image {

    unsigned int imgWidth = CGImageGetWidth(image);
    unsigned int imgHeight = CGImageGetHeight(image);
    unsigned char* imgBuffer = new unsigned char[imgHeight * imgWidth * 4];

    NSUInteger bytesPerPixel = 4;
    NSUInteger bytesPerRow = bytesPerPixel * imgWidth;
    NSUInteger bitsPerComponent = 8;

    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(imgBuffer, imgWidth, imgHeight,
                                                 bitsPerComponent, bytesPerRow, colorSpace,
                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);

    CGContextDrawImage(context, CGRectMake(0, 0, imgWidth, imgHeight), image);
    CGContextRelease(context);

    return imgBuffer;
}

-(float)getDpi {

    NSString* device = deviceName();
    if (device != nil) {

        NSNumber* dpi = [deviceDpi objectForKey:device];
        if (dpi != nil)
            return [dpi floatValue];
    }
    return 160.f;
}
-(unsigned char*)getBufferPNG:(NSString*)Name inGrayScale:(BOOL)grayScale {
    @autoreleasepool {

        NSString* pngFilePath = [[NSBundle mainBundle] pathForResource:Name ofType:@"png"];
        if ([[NSFileManager defaultManager] fileExistsAtPath:pngFilePath]) {

            UIImage* pngImage = [[UIImage alloc] initWithContentsOfFile:pngFilePath];
            CGImageRef imgRef = [pngImage CGImage];
            _pngWidth = CGImageGetWidth(imgRef);
            _pngHeight = CGImageGetHeight(imgRef);

            unsigned char* pngBuffer = [EngResources genBufferIMG:imgRef];
            if (grayScale) {

                // Convert RGBA color buffer into LA buffer (Luminance Alpha)
                unsigned int graySize = _pngWidth * _pngHeight;
                unsigned char* grayBuffer = new unsigned char[graySize * 2];

                for (unsigned int i = 0; i < graySize; i++) {

                    grayBuffer[(i * 2) + 0] = pngBuffer[(i * 4) + 0]; // Luminance (R == G == B: + 0 -> Red)
                    grayBuffer[(i * 2) + 1] = pngBuffer[(i * 4) + 3]; // Alpha
                }
                delete [] pngBuffer;
                pngBuffer = grayBuffer;
            }
            [pngImage release];
            return pngBuffer;
        }
        else
            NSLog(@"ERROR: PNG file %@.png not found!", Name);
    }
    return NULL;
}

-(unsigned char*)getBufferOGG:(NSString*)Name {
    @autoreleasepool {

        NSString* oggFilePath = [[NSBundle mainBundle] pathForResource:Name ofType:@"ogg"];
        if ([[NSFileManager defaultManager] fileExistsAtPath:oggFilePath]) {

            NSData* oggData = [[NSData alloc] initWithContentsOfFile:oggFilePath];
            _oggLength = oggData.length;

            unsigned char* oggBuffer = new unsigned char[_oggLength];
            memcpy(oggBuffer, (const unsigned char*)[oggData bytes], _oggLength);

            [oggData release];
            return oggBuffer;
        }
        else
            NSLog(@"ERROR: Ogg file %@.ogg not found!", Name);
    }
    return NULL;
}

-(void)dealloc {

    [deviceDpi release];
    [super dealloc];
}

@end