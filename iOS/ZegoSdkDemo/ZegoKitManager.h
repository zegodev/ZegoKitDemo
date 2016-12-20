//
//  ZegoKitManager.h
//  ZegoSdkDemo
//
//  Created by Strong on 2016/12/15.
//  Copyright © 2016年 Zego. All rights reserved.
//

#import <ZegoKit/AVRoom.h>
#import <Foundation/Foundation.h>

@interface ZegoKitManager : NSObject

+ (instancetype)sharedInstance;

@property (nonatomic, strong, readonly) AVRoom *avRoom;

@end
