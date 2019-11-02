# script for generating voltage points for scl simulation with mplabx
#
import numpy as np
import matplotlib.pyplot as plt

# sample frequency
fs = 2000
# set frequency
f = 82.4
# set fundamental frequency amplitude in mili volts
A = 300
# set some sample size
samples = 512
# amplifyer circuit voltage bias in mV
vbias = 1800
# time scale
t = np.arange(0,samples/fs,1.0/fs) #entire chunk of signal
#t = np.arange(0,1/f,1.0/fs) #one period of signal

# fundemental frequency
fundamental = A*np.sin(2 * np.pi * f * t) 
# add some harmonics
second_harmonic = 3*A*np.sin(2 * np.pi *2* f * t) 
third_harmonic = 2*A*np.sin(2 * np.pi *3* f * t) 

# signal y(t) is the sum of its harmonics
yt = (fundamental + second_harmonic + third_harmonic) + vbias

maxi = np.argmax(yt)
mini = np.argmin(yt)
if yt[mini] < 0:
    print("signal below 0 V")
    
mid = (yt[maxi] + yt[mini])/2 
pk_pk = yt[maxi] - yt[mini]

print ("mV peak",yt[maxi],"peak to peak",pk_pk ,"offset", np.uint16(mid))

# plot the signal
plt.plot(t,yt)
plt.axhline(mid,color='orange')
plt.axhline(0,color='black')
plt.show()

# open and overwrite file
file = open("../guitar_tuner.X/datapoints.txt","w")

for i in range(len(yt)):
    file.write('{0} {1}\n'.format(np.uint16(yt[i])," mV"))
file.close()
