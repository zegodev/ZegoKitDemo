//
//  ZegoKitManager.m
//  ZegoSdkDemo
//
//  Created by Strong on 2016/12/15.
//  Copyright © 2016年 Zego. All rights reserved.
//

#import "ZegoKitManager.h"

@implementation ZegoKitManager

+ (instancetype)sharedInstance
{
    static ZegoKitManager *gInstance = nil;
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        gInstance = [[ZegoKitManager alloc] init];
    });
    
    return gInstance;
}

- (instancetype)init
{
    self = [super init];
    if (self)
    {
        _avRoom = [[AVRoom alloc] init];
        
        Byte signkey[] = {0x91,0x93,0xcc,0x66,0x2a,0x1c,0xe,0xc1,
            0x35,0xec,0x71,0xfb,0x7,0x19,0x4b,0x38,
            0x15,0xf1,0x43,0xf5,0x7c,0xd2,0xb5,0x9a,
            0xe3,0xdd,0xdb,0xe0,0xf1,0x74,0x36,0xd};
        
        NSData *nsDataAppSignature = [[NSData alloc] initWithBytes:signkey length:32];
        
        [self.avRoom SetLogLevel:AVROOM_LOG_LEVEL_DEBUG];
        [self.avRoom Init:1 AppSignature:nsDataAppSignature];
    }
    
    return self;
}

@end
