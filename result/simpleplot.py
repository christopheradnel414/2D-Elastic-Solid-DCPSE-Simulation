# -*- coding: utf-8 -*-
"""
Created on Mon Apr 13 17:31:56 2020

@author: chris
"""

import matplotlib.pyplot as plt


file = open("simplescatter.txt","r")

readdata = file.readlines()

x = []
y = []
data = []

for i in range(len(readdata)):
    temp = readdata[i].split("\t")
    x.append(float(temp[0]))
    y.append(float(temp[1]))
    data.append(float(temp[2]))
    
plt.scatter(x,y,c=data)
plt.axis("equal")