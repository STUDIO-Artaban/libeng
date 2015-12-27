//
//  EngController.h
//
//  Created by Pascal Viguié on 11/07/2014.
//  Copyright (c) 2014 Pascal Viguié. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "Main.h"


@interface EngController : UIViewController {

    UIAlertView* alertMessage;
}
-(void)alert:(NSString*)message during:(double)delay;
-(void)alertExpired:(NSTimer*)timer;

#ifdef LIBENG_ENABLE_INTERNET
+(unsigned char)isConnected;
+(bool)isOnline;
#endif

@end //
