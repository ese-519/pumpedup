//
//  PumpViewController.m
//  PumpedUpControls
//
//  Created by Adriana Vazquez on 12/7/15.
//  Copyright © 2015 RedBearLab. All rights reserved.
//

#import "PumpViewController.h"

@interface PumpViewController ()

@end
@implementation PumpViewController

NSArray *imagesA;
NSArray *imagesB;
NSArray *imagesC;

const int OFF_THR = 7;
const int LOW_THR = 85;
const int MED_THR = LOW_THR*2;
const int HIGH_THR = LOW_THR*3;



- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    imagesA = [NSArray arrayWithObjects: @"off.png", @"A_1.png", @"A_2.png", @"A_3.png", nil];

    imagesB = [NSArray arrayWithObjects: @"off.png", @"B_1.png", @"B_2.png", @"B_3.png", nil];
    
    imagesC = [NSArray arrayWithObjects: @"off.png", @"C_1.png", @"C_2.png", @"C_3.png", nil];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/


- (IBAction)toggleA:(id)sender {
    UInt8 buf[3] = {0x05, 0x00, 0x00};
    if (switchA.on)
        buf[1] = 0x01;
    else
        buf[1] = 0x00;
    
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [_ble write:data];
 }


- (IBAction)toggleB:(id)sender {
    UInt8 buf[3] = {0x06, 0x00, 0x00};
    if (switchB.on)
        buf[1] = 0x01;
    else
        buf[1] = 0x00;
    
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [_ble write:data];
}

- (IBAction)toggleC:(id)sender {
    UInt8 buf[3] = {0x07, 0x00, 0x00};
    if (switchC.on)
        buf[1] = 0x01;
    else
        buf[1] = 0x00;
    
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [_ble write:data];
}


- (IBAction)toggleD:(id)sender {
    UInt8 buf[3] = {0x01, 0x00, 0x00};
    if (swPump.on){
        buf[1] = 0x01;
        sldPWMPump.enabled=TRUE;
    }
    else{
        buf[1] = 0x00;
        sldPWMPump.enabled=FALSE;
    }
    
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [_ble write:data];
}

- (IBAction)slidePump:(id)sender {
    
    UInt8 buf[3] = {0x02, 0x00, 0x00};
    
    NSLog(@"val %d", (int)sldPWMPump.value);
    
    buf[1] = (int)sldPWMPump.value;
    buf[2] = (int)sldPWMPump.value >> 8;
    
    NSData *data = [[NSData alloc] initWithBytes:buf length:3];
    [_ble write:data];
}


-(void) updateImageAWithValue: (UInt16) value{
    if(value <OFF_THR){
        [imageA setImage:[UIImage imageNamed:@"off.png"]];
    }
    else if(value<LOW_THR){
        [imageA setImage:[UIImage imageNamed:@"A_1.png"]];
    }
    else if(value < MED_THR){
        [imageA setImage:[UIImage imageNamed:@"A_2.png"]];
    }
    else {
        [imageA setImage:[UIImage imageNamed:@"A_3.png"]];
    }
    
}


-(void) updateImageBWithValue: (UInt16) value{
    if(value <OFF_THR){
        [imageB setImage:[UIImage imageNamed:@"off.png"]];
    }
    else if(value<LOW_THR){
        [imageB setImage:[UIImage imageNamed:@"B_1.png"]];
    }
    else if(value < MED_THR){
        [imageB setImage:[UIImage imageNamed:@"B_2.png"]];
    }
    else {
        [imageB setImage:[UIImage imageNamed:@"B_3.png"]];
    }

}


-(void) updateImageCWithValue: (UInt16) value{
    if(value <OFF_THR){
        [imageC setImage:[UIImage imageNamed:@"off.png"]];
    }
    else if(value<LOW_THR){
        [imageC setImage:[UIImage imageNamed:@"C_1.png"]];
    }
    else if(value < MED_THR){
        [imageC setImage:[UIImage imageNamed:@"C_2.png"]];
    }
    else {
        [imageC setImage:[UIImage imageNamed:@"C_3.png"]];
    }
    
}


// When data is coming, this will be called
-(void) bleDidReceiveData:(unsigned char *)data length:(int)length
{
    NSLog(@"Length: %d", length);
    
    // parse data, all commands are in 3-byte
    for (int i = 0; i < length; i+=3)
    {
        //NSLog(@"0x%02X, 0x%02X, 0x%02X", data[i], data[i+1], data[i+2]);
        
      
        if (data[i] == 0x0A)
        {
            UInt16 value;
            value = data[i+1];
            //NSLog([NSString stringWithFormat:@"A %d", value]);
            
                [self updateImageAWithValue:value];
        }
        
        else if (data[i] == 0x0B)
        {
            UInt16 value;
            value = data[i+1];
            //NSLog([NSString stringWithFormat:@"B %d", value]);
            
            [self updateImageBWithValue:value];
        }
        
        else if (data[i] == 0x0C)
        {
            UInt16 value;
            value = data[i+1];
            //NSLog([NSString stringWithFormat:@"C %d", value]);
            
                [self updateImageCWithValue:value];
        }
    }
}

#pragma mark - BLE delegate
// When RSSI is changed, this will be called
-(void) bleDidUpdateRSSI:(NSNumber *) rssi
{
    //lblRSSI.text = rssi.stringValue;
}

-(void) readRSSITimer:(NSTimer *)timer
{
    [_ble readRSSI];
}

@end
