//
//  EngController.mm
//
//  Created by Pascal Viguié on 11/07/2014.
//  Copyright (c) 2014 Pascal Viguié. All rights reserved.
//

#import "EngController.h"

#ifdef LIBENG_ENABLE_INTERNET
#import <libeng/Features/Internet/Internet.h>

#import <netdb.h>
#import <SystemConfiguration/SystemConfiguration.h>
#endif


//////
@implementation EngController

-(id)initWithNibName:(NSString*)nibNameOrNil bundle:(NSBundle*)nibBundleOrNil {

    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
        alertMessage = [[UIAlertView alloc] initWithTitle:nil message:nil delegate:nil cancelButtonTitle:nil
                                        otherButtonTitles:nil];
    return self;
}
-(void)viewDidLoad { [super viewDidLoad]; }
-(void)didReceiveMemoryWarning { [super didReceiveMemoryWarning]; }
-(void)alert:(NSString*)message during:(double)delay {
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{

        alertMessage.message = message;
        [alertMessage show];
        [NSTimer scheduledTimerWithTimeInterval:delay target:self selector:@selector(alertExpired:) userInfo:nil
                                        repeats:NO];
    }];
}
-(void)alertExpired:(NSTimer*)timer {

    [alertMessage dismissWithClickedButtonIndex:0 animated:YES];
}

#ifdef LIBENG_ENABLE_INTERNET
+(SCNetworkReachabilityFlags)getNetworkFlags:(BOOL)local {

    SCNetworkReachabilityRef reachability = NULL;
    if (local) {

        struct sockaddr_in zeroAddress;
        bzero(&zeroAddress, sizeof(zeroAddress));
        zeroAddress.sin_len = sizeof(zeroAddress);
        zeroAddress.sin_family = AF_INET;
        
        reachability = SCNetworkReachabilityCreateWithAddress(kCFAllocatorDefault,
                                                              (const struct sockaddr*)&zeroAddress);
    }
    else
        reachability = SCNetworkReachabilityCreateWithName(kCFAllocatorDefault, "www.google.com");

    if (reachability != NULL) {
        SCNetworkReachabilityFlags flags;
        if (SCNetworkReachabilityGetFlags(reachability, &flags))
            return flags;
    }
    return 0;
}

+(unsigned char)isConnected {

    SCNetworkReachabilityFlags flags = [EngController getNetworkFlags:TRUE];
    if (flags & kSCNetworkReachabilityFlagsReachable) {
        if ((flags & kSCNetworkReachabilityFlagsIsWWAN) != kSCNetworkReachabilityFlagsIsWWAN)
            return Internet::CONNECTION_WIFI; // Not 3G/4G -> Wi-Fi

        return Internet::CONNECTION_UNKNOWN;
    }
    return Internet::CONNECTION_NONE;
}
+(bool)isOnline {

    SCNetworkReachabilityFlags flags = [EngController getNetworkFlags:FALSE];
    if ((flags != 0) &&
        ((((flags & kSCNetworkReachabilityFlagsConnectionRequired) == 0) &&
          ((flags & kSCNetworkFlagsReachable) != 0)) ||
         ((flags & kSCNetworkReachabilityFlagsIsWWAN) == kSCNetworkReachabilityFlagsIsWWAN) ||
         (((((flags & kSCNetworkReachabilityFlagsConnectionOnDemand) != 0) ||
            (flags & kSCNetworkReachabilityFlagsConnectionOnTraffic) != 0))  &&
          ((flags & kSCNetworkReachabilityFlagsInterventionRequired) == 0))))
        return true;

    return false;
}
#endif

-(void)dealloc {

    [alertMessage release];
    [super dealloc];
}

@end
