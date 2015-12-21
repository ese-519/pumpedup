#include "mbed.h"

PwmOut valve1(p21);
PwmOut valve2(p22);
PwmOut valve3(p23);
//PwmOut valve4(p24); 

AnalogIn potDC1(p15);
AnalogIn potDC2(p16);
AnalogIn potDC3(p19); 

DigitalIn valve1On(p5);
DigitalIn valve2On(p6);
DigitalIn valve3On(p7);

DigitalIn valve1SoftwareControl(p10);
DigitalIn valve2SoftwareControl(p11);
DigitalIn valve3SoftwareControl(p12);

Serial pc(USBTX, USBRX);
//Variables to store the Potentiometer reading
float valve1DutyControl = 0.0f;
float valve2DutyControl = 0.0f;
float valve3DutyControl = 0.0f;
//setting period of the PWMs for the valves
float periodOfValves = 6.0f;

void PWMConfig1();
void PWMConfig2();
void PWMConfig3();
    
 int v1on = false;
 int v2on = false;
 int v3on = false;

int main() {
    //Setup valve periods
    valve1.period(periodOfValves);
    valve2.period(periodOfValves);
    valve3.period(periodOfValves);//All PWM channels share the same period anyway
        
    while(1){
        
    
        PWMConfig1();
        PWMConfig2();
        PWMConfig3();
        
             
        }
}
void PWMConfig1(){
     if(valve1On.read()){
        if(valve1SoftwareControl.read())
            valve1.write(0.80f);
        else
            valve1.write(0.30f);
        
        }
                 
    else{
        valve1.write(0.0f);
        
        }
        
}
void PWMConfig2(){
    if(valve2On.read()){
         if(valve2SoftwareControl.read())
            valve2.write(0.8f);
        else
            valve2.write(0.30f);
        
        }
        
    else
        valve2.write(0.0f);
                       
}

void PWMConfig3(){
     if(valve3On.read()){
        if(valve3SoftwareControl.read())
            valve3.write(0.8f);
        else
            valve3.write(0.30f);
       
          }
       
    else{
        valve3.write(0.0f);
        }
}
