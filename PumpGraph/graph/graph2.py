import serial
import numpy as np
from matplotlib import pyplot as plt

def voltageToNewtons(v):
    resistance = (3300 - v) * 10000/ v
    if(resistance==0):
        resistance = 0.001
    conductance = 1000000/resistance  ###fix divide by zero
    if(conductance<1000):
        newton1 = conductance / 80
    else:
        newton1 = conductance / 30
    return newton1

def newtonToWeight(n):
    return n/9.8

def dataToVoltage(d):
    return 1000*d/(1/3.3) 

ser = serial.Serial('/dev/tty.usbmodem1422', 9600)
#ser = serial.Serial('/dev/tty.usbmodem1422', 115200)
 
plt.ion() # set plot to animated 
 

 
 
 
ydata = [0]*50
ydata2 = [0]*50
ydata3 = [0]*50

ax1=plt.axes()  
 
# make plot
line, = plt.plot(ydata,  lw=2)
line2, = plt.plot(ydata2,  lw=2) #1 19 0blue
line3, = plt.plot(ydata3,  lw=2)
plt.ylim([0,1100])
#ser.flushInput() 
# start data collection

ymin= 0
ymax =1200

while True: 

    data = ser.readline().rstrip() # read data from serial, port and strip line endings
    dataArr = data.split("xxx")
    print dataArr
    data=dataArr[0]
    data2=dataArr[1]
    data3=dataArr[2] ##TODO: change back to 2
    
    dataFloat = float(data)*10
    dataFloat2 = float(data2)*10
    dataFloat3 = float(data3)*10
    
    voltage = dataToVoltage(float(data))
    if(voltage==0):
        voltage = 0.0001
    newton1 = voltageToNewtons(voltage)
    print "%d N", newton1
    weight1 = newtonToWeight(newton1)
    print "%d kg", weight1
    
    voltage2 = dataToVoltage(float(data2))
    if(voltage2==0):
        voltage2 = 0.0001
    newton2 = voltageToNewtons(voltage2)
    weight2 = newtonToWeight(newton2)
    
    voltage3 = dataToVoltage(float(data3))
    if(voltage3==0):
        voltage3 = 0.0001
    newton3 = voltageToNewtons(voltage3)
    weight3 = newtonToWeight(newton3)
    
       
    print dataFloat
    if len(data.split(".")) == 2:
        #ymin = float(min(ydata))-10
        #all = []
        #all.extend(ydata)
        #all.extend(ydata2)
        #all.extend(ydata3)
        # ymax = float(max(ydata))+10
        #ymax = float(max(all))+100
        
        plt.ylim([ymin,ymax])
        ydata.append(str(weight1*1000))## append and convert to grams
        ydata2.append(str(weight2*1000))
        ydata3.append(str(weight3*1000))
        del ydata[0]
        del ydata2[0]
        del ydata3[0]
        line.set_xdata(np.arange(len(ydata)))
        line.set_ydata(ydata)  # update the data
        line2.set_ydata(ydata2)
        line3.set_ydata(ydata3)
        plt.draw() # update the plot
        plt.pause(0.2);
        ser.flushInput()