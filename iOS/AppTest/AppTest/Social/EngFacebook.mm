//
//  EngFacebook.mm
//
//  Created by Pascal Viguié on 10/07/2014.
//  Copyright (c) 2014 Pascal Viguié. All rights reserved.
//

#import "Engfacebook.h"

#ifdef LIBENG_ENABLE_SOCIAL
#import <FacebookSDK/FBSettings.h>
#import <FacebookSDK/FBErrorUtility.h>
#import <FacebookSDK/FBRequestConnection.h>
#import <FacebookSDK/FBRequest.h>

#import <FacebookSDK/FacebookSDK.h>


//////
@implementation EngFacebook

@synthesize displayError;
@synthesize userID;

@synthesize userName;
@synthesize userGender;
@synthesize userBirthday;
@synthesize userLocation;

-(id)initWithDisplayError:(bool)display on:(EngController*)viewController {

    if (!(self = [super initWith:Network::FACEBOOK]))
        return nil;

    displayError = display;
    self.controller = [viewController retain];
    userID = nil;

    userName = nil;
    userGender = GENDER_NONE;
    userBirthday = nil;
    userLocation = nil;

    //
    self.shareData = nil;
    self.videoData = nil;

    NSSet* logBehavior = [[NSSet alloc] initWithObjects:FBLoggingBehaviorAccessTokens, nil];
    [FBSettings setLoggingBehavior:logBehavior];
    [logBehavior release];

    self.permissions = [NSMutableArray arrayWithObjects:@"publish_actions", nil];
    if (engReqInfoField(FIELD_BIRTHDAY, Network::FACEBOOK))
        [self.permissions addObject:@"user_birthday"];
    if (engReqInfoField(FIELD_LOCATION, Network::FACEBOOK))
        [self.permissions addObject:@"user_location"];

    self.session = [FBSession activeSession];
    if (self.session == nil) {

        self.session = [[FBSession alloc] init];
        [FBSession setActiveSession:self.session];
    }
    return self;
}

-(bool)login {

    if (![EngController isOnline]) {

        NSLog(@"ERROR: Device not connected (line:%d)", __LINE__);
        if (displayError)
            [self.controller alert:@"ERROR: Device not connected!" during:2.5];
        return false;
    }

    //
    FBSession* session = [FBSession activeSession];
    [session accessTokenData];
    if (![session isOpen]) {

        [[NSOperationQueue mainQueue] addOperationWithBlock:^{
            [FBSession openActiveSessionWithPublishPermissions:self.permissions
                                defaultAudience:FBSessionDefaultAudienceOnlyMe allowLoginUI:YES
                                completionHandler:^(FBSession* session, FBSessionState status, NSError* error) {
                if (error) {

                    if ([FBErrorUtility errorCategoryForError:error] == FBErrorCategoryUserCancelled)
                        platformLoadSocial(Network::FACEBOOK, Session::REQUEST_LOGIN, Request::RESULT_CANCELED,
                                            0, 0, NULL);
                    else {

                        NSLog(@"Login error: %@ (line:%d)", [error description], __LINE__);
                        if (displayError)
                            [self.controller alert:@"ERROR: Failed to login!" during:2.5];
                        platformLoadSocial(Network::FACEBOOK, Session::REQUEST_LOGIN, Request::RESULT_FAILED,
                                            0, 0, NULL);
                    }
                }
                else switch (status) {

                    case FBSessionStateOpen: {
                        platformLoadSocial(Network::FACEBOOK, Session::REQUEST_LOGIN, Request::RESULT_SUCCEEDED,
                                            0, 0, NULL);
                        break;
                    }
                    case FBSessionStateCreated:
                    case FBSessionStateCreatedTokenLoaded:
                    case FBSessionStateCreatedOpening:
                    case FBSessionStateOpenTokenExtended:
                    case FBSessionStateClosedLoginFailed:
                    case FBSessionStateClosed:
                        break;
                }
            }];
        }];
    }
    return true;
}
-(void)logout {

    FBSession* session = [FBSession activeSession];
    if ([session isOpen])
        [session closeAndClearTokenInformation];
    
    if (userID != nil) [userID release];
    if (userName != nil) [userName release];
    if (userBirthday != nil) [userBirthday release];
    if (userLocation != nil) [userLocation release];

    userID = nil;
    userName = nil;
    userGender = GENDER_NONE;
    userBirthday = nil;
    userLocation = nil;
}
-(bool)isLogged { return [[FBSession activeSession] isOpen]; }

-(bool)getUserInfo {

    if (![[FBSession activeSession] isOpen]) {

        NSLog(@"ERROR: The session is closed (line:%d)", __LINE__);
        if (displayError)
            [self.controller alert:@"ERROR: Failed to get user info!" during:2.5];
        return false;
    }
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{
        [FBRequestConnection startWithGraphPath:@"/me" parameters:nil HTTPMethod:@"GET"
                              completionHandler:^(FBRequestConnection* connection, id result, NSError* error) {
            if (error) {

                NSLog(@"ERROR (Request info): %@ (line:%d)", [error description], __LINE__);
                if ([FBErrorUtility errorCategoryForError:error] == FBErrorCategoryAuthenticationReopenSession)
                    platformLoadSocial(Network::FACEBOOK, Session::REQUEST_INFO, Request::RESULT_EXPIRED, 0, 0, NULL);
                else {

                    if (displayError)
                        [self.controller alert:@"ERROR: Failed to get user info!" during:2.5];
                    platformLoadSocial(Network::FACEBOOK, Session::REQUEST_INFO, Request::RESULT_FAILED, 0, 0, NULL);
                }
            }
            else {

                // {
                //      birthday = "12/17/1975";
                //      "first_name" = Pascal;
                //      gender = male;
                //      id = 881525691862086;
                //      "last_name" = Dufresne;
                //      link = "https://www.facebook.com/app_scoped_user_id/881525691862086/";
                //      locale = "fr_FR";
                //      location =     {
                //              id = 116217801725646;
                //              name = "Clapiers, Languedoc-Roussillon, France";
                //      };
                //      name = "Pascal Dufresne";
                //      timezone = 2;
                //      "updated_time" = "2014-07-02T15:20:01+0000";
                //      verified = 1;
                // }

                if ((userID == nil) && (result[@"id"]))
                    userID = [[NSString alloc] initWithString:result[@"id"]];
                if ((userID == nil) || ([userID length] == 0)) {

                    NSLog(@"ERROR (Request info): Failed to get user info (line:%d)", __LINE__);
                    if (displayError)
                        [self.controller alert:@"ERROR: Failed to get user info!" during:2.5];
                    platformLoadSocial(Network::FACEBOOK, Session::REQUEST_INFO, Request::RESULT_FAILED,
                                       0, 0, NULL);
                }
                if (result[@"gender"]) {
                    if ([result[@"gender"] isEqualToString:@"male"])
                        userGender = GENDER_MALE;
                    else
                        userGender = GENDER_FEMALE;
                }
                if ((userName == nil) && (result[@"name"]))
                    userName = [[NSString alloc] initWithString:result[@"name"]];
                if ((engReqInfoField(FIELD_BIRTHDAY, Network::FACEBOOK)) && (userBirthday == nil) &&
                    (result[@"birthday"])) {

                    NSDateFormatter* dateFormatter = [[NSDateFormatter alloc] init];
                    dateFormatter.dateFormat = @"MM/dd/yyyy";
                    [dateFormatter setTimeZone:[NSTimeZone timeZoneWithAbbreviation:@"GMT"]];
                    userBirthday = [[NSDate alloc] initWithTimeInterval:0 sinceDate:[dateFormatter
                                                                    dateFromString:result[@"birthday"]]];
                    [dateFormatter release];
                }
                if ((engReqInfoField(FIELD_LOCATION, Network::FACEBOOK)) && (result[@"location"]) &&
                    ([result[@"location"] count] > 0) && (result[@"location"][@"name"]) && (userLocation == nil))
                    userLocation = [[NSString alloc] initWithString:result[@"location"][@"name"]];

                platformLoadSocial(Network::FACEBOOK, Session::REQUEST_INFO, Request::RESULT_SUCCEEDED,
                                   0, 0, NULL);
            }
        }];
    }];
    return true;
}

#define FACEBOOK_PICTURE_SIZE       200

static const NSString* PICTURE_URL_A = @"https://graph.facebook.com/";
static const NSString* PICTURE_URL_B = @"/picture?redirect=1&height=";
static const NSString* PICTURE_URL_C = @"&type=square&width=";

-(bool)getUserPicture {

    if (![[FBSession activeSession] isOpen])
        return false;

    NSString* url = [[NSString alloc] initWithFormat:@"%@%@%@%d%@%d", PICTURE_URL_A, userID, PICTURE_URL_B,
                                                FACEBOOK_PICTURE_SIZE, PICTURE_URL_C, FACEBOOK_PICTURE_SIZE];
    [super getPictureFrom:url];
    return true;
}

#define FACEBOOK_SHARE_NAME         0 // Link/Video name
#define FACEBOOK_SHARE_CAPTION      1 // Link caption (REQUEST_SHARE_LINK) & Video mime type (REQUEST_SHARE_VIDEO)
#define FACEBOOK_SHARE_DESCRIPTION  2 // Link/Video description
#define FACEBOOK_SHARE_LINK         3 // Link URL (REQUEST_SHARE_LINK) & Video file name (REQUEST_SHARE_VIDEO)
#define FACEBOOK_SHARE_PICTURE      4 // Link picture (REQUEST_SHARE_LINK)

-(bool)shareLink:(NSArray*)data {

    if (data == nil) {

        NSLog(@"WARNING: No link data to share (line:%d)", __LINE__);
        if (displayError)
            [self.controller alert:@"ERROR: Failed to share link!" during:2.5];
        return false;
    }

    //
    if (![[FBSession activeSession] isOpen]) {

        NSLog(@"ERROR (Share link): The session is closed (line:%d)", __LINE__);
        if (displayError)
            [self.controller alert:@"ERROR: Failed to share link!" during:2.5];
        return false;
    }
    if (self.shareData != nil)
        [self.shareData release];
    self.shareData = [NSDictionary dictionaryWithObjectsAndKeys:
                                [data objectAtIndex:FACEBOOK_SHARE_NAME], @"name",
                                [data objectAtIndex:FACEBOOK_SHARE_CAPTION], @"caption",
                                [data objectAtIndex:FACEBOOK_SHARE_DESCRIPTION], @"description",
                                [data objectAtIndex:FACEBOOK_SHARE_LINK], @"link",
                                [data objectAtIndex:FACEBOOK_SHARE_PICTURE], @"picture", nil];
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{
        [FBRequestConnection startWithGraphPath:@"/me/feed" parameters:self.shareData HTTPMethod:@"POST"
                          completionHandler:^(FBRequestConnection* connection, id result, NSError* error) {
            if (error) {

                NSLog(@"ERROR (Share link): %@ (line:%d)", error.description, __LINE__);
                if (displayError)
                    [self.controller alert:@"ERROR: Failed to share link!" during:2.5];
                platformLoadSocial(Network::FACEBOOK, Session::REQUEST_SHARE_LINK, Request::RESULT_FAILED, 0, 0, NULL);
            }
            else {

                if (displayError)
                    [self.controller alert:@"INFO: Facebook link shared!" during:2.5];
                platformLoadSocial(Network::FACEBOOK, Session::REQUEST_SHARE_LINK, Request::RESULT_SUCCEEDED, 0, 0, NULL);
            }
        }];
    }];
    return true;
}
-(bool)shareVideo:(NSArray*)data {

    if (data == nil) {
        
        NSLog(@"WARNING: No video data to share (line:%d)", __LINE__);
        if (displayError)
            [self.controller alert:@"ERROR: Failed to share video!" during:2.5];
        return false;
    }
    
    //
    if (![[FBSession activeSession] isOpen]) {
        
        NSLog(@"ERROR (Share video): The session is closed (line:%d)", __LINE__);
        if (displayError)
            [self.controller alert:@"ERROR: Failed to share video!" during:2.5];
        return false;
    }
    if (self.videoData != nil)
        [self.videoData release];
    self.videoData = [NSData dataWithContentsOfFile:[data objectAtIndex:FACEBOOK_SHARE_LINK]];
    NSUInteger fileNameIdx = [[data objectAtIndex:FACEBOOK_SHARE_LINK] rangeOfString:@"/"
                                                                             options:NSBackwardsSearch].location;
    if (self.shareData != nil)
        [self.shareData release];
    self.shareData = [NSDictionary dictionaryWithObjectsAndKeys:
                      self.videoData, [[data objectAtIndex:FACEBOOK_SHARE_LINK] substringFromIndex:fileNameIdx],
                      [data objectAtIndex:FACEBOOK_SHARE_CAPTION], @"contentType",
                      [data objectAtIndex:FACEBOOK_SHARE_NAME], @"name",
                      [data objectAtIndex:FACEBOOK_SHARE_DESCRIPTION], @"description", nil];
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{
        [FBRequestConnection startWithGraphPath:@"/me/videos" parameters:self.shareData HTTPMethod:@"POST"
                              completionHandler:^(FBRequestConnection* connection, id result, NSError* error) {
            if (error) {

                NSLog(@"ERROR (Share video): %@ (line:%d)", error.description, __LINE__);
                if (displayError)
                    [self.controller alert:@"ERROR: Failed to share video!" during:2.5];
                platformLoadSocial(Network::FACEBOOK, Session::REQUEST_SHARE_VIDEO, Request::RESULT_FAILED, 0, 0, NULL);
            }
            else {

                if (displayError)
                    [self.controller alert:@"INFO: Video shared on Facebook!" during:2.5];
                platformLoadSocial(Network::FACEBOOK, Session::REQUEST_SHARE_VIDEO, Request::RESULT_SUCCEEDED, 0, 0, NULL);
            }
        }];
    }];
    return true;
}

+(BOOL)openURL:(NSURL*)url sourceApplication:(NSString*)sourceApplication {
    return [FBAppCall handleOpenURL:url sourceApplication:sourceApplication fallbackHandler:^(FBAppCall* call) { }];
}

-(void)resume {

    [FBAppEvents activateApp];
    [FBAppCall handleDidBecomeActive];
}
-(void)terminate { [FBSession.activeSession close]; }
-(void)remove {

    if (self.session != nil)
        [self.session release];
    [self.permissions release];

    if (self.shareData != nil)
        [self.shareData release];
    if (self.videoData != nil)
        [self.videoData release];

    if (userID != nil) [userID release];
    if (userName != nil) [userName release];
    if (userBirthday != nil) [userBirthday release];
    if (userLocation != nil) [userLocation release];

    [self.controller autorelease];
    [self release];
}

@end //

#endif // LIBENG_ENABLE_SOCIAL
