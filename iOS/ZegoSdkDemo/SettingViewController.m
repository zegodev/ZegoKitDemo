//
//  SettingViewController.m
//  ZegoSdkDemo
//
//  Created by jiangningb on 15/9/28.
//  Copyright (c) 2015年 Zego. All rights reserved.
//

#import "SettingViewController.h"

@interface SettingViewController ()
{
    NSArray *naEnv;
    int mNEnv;
    NSString *mStrTestIP;
    NSString *mStrTestPort;
}
@property (weak, nonatomic) IBOutlet UIPickerView *pickAVEnv;
@property (weak, nonatomic) IBOutlet UILabel *labelIP;
@property (weak, nonatomic) IBOutlet UILabel *labelPort;
@property (weak, nonatomic) IBOutlet UITextField *textIP;
@property (weak, nonatomic) IBOutlet UITextField *textPort;

@end

@implementation SettingViewController

@synthesize mainViewController;

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    naEnv = [NSArray arrayWithObjects:@"正式环境",@"开发环境",nil];
    _pickAVEnv.delegate = self;
    _pickAVEnv.dataSource = self;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    self.navigationController.navigationBar.hidden = NO;
    if (mNEnv == 0)
    {
        [_pickAVEnv selectRow:0 inComponent:0 animated:true];
        [self ShowTestAddrUI:false];
    }
    else
    {
        [_pickAVEnv selectRow:1 inComponent:0 animated:true];
        [self ShowTestAddrUI:true];
        _textIP.text = mStrTestIP;
        _textPort.text = mStrTestPort;
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView{
    return 1;
}

-(NSInteger) pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component{
    return [naEnv count];
}

-(NSString*) pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component{
    return [naEnv objectAtIndex:row];
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component{
    if (row == 1)
    {
        [self ShowTestAddrUI:true];
    }
    else
    {
        [self ShowTestAddrUI:false];
    }
}

-(void)ShowTestAddrUI:(BOOL)bEnable
{
    _labelIP.hidden = !bEnable;
    _labelPort.hidden = !bEnable;
    _textIP.hidden = !bEnable;
    _textPort.hidden = !bEnable;
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    if ([mainViewController respondsToSelector:@selector(SetEnv:IP:Port:)])
    {
        NSLog(@"viewWillDisappear");
        mNEnv = (int)[_pickAVEnv selectedRowInComponent:0];
        if (mNEnv == 1)
        {
            mStrTestIP = _textIP.text;
            mStrTestPort = _textPort.text;
        }
        [mainViewController SetEnv:mNEnv IP:mStrTestIP Port:mStrTestPort];
    }
}

- (void)InitParam:(int)nEnv IP:(NSString*)strIP Port:(NSString*)strPort
{
    mNEnv = nEnv;
    if (nEnv == 1)
    {
        mStrTestIP = strIP;
        mStrTestPort = strPort;
    }
}

- (IBAction)backgroundTap:(id)sender {
    [self.textIP resignFirstResponder];
    [self.textPort resignFirstResponder];
}
/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
