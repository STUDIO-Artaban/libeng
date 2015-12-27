//
//  EngSocial.h
//
//  Created by Pascal Viguié on 15/08/2014.
//  Copyright (c) 2014 Pascal Viguié. All rights reserved.
//

#include "Main.h"

#ifdef LIBENG_ENABLE_SOCIAL
#import <Foundation/Foundation.h>
#import <libeng/Social/Session.h>


@interface EngSocial : NSObject {

    Network::ID networkID;
}
-(id)initWith:(Network::ID)network;
-(void)getPictureFrom:(NSString*)url;

@end //

#endif // LIBENG_ENABLE_SOCIAL
