//
//  EngSocial.mm
//
//  Created by Pascal Viguié on 15/08/2014.
//  Copyright (c) 2014 Pascal Viguié. All rights reserved.
//

#import "EngSocial.h"

#ifdef LIBENG_ENABLE_SOCIAL


//////
@implementation EngSocial

-(id)initWith:(Network::ID)network {

    if (!(self = [super init]))
        return nil;

    networkID = network;
    return self;
}
-(void)getPictureFrom:(NSString*)url {

    NSURLRequest* reqURL = [NSURLRequest requestWithURL:[NSURL URLWithString:url]];
    [NSURLConnection sendAsynchronousRequest:reqURL queue:[[NSOperationQueue alloc] init]
        completionHandler:^(NSURLResponse* response, NSData* data, NSError* error) {
            if (error) {

                NSLog(@"ERROR (Request picture): %@ (network:%i;line:%d)", [error description],
                      static_cast<short>(networkID), __LINE__);
                platformLoadSocial(networkID, Session::REQUEST_PICTURE, Request::RESULT_FAILED, 0, 0, NULL);
            }
            else if (![data length]) {

                NSLog(@"ERROR (Request picture): Empty data (network:%i;line:%d)",
                      static_cast<short>(networkID), __LINE__);
                platformLoadSocial(networkID, Session::REQUEST_PICTURE, Request::RESULT_FAILED, 0, 0, NULL);
            }
            else {

                UIImage* picImage = [[UIImage alloc] initWithData:data];
                CGImageRef imgRef = [picImage CGImage];
                platformLoadSocial(networkID, Session::REQUEST_PICTURE, Request::RESULT_SUCCEEDED,
                        static_cast<short>(CGImageGetWidth(imgRef)),
                        static_cast<short>(CGImageGetHeight(imgRef)),
                        [EngResources genBufferIMG:imgRef]);
            }
    }];
}

@end //

#endif // LIBENG_ENABLE_SOCIAL
