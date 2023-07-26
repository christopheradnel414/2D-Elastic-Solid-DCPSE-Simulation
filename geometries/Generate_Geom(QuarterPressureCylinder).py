#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jan  8 20:57:46 2020

@author: admin
"""

from plotter import boundaryplot
import numpy as np

E = 125e6
v = 0.33
rho = 70
go = 0.0
innerradius = 200
outerradius = 300
a = outerradius - innerradius
Nx = 100
Ny = 100
h = a/(Nx-1)
Pin = -50e3
Pout = -0.1e3

xmid = 0.0
ymid = 0.0

innernode = 4*Nx
outernode = 6*Nx

#%% Outer Rectangular Boundary

# left boundary

xleft = []
yleft = []
nxleft = []
nyleft = []
dispXleft = []
dispYleft = [] 
forceXleft = []
forceYleft = []
bxleft = []
byleft = []
for i in range(Ny):
    xleft.append(0.0)
    yleft.append(innerradius + i*h)
    nxleft.append(-1.0)
    nyleft.append(0.0)
    dispXleft.append(0.0)
    dispYleft.append('nan')
    forceXleft.append('nan')
    forceYleft.append(0.0)
    bxleft.append(0.0)
    byleft.append(-rho*go)

# bottom boundary
    
xbottom = []
ybottom = []
nxbottom = []
nybottom = []
dispXbottom = []
dispYbottom = []
forceXbottom = []
forceYbottom = []
bxbottom = []
bybottom = []
for i in range(Nx):
    xbottom.append(innerradius + i*h)
    ybottom.append(0.0)
    nxbottom.append(0.0)
    nybottom.append(-1.0)
    dispXbottom.append('nan')
    dispYbottom.append(0.0)
    forceXbottom.append(0.0)
    forceYbottom.append('nan')
    bxbottom.append(0.0)
    bybottom.append(-rho*go)
    
#%% Quarter Circle Boundary

xinnercircle = []
yinnercircle = []
nxinnercircle = []
nyinnercircle = []
dispXinnercircle = []
dispYinnercircle = []
forceXinnercircle = []
forceYinnercircle = []
bxinnercircle = []
byinnercircle = []

xoutercircle = []
youtercircle = []
nxoutercircle = []
nyoutercircle = []
dispXoutercircle = []
dispYoutercircle = []
forceXoutercircle = []
forceYoutercircle = []
bxoutercircle = []
byoutercircle = []

for i in range(1,innernode-1):
    xinnercircle.append(innerradius*np.cos((i)*np.pi/(2*(innernode-1))) + xmid)
    yinnercircle.append(innerradius*np.sin((i)*np.pi/(2*(innernode-1))) + ymid)
    nxtemp = xmid - xinnercircle[i-1]
    nytemp = ymid - yinnercircle[i-1]
    nxtemp1 = float(nxtemp/np.sqrt(nxtemp**(2) + nytemp**(2)))
    nytemp1 = float(nytemp/np.sqrt(nxtemp**(2) + nytemp**(2)))
    nxinnercircle.append(nxtemp1)
    nyinnercircle.append(nytemp1)
    dispXinnercircle.append('nan')
    dispYinnercircle.append('nan')
    forceXinnercircle.append(nxtemp1*Pin)
    forceYinnercircle.append(nytemp1*Pin)
    bxinnercircle.append(0.0)
    byinnercircle.append(-rho*go)
    
for i in range(1,outernode-1):
    xoutercircle.append(outerradius*np.cos((i)*np.pi/(2*(outernode-1))) + xmid)
    youtercircle.append(outerradius*np.sin((i)*np.pi/(2*(outernode-1))) + ymid)
    nxtemp = xmid - xoutercircle[i-1]
    nytemp = ymid - youtercircle[i-1]
    nxtemp1 = -float(nxtemp/np.sqrt(nxtemp**(2) + nytemp**(2)))
    nytemp1 = -float(nytemp/np.sqrt(nxtemp**(2) + nytemp**(2)))
    nxoutercircle.append(nxtemp1)
    nyoutercircle.append(nytemp1)
    dispXoutercircle.append('nan')
    dispYoutercircle.append('nan')
    forceXoutercircle.append(nxtemp1*Pout)
    forceYoutercircle.append(nytemp1*Pout)
    bxoutercircle.append(0.0)
    byoutercircle.append(-rho*go)


#%% Inner nodes
    
xinner = []
yinner = []
nxinner = []
nyinner = []
dispXinner = []
dispYinner = []
forceXinner = []
forceYinner = []
bxinner = []
byinner = []

for i in range(Nx + int(innerradius/h)):
    for j in range(Ny + int(innerradius/h)):
        xtemp = i*h
        ytemp = j*h
        if (xtemp - xmid)**(2) + (ytemp - ymid)**(2) > innerradius**(2) and (xtemp - xmid)**(2) + (ytemp - ymid)**(2) < outerradius**(2) and xtemp > 0 and ytemp > 0:
            xinner.append(xtemp)
            yinner.append(ytemp)
            nxinner.append('nan')
            nyinner.append('nan')
            dispXinner.append('nan')
            dispYinner.append('nan')
            forceXinner.append('nan')
            forceYinner.append('nan')
            bxinner.append(0.0)
            byinner.append(-rho*go)
    
#%% Combining the boundary and inner nodes list
'''
x = xbottom + xleft + xinner #+ xinnercircle + xoutercircle
y = ybottom + yleft + yinner #+ yinnercircle + youtercircle
nx = nxbottom + nxleft + nxinner #+ nxinnercircle + nxoutercircle
ny = nybottom + nyleft + nyinner #+ nyinnercircle + nyoutercircle
dispX = dispXbottom + dispXleft + dispXinner #+ dispXinnercircle + dispXoutercircle + dispXinner
dispY = dispYbottom + dispYleft + dispYinner #+ dispYinnercircle + dispYoutercircle + dispYinner
forceX = forceXbottom + forceXleft + forceXinner #+ forceXinnercircle + forceXoutercircle
forceY = forceYbottom + forceYleft + forceYinner #+ forceYinnercircle + forceYoutercircle
bx = bxbottom + bxleft + bxinner #+ bxinnercircle + bxoutercircle
by = bybottom + byleft + byinner #+ byinnercircle + byoutercircle
'''
x = xbottom + xleft + xinner + xinnercircle + xoutercircle
y = ybottom + yleft + yinner + yinnercircle + youtercircle
nx = nxbottom + nxleft + nxinner + nxinnercircle + nxoutercircle
ny = nybottom + nyleft + nyinner + nyinnercircle + nyoutercircle
dispX = dispXbottom + dispXleft + dispXinner + dispXinnercircle + dispXoutercircle + dispXinner
dispY = dispYbottom + dispYleft + dispYinner + dispYinnercircle + dispYoutercircle + dispYinner
forceX = forceXbottom + forceXleft + forceXinner + forceXinnercircle + forceXoutercircle
forceY = forceYbottom + forceYleft + forceYinner + forceYinnercircle + forceYoutercircle
bx = bxbottom + bxleft + bxinner + bxinnercircle + bxoutercircle
by = bybottom + byleft + byinner + byinnercircle + byoutercircle


boundaryplot(x,y,dispX,dispY,forceX,forceY,nx,ny,10)

#%% Writing the txt data

file = open("geom_param.txt","w")

file.write(f"{E}")
file.write("\n")
file.write(f"{v}")
file.write("\n")
file.write(f"{h}")

file.close()

file = open("geom_data.txt","w")

for i in range(len(x)):
    file.write(f"{x[i]}" + "\t" + f"{y[i]}" + "\t" + f"{nx[i]}" + "\t" + f"{ny[i]}" + "\t" + f"{dispX[i]}" + "\t" + f"{dispY[i]}" + "\t" + f"{forceX[i]}" + "\t" + f"{forceY[i]}" + "\t" + f"{bx[i]}" + "\t" + f"{by[i]}")
    file.write("\n")
    
file.close()