
#import <Foundation/Foundation.h>
#import <ZegoKit/RoomUser.h>

@interface AVRoomTextMsg : NSObject

@property (nonatomic,retain) RoomUser* oRoomUser;

@property (nonatomic) time_t sendTime;

@property (nonatomic,retain) NSString* strMsg;


- (instancetype)init;

@end
