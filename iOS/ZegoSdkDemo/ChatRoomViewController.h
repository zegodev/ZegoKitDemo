
#import <UIKit/UIKit.h>
#import "NotificationEntity.h"
#import <ZegoKit/AVRoomCallback.h>

@interface ChatRoomViewController : UIViewController<AVRoomCallback, UITextFieldDelegate>

@property (retain,nonatomic) NotificationEntity *mNotifEntity;

- (IBAction)backgroundTap:(id)sender;

@end
