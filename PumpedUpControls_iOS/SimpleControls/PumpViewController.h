//
//  PumpViewController.h
//  PumpedUpControls
//
//  Created by Adriana Vazquez on 12/7/15.
//  Copyright © 2015 RedBearLab. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BLE.h"

@interface PumpViewController : UIViewController <BLEDelegate>
{

    IBOutlet UISwitch *switchA;
    
    IBOutlet UISwitch *switchB;
    
    IBOutlet UISwitch *switchC;
    IBOutlet UISlider *sldPWMPump;

    IBOutlet UISwitch *swPump;

    IBOutlet UIImageView *offImg;
    IBOutlet UIImageView *imageA;
    IBOutlet UIImageView *imageB;
    IBOutlet UIImageView *imageC;
    
}

@property (strong, nonatomic) BLE *ble;
@end
