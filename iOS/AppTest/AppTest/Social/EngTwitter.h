//
//  EngTwitter.h
//
//  Created by Pascal Viguié on 10/07/2014.
//  Copyright (c) 2014 Pascal Viguié. All rights reserved.
//

#include "Main.h"

#ifdef LIBENG_ENABLE_SOCIAL
#import <Foundation/Foundation.h>
#import "EngSocial.h"
#import "EngController.h"


@interface EngTwitter : EngSocial <SocialOS> {

    bool displayError;
    NSString* userID;

    NSString* userName;
    unsigned char userGender;
    NSDate* userBirthday;
    NSString* userLocation;
}
-(id)initWithDisplayError:(bool)display on:(EngController*)viewController;

-(bool)login;
-(void)logout;
-(bool)isLogged;

-(bool)getUserInfo;
-(bool)getUserPicture;

-(bool)shareLink:(NSArray*)data;

-(void)resume;
-(void)terminate;
-(void)remove;

//
@property (nonatomic, strong) EngController* controller;

@end //

#endif // LIBENG_ENABLE_SOCIAL
