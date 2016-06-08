
#import <Foundation/Foundation.h>
#import <ZegoKit/RoomUser.h>

@interface AVRoomCustomMsg : NSObject

@property (nonatomic,retain) RoomUser* oRoomUser;

@property (nonatomic) time_t sendTime;

@property (nonatomic,retain) NSData* szMsg;

@end
