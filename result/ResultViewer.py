# -*- coding: utf-8 -*-
"""
Created on Tue Jan 28 13:04:28 2020

@author: chris
"""
import array as arr
import matplotlib.pyplot as plt
import numpy as np
from plotter import plotterrainbow
from plotter import plottergray
from plotter import plotterviridis
from plotter import plotterjet
plt.close('all')

#%% Zipped Sorting function
def sorting(sorter,sortdata):
    c = [x for _,x in sorted(zip(sorter,sortdata))]
    
    return c

#%% Opening the result file

file = open("result.txt","r")
readdata = file.readlines()
file.close()

h = float(readdata[0])

x = arr.array('d',[])
y = arr.array('d',[])
vonmises = arr.array('d',[])
sigmax = arr.array('d',[])
sigmay = arr.array('d',[])
sigmaxy = arr.array('d',[])
Ux = arr.array('d',[])
Uy = arr.array('d',[])

deleter = []
for i in range(1,len(readdata)):
    if readdata[i] != 'nan' + '\n':
        temp = readdata[i].split("\t")
        x.append(float(temp[0]))
        y.append(float(temp[1]))
        vonmises.append(float(temp[2]))
        sigmax.append(float(temp[3]))
        sigmay.append(float(temp[4]))
        sigmaxy.append(float(temp[5]))
        Ux.append(float(temp[6]))
        Uy.append(float(temp[7]))
    else:
        deleter.append(i-1)

del readdata,temp
maxnumx = (max(x) - min(x))/h
maxnumy = (max(y) - min(y))/h
maxnum = max([maxnumx,maxnumy])

#%% Color Scheme Setting

scattersize = 100**(2)/(maxnum)**(2) + 1
# The sizing of each node in the scatter plot

dispscale = 100
# The displacement scale for plotting

contourscheme = 3
# 1 for rainbow color scheme
# 2 for viridis color scheme
# 3 for jet color scheme
# 4 for gray color scheme

convertunits = 1
# 1 to keep current units
# 2 to convert unit from mm to m

#%% Calculating Final Position and Plotting the result

Xnew = arr.array('d',[])
Ynew = arr.array('d',[])

for i in range(len(x)):
    Xnew.append(x[i] + Ux[i]*dispscale)
    Ynew.append(y[i] + Uy[i]*dispscale)
    
if convertunits == 2:
    # Converting to SI units (m and Pa)
    for i in range(len(Xnew)):
        Xnew[i] = Xnew[i]/1000
        Ynew[i] = Ynew[i]/1000
        sigmax[i] = sigmax[i]*1000
        sigmay[i] = sigmay[i]*1000
        sigmaxy[i] = sigmaxy[i]*1000
        vonmises[i] = vonmises[i]*1000
        Ux[i] = Ux[i]/1000
        Uy[i] = Uy[i]/1000
    for i in range(len(x)):
        x[i] = x[i]/1000
        y[i] = y[i]/1000

if contourscheme == 1:
    plotterrainbow(Xnew,Ynew,vonmises,scattersize,r'$\sigma_{VonMises},\delta_{Scale}:$'+f'{dispscale}')
    plotterrainbow(Xnew,Ynew,sigmax,scattersize,r'$\sigma_{xx},\delta_{Scale}:$'+f'{dispscale}')
    plotterrainbow(Xnew,Ynew,sigmay,scattersize,r'$\sigma_{yy},\delta_{Scale}:$'+f'{dispscale}')
    plotterrainbow(Xnew,Ynew,sigmaxy,scattersize,r'$\sigma_{xy},\delta_{Scale}:$'+f'{dispscale}')
elif contourscheme == 2:
    plotterviridis(Xnew,Ynew,vonmises,scattersize,r'$\sigma_{VonMises},\delta_{Scale}:$'+f'{dispscale}')
    plotterviridis(Xnew,Ynew,sigmax,scattersize,r'$\sigma_{xx},\delta_{Scale}:$'+f'{dispscale}')
    plotterviridis(Xnew,Ynew,sigmay,scattersize,r'$\sigma_{yy},\delta_{Scale}:$'+f'{dispscale}')
    plotterviridis(Xnew,Ynew,sigmaxy,scattersize,r'$\sigma_{xy},\delta_{Scale}:$'+f'{dispscale}')
elif contourscheme == 3:
    plotterjet(Xnew,Ynew,vonmises,scattersize,r'$\sigma_{VonMises},\delta_{Scale}:$'+f'{dispscale}')
    plotterjet(Xnew,Ynew,sigmax,scattersize,r'$\sigma_{xx},\delta_{Scale}:$'+f'{dispscale}')
    plotterjet(Xnew,Ynew,sigmay,scattersize,r'$\sigma_{yy},\delta_{Scale}:$'+f'{dispscale}')
    plotterjet(Xnew,Ynew,sigmaxy,scattersize,r'$\sigma_{xy},\delta_{Scale}:$'+f'{dispscale}')
    plotterjet(Xnew,Ynew,Ux,scattersize,r'$U_{x},\delta_{Scale}:$'+f'{dispscale}')
    plotterjet(Xnew,Ynew,Uy,scattersize,r'$U_{y},\delta_{Scale}:$'+f'{dispscale}')
elif contourscheme == 4:
    plottergray(Xnew,Ynew,vonmises,scattersize,r'$\sigma_{VonMises},\delta_{Scale}:$'+f'{dispscale}')
    plottergray(Xnew,Ynew,sigmax,scattersize,r'$\sigma_{xx},\delta_{Scale}:$'+f'{dispscale}')
    plottergray(Xnew,Ynew,sigmay,scattersize,r'$\sigma_{yy},\delta_{Scale}:$'+f'{dispscale}')
    plottergray(Xnew,Ynew,sigmaxy,scattersize,r'$\sigma_{xy},\delta_{Scale}:$'+f'{dispscale}')

print(f"Max Positive x displacement: {max(Ux)}")
print(f"Max Negative x displacement: {min(Ux)}")
print(f"Max Positive Y displacement: {max(Uy)}")
print(f"Max Negative Y displacement: {min(Uy)}")


