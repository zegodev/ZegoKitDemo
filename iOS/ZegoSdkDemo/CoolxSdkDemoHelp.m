
#import "CoolxSdkDemoHelp.h"
#import "MyToast.h"

static int nRoomkeyBase = 900000;

@implementation CoolxSdkDemoHelp

+(int)coolxSdkDemoRoomKeyBase
{
    return nRoomkeyBase;
}

+ (void) ShowToast:(NSString*)strMsg
{
    if ([NSThread isMainThread])
    {
        [MYToast showWithText:strMsg bottomOffset:100 duration:1];
    }
    else
    {
        dispatch_async(dispatch_get_main_queue(), ^{
            [MYToast showWithText:strMsg bottomOffset:100 duration:1];
        });
    }
}

@end
