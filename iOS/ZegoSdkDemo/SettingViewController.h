//
//  SettingViewController.h
//  ZegoSdkDemo
//
//  Created by jiangningb on 15/9/28.
//  Copyright (c) 2015年 Zego. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ViewController.h"

@interface SettingViewController : UIViewController<UIPickerViewDelegate, UIPickerViewDataSource>

- (void)InitParam:(int)nEnv IP:(NSString*)strIP Port:(NSString*)strPort;

@property (strong,nonatomic) ViewController *mainViewController;

@end
