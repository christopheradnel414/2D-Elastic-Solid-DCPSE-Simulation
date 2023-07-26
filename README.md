# 2D Elastic Solic DCPSE Simulation
This repository contains the source code for a 2D linear elastostatics solid simulation using particle-based discretization method DCPSE (Discretization Corrected Particle Strength Exchange). This work is part of my undergraduate thesis in computational solid mechanics and this work is also published in JETS 2022 Journal titled "Discretization Corrected Particle Strength Exchange for Steady State Linear Elasticity". 

This repository mainly consists of 3 parts:
1. 2D Linear Elastostatics DCPSE Source Code (main directory)
2. Example Geometry Generation Code (geometries folder)
3. Result Plotter Code (result folder)

The main Solver is written in C++ and utilizes the Eigen (https://eigen.tuxfamily.org) library as the sparse linear algebra engine. The main solver takes into input a .txt file containing point clouds, each anotated with the constraint and condition of the nodes (inner node, dirichlet boundary node, neumann boundary node, etc) and outputs a .txt file of the simulation results containing the node displacements, stress, strain, etc.

Next, the solver will first compute the closest neighbours of each nodes by iterating through every possible node pairs and only considers node within the cutoff radius. 
