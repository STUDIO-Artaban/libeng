//
//  EngFacebook.h
//
//  Created by Pascal Viguié on 10/07/2014.
//  Copyright (c) 2014 Pascal Viguié. All rights reserved.
//

#include "Main.h"

#ifdef LIBENG_ENABLE_SOCIAL
#import <Foundation/Foundation.h>
#import "EngSocial.h"
#import "EngController.h"

#import <FacebookSDK/FBSession.h>


@interface EngFacebook : EngSocial <SocialOS> {

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
-(bool)shareVideo:(NSArray*)data;

-(void)resume;
-(void)terminate;
-(void)remove;

//
@property (nonatomic, strong) EngController* controller;
@property (nonatomic, strong) NSMutableArray* permissions;
@property (nonatomic, strong) FBSession* session;
@property (nonatomic, strong) NSDictionary* shareData;
@property (nonatomic, strong) NSData* videoData;

+(BOOL)openURL:(NSURL*)url sourceApplication:(NSString*)sourceApplication;

@end //

#endif // LIBENG_ENABLE_SOCIAL