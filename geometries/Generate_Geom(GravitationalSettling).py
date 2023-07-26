#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jan  8 20:57:46 2020

@author: admin
"""

from plotter import boundaryplot
import numpy as np

E = 210e6
v = 0.3
rho = 700
go = -9.8

a = 100
Nx = 200
Ny = 200
h = a/(Nx-1)

#%% Outer Rectangular Boundary

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
    yleft.append(i*h)
    nxleft.append(-1.0)
    nyleft.append(0.0)
    dispXleft.append(0.0)
    dispYleft.append('nan')
    forceXleft.append('nan')
    forceYleft.append(0.0)
    bxleft.append(0.0)
    byleft.append(-rho*go)
    
xright = []
yright = []
nxright = []
nyright = []
dispXright = []
dispYright = [] 
forceXright = []
forceYright = []
bxright = []
byright = []

for i in range(Ny):
    xright.append((Nx-1)*h)
    yright.append(i*h)
    nxright.append(1.0)
    nyright.append(0.0)
    dispXright.append('nan')
    dispYright.append('nan')
    forceXright.append(0.0)
    forceYright.append(0.0)
    bxright.append(0.0)
    byright.append(-rho*go)
    
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

for i in range(1,Nx-1):
    xbottom.append(i*h)
    ybottom.append(0.0)
    nxbottom.append(0.0)
    nybottom.append(-1.0)
    dispXbottom.append('nan')
    dispYbottom.append(0.0)
    forceXbottom.append(0.0)
    forceYbottom.append('nan')
    bxbottom.append(0.0)
    bybottom.append(-rho*go)
    
xtop = []
ytop = []
nxtop = []
nytop = []
dispXtop = []
dispYtop = []
forceXtop = []
forceYtop = []
bxtop = []
bytop = []

for i in range(1,Nx-1):
    xtop.append(i*h)
    ytop.append((Ny-1)*h)
    nxtop.append(0.0)
    nytop.append(1.0)
    dispXtop.append('nan')
    dispYtop.append('nan')
    forceXtop.append(0.0)
    forceYtop.append(0.0)
    bxtop.append(0.0)
    bytop.append(-rho*go)

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

for i in range(1,Nx-1):
    for j in range(1,Ny-1):
        xtemp = i*h
        ytemp = j*h
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
    
x = xtop + xbottom + xright + xleft + xinner
y = ytop + ybottom + yright + yleft + yinner
nx = nxtop + nxbottom + nxright + nxleft + nxinner
ny = nytop + nybottom + nyright + nyleft + nyinner
dispX = dispXtop + dispXbottom + dispXright + dispXleft + dispXinner
dispY = dispYtop + dispYbottom + dispYright + dispYleft + dispYinner
forceX = forceXtop + forceXbottom + forceXright + forceXleft + forceXinner
forceY = forceYtop + forceYbottom + forceYright + forceYleft + forceYinner
bx = bxtop + bxbottom + bxright + bxleft + bxinner
by = bytop + bybottom + byright + byleft + byinner
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