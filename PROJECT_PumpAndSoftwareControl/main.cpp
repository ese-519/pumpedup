#include "mbed.h"
#include "millis.h"

#define BETA 0.95
#include "Arduino.h"
#include "BLEPeripheral.h"


Serial serial(USBTX, USBRX);

PwmOut pump(p24);

DigitalIn v1(p30);
DigitalIn v2(p29);
DigitalIn v3(p28);

DigitalOut v4(p27);

DigitalOut valve1SoftwareControl(p11);
DigitalOut valve2SoftwareControl(p12);
DigitalOut valve3SoftwareControl(p13);
DigitalOut myled(p16);
DigitalOut myled2(p17);
AnalogIn potDCPump(p20);

AnalogIn fsr1(p18);
AnalogIn fsr2(p15);
AnalogIn fsr3(p19);

DigitalIn pumpOn(p14);

//////App control
SPI spi(p5, p6, p7);//mosi, miso, sclk   
DigitalInOut BLE_RDY(p8);
DigitalInOut BLE_REQ(p9);
DigitalInOut BLE_RST(p10);

unsigned char buf[16] = {0};
unsigned char len = 0;

unsigned char analog_enabled = 1;
unsigned char old_state = LOW;

float waitTime = 0.1;


/*----- BLE Utility -------------------------------------------------------------------------*/
// create peripheral instance, see pinouts above
BLEPeripheral            blePeripheral        = BLEPeripheral(&BLE_REQ, &BLE_RDY, &BLE_RST);

// create service
BLEService               uartService          = BLEService("713d0000503e4c75ba943148f18d941e");

// create characteristic
BLECharacteristic    txCharacteristic = BLECharacteristic("713d0002503e4c75ba943148f18d941e", BLENotify, 20);
BLECharacteristic    rxCharacteristic = BLECharacteristic("713d0003503e4c75ba943148f18d941e", BLEWriteWithoutResponse, 20);
/*--------------------------------------------------------------------------------------------*/
 
/////////////////

//Variables to store the Potentiometer reading
float pumpDutyControl = 0.0f;

//initializing PID control variable "error" to 0
float presError = 0.0f;

//initializing previous error
float lastError = 0.0f;

//initializing current and previous ticker values 
float now = 0.0f;
float lastTime = 0.0f;

//initializing cumulative error and error difference;
float errorSum = 0.0f;
float errorDiff= 0.0f;

float pumpDuty = 0.0f;
float lastPressure = 0.0f;

//initializing the required pressure sensor value 
float presRequired = 0.0f;
//setting period of the PWMs for the valves

int periodOfPumpMicro = 111;//period of pump in micro seconds (this should be ˜9Khz)
void PWMConfigPump();
void reset();

float force = 0;
int pwmSwitch = 0;


/*#define BETA 0.92
float angle=0;
while(1){
   angle = BETA*angle + (1-BETA)*ADC;
*/


int main() {
    //serial.baud(9600);
    //serial.printf("Serial begin!\r\n");
    pump.period_us(periodOfPumpMicro);//set the period for the pump
    startMillis(); 
    unsigned int counter = 0; 
    presRequired = 0.75f;
    
    
     /*----- BLE Utility ---------------------------------------------*/
    // set advertised local name and service UUID
    blePeripheral.setLocalName("BLE Shield");
    
    blePeripheral.setAdvertisedServiceUuid(uartService.uuid());
    
    // add service and characteristic
    blePeripheral.addAttribute(uartService);
    blePeripheral.addAttribute(rxCharacteristic);
    blePeripheral.addAttribute(txCharacteristic);
    
    // begin initialization
    blePeripheral.begin();
    /*---------------------------------------------------------------*/
    //serial.printf("BLE UART Peripheral begin!\r\n"); 
    while(1){
        

        //sensor readings
       
        //pc. printf("flex %f\n",flex.read()); 
        //pc. printf("\n"); 
        
        //  angle = BETA*angle + (1-BETA)*ADC;
        force = BETA*force + (1-BETA)*fsr1.read();
        serial.printf("%fxxx%fxxx%f\n\r",fsr1.read(),fsr2.read(), fsr3.read()); 
         
        BLECentral central = blePeripheral.central();
        if (central) 
        {
            // central connected to peripheral
            //serial.printf("Connected to central\r\n");
            while (central.connected()) 
            {   
                serial.printf("%fxxx%fxxx%f\n\r",fsr1.read(),fsr2.read(), fsr3.read()); 
               
                // central still connected to peripheral
                if (rxCharacteristic.written()) 
                {
                    unsigned char len = rxCharacteristic.valueLength();
                    const unsigned char *val = rxCharacteristic.value();
                    //serial.printf("didCharacteristicWritten, Length: %d\r\n", len); 
                
                    unsigned char i = 0;
                    while(i<len)
                    {
                        unsigned char data0 = val[i++];
                        unsigned char data1 = val[i++];
                        unsigned char data2 = val[i++]; 
                        //serial.printf("D0 is %d\t", data0);

                        if (data0 == 0x01)  // Command is to control digital out pin
                        {
                            if (data1 == 0x01){
                                myled = 1;
                                pwmSwitch = 1;
                                }
                                //digitalWrite(&DIGITAL_OUT_PIN, HIGH);
                            else
                            {
                                myled = 0;
                                pwmSwitch = 0; 
                                    
                            }
                                //digitalWrite(&DIGITAL_OUT_PIN, LOW);
                        } //end 0x01
                        
                        else if (data0 == 0x02) // Command is to control Pressure Required
                        {
                            if(pwmSwitch == 1){
                                 float percent = (0.40f + (data1 - 0) * (0.75f - 0.40f) / (255 - 0));
                                 //float percent = (1.0 / 255) * data1; // Convert 0~255 to 0%~100%
                                 presRequired = percent;
                                 //serial.printf("d1 is %d\t", data1);
                            }                                 
                        }// end 0x02
                        
                        else if (data0 == 0x04)//RESET
                        {
                           reset();
                           
                            //digitalWrite(&DIGITAL_OUT_PIN, LOW);
                        }// end 0x04
                        
                            else if (data0 == 0x05) // Command is to control PWM2 pin
                        {
                            if (data1 == 0x01)
                                {valve1SoftwareControl.write(1);
                                }
                            else{
                                valve1SoftwareControl.write(0);
                                }
                        }// end 0x05
                          
                          else if (data0 == 0x06) // Command is to control PWM2 pin
                        {
                             if (data1 == 0x01)
                                valve2SoftwareControl.write(1);
                            else{
                                valve2SoftwareControl.write(0);
                                }
                        }// end 0x06
                        
                        else if (data0 == 0x07) // Command is to control PWM2 pin
                        {
                             if (data1 == 0x01){
                                valve3SoftwareControl.write(1);
                                }
                            else{
                                valve3SoftwareControl.write(0);
                                }
                        }
                    } 
                }

                // Force sensors - read and send out //TODO serial as well!
                
                unsigned short value1 = fsr1.read()*255;       
                unsigned char val[3] = {0x0A, value1, 0x00};                
                
                unsigned short value2 = fsr2.read()*255;       
                unsigned char val2[3] = {0x0B, value2, 0x00};
                
                unsigned short value3 = fsr3.read()*255;       
                unsigned char val3[3] = {0x0C, value3, 0x00};
                
                if(counter%6 < 2)
                {
                    txCharacteristic.setValue(val, 3);
                }
                
                else if(counter%6 < 4)
                {
                    txCharacteristic.setValue(val2, 3);
                }
                
                else if(counter%6 < 6)
                {
                    txCharacteristic.setValue(val3, 3);
                }
                
                counter = counter +1;
                
                wait(waitTime);
                if(!v1.read() && !v2.read() && !v3.read()){
                v4.write(1);
                }
                else{
                    v4.write(0);
                    PWMConfigPump();  
                }
                
               
            }// central.connected ends
            
            
            //serial.printf("Disconnected from central\r\n");
        }// if(central) ends 
   
      reset();
        wait(waitTime);
        if(!v1.read() && !v2.read() && !v3.read()){
            v4.write(1);
            }
        else{
            v4.write(0);
             //pumpDutyControl = potDCPump.read();
             //wait(0.1);
             PWMConfigPump();  
            }
        }
    
}

void PWMConfigPump(){
     float kp = 2.0f;
     float kd = 0.00f;
     float ki = 0.8f;
     float deltaT = 0.005; //5 milli seconds interns of seconds
     now = millis();
     int timeChange = (now - lastTime);
     //pc.printf("dutycycle = %f \n\r",pump.read());
     if(pumpOn.read() && pwmSwitch == 1){
         if(timeChange >=deltaT*100){//checking time difference from previous sample (in millis)
              presError = presRequired - fsr1.read(); //error = required pressure - current reading
              errorSum = errorSum + presError;                //cumulative error
              errorDiff = fsr1.read() - lastPressure;         //change in error from last time
              
              pumpDuty = pumpDuty + 0.8*(kp*presError + (ki*deltaT*errorSum) + (kd*errorDiff/deltaT));  //PID control 
              if(pumpDuty < 0){
                  pumpDuty =0;
                  }
             else if (pumpDuty > 1){
                pumpDuty = 1;
              }
              lastError = presError;
              lastTime = now;
              lastPressure = fsr1.read();
              
              pump.write(pumpDuty);// capping the duty cycle at 96%
              
              
              
         }  
     }  
     else{
         pump.write(0.0f);// pump is off
     }
        
}

void reset(){
        valve1SoftwareControl.write(0);
        valve2SoftwareControl.write(0);
        valve3SoftwareControl.write(0);
        presRequired = 0.75f;
        pwmSwitch = 1;
        myled = 1;
        
    }
    
    