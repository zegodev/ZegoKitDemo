
#import <UIKit/UIKit.h>

@interface ViewController : UIViewController<UIPickerViewDelegate, UIPickerViewDataSource>

- (IBAction)backgroundTap:(id)sender;
- (void)SetEnv:(int)nEnv IP:(NSString*)strIP Port:(NSString*)strPort;

@end

