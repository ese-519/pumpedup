//
//  ConnectViewController.h
//  PumpedUpControls
//
//  Created by Adriana Vazquez on 12/7/15.
//  Copyright © 2015 RedBearLab. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BLE.h"

@interface ConnectViewController : UIViewController<BLEDelegate>
{

    IBOutlet UIButton *btnConnect;
    IBOutlet UIActivityIndicatorView *indConnecting;
}

@property (strong, nonatomic) BLE *ble;

@end
