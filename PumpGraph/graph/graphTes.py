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

#ser = serial.Serial('/dev/tty.usbmodem1422', 9600)
#ser = serial.Serial('/dev/tty.usbmodem1422', 115200)
 
 
# These are the "Tableau 20" colors as RGB.    
tableau20 = [(31, 119, 180), (174, 199, 232), (255, 127, 14), (255, 187, 120),    
             (44, 160, 44), (152, 223, 138), (214, 39, 40), (255, 152, 150),    
             (148, 103, 189), (197, 176, 213), (140, 86, 75), (196, 156, 148),    
             (227, 119, 194), (247, 182, 210), (127, 127, 127), (199, 199, 199),    
             (188, 189, 34), (219, 219, 141), (23, 190, 207), (158, 218, 229)]    
  
# Scale the RGB values to the [0, 1] range, which is the format matplotlib accepts.    
for i in range(len(tableau20)):    
    r, g, b = tableau20[i]    
    tableau20[i] = (r / 255., g / 255., b / 255.)    
   
hfont = {'fontname':'Helvetica'}  
plt.ion() # set plot to animated 
fig = plt.figure(figsize=(10, 7.5))  
plt.ylabel("Weight (g)", fontsize=14, **hfont)
plt.xlabel("Time", fontsize=14, **hfont)
title = plt.title("Force applied in grams", fontsize = 18, y=1.02, **hfont)
title.set_weight('bold')

ax = plt.subplot(111)    
ax.spines["top"].set_visible(False)    
ax.spines["bottom"].set_visible(False)    
ax.spines["right"].set_visible(False)    
ax.spines["left"].set_visible(False)    

ax.get_yaxis().tick_left()  

for y in range(0, 1100, 100):    
    plt.plot(range(0, 50), [y] * len(range(0, 50)), "--", lw=0.5, color="black", alpha=0.3)    

plt.tick_params(axis="both", which="both", bottom="off", top="off",    
                labelbottom="off", left="off", right="off", labelleft="on")  
 
ydata = [110]*50
ydata2 = [210]*50
ydata3 = [310]*50


Fs=8000
f=500
for n in range(50):
    ydata[n]=np.sin(2*np.pi*f*n/Fs)*700
    ydata2[n]=np.sin(2*np.pi*f*n/Fs)*800+20
    ydata3[n]=np.sin(2*np.pi*(f-10)*n/Fs+10)*750-25
    
#plt.axes()  
 
# make plot
line, = plt.plot(ydata, color=tableau20[18], lw=2)
line2, = plt.plot(ydata2, color = tableau20[0], lw=2) #1 19 0blue
line3, = plt.plot(ydata3, color= tableau20[8], lw=2)
plt.ylim([0,1100])
#ser.flushInput() 
# start data collection
plt.draw() # update the plot

while True: 
    ymin= 0
    ymax =1200


