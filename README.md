# 2D Elastic Solic DCPSE Simulation
This repository contains the source code for a 2D linear elastostatics solid simulation using particle-based discretization method DCPSE (Discretization Corrected Particle Strength Exchange). This work is part of my undergraduate thesis in computational solid mechanics and this work is also published in JETS 2022 Journal titled "Discretization Corrected Particle Strength Exchange for Steady State Linear Elasticity". 

<img height="150" alt="Screenshot 2023-07-27 at 00 47 04" src="https://github.com/christopheradnel414/2D-Elastic-Solid-DCPSE-Simulation/assets/41734037/013c7c04-f879-40f0-a56a-b835abb52dbb">
<img height="150" alt="Screenshot 2023-07-27 at 00 47 52" src="https://github.com/christopheradnel414/2D-Elastic-Solid-DCPSE-Simulation/assets/41734037/e4c2e8d8-7a25-4079-9d84-170987da16a0">
<img height="150" alt="Screenshot 2023-07-27 at 00 48 04" src="https://github.com/christopheradnel414/2D-Elastic-Solid-DCPSE-Simulation/assets/41734037/14dbed55-1557-40d8-a93b-aa8f45a55e76">
<img height="150" alt="Screenshot 2023-07-27 at 00 49 32" src="https://github.com/christopheradnel414/2D-Elastic-Solid-DCPSE-Simulation/assets/41734037/e29db208-f614-41d9-a329-0b9ab0b48df3">


This repository mainly consists of 3 parts:
1. 2D Linear Elastostatics DCPSE Source Code (main directory)
2. Example Geometry Generation Code (geometries folder)
3. Result Plotter Code (result folder)

The main Solver is written in C++ and utilizes the Eigen (https://eigen.tuxfamily.org) library as the sparse linear algebra engine. The main solver takes into input a .txt file containing point clouds, each anotated with the constraint and condition of the nodes (inner node, dirichlet boundary node, neumann boundary node, etc) and outputs a .txt file of the simulation results containing the node displacements, stresses, and strains. This solver works by solving the Static Navier-Cauchy equations (https://en.wikipedia.org/wiki/Linear_elasticity) using DCPSE discretization method. The 2D Static Navier-Cauchy equations can be written as follows:

1. Static Navier-Cauchy for Inner Nodes:
<img height="100" alt="Screenshot 2023-07-27 at 00 42 36" src="https://github.com/christopheradnel414/2D-Elastic-Solid-DCPSE-Simulation/assets/41734037/f02865cf-ff84-4b38-97b9-4aeb8a15e7f2">

2. Static Navier-Cauchy for Neumann Nodes (Traction/Force Boundary):
<img height="100" alt="Screenshot 2023-07-27 at 00 42 46" src="https://github.com/christopheradnel414/2D-Elastic-Solid-DCPSE-Simulation/assets/41734037/138f0154-5cfb-4428-a17c-ceec1e6dcda1">

3. Static Navier-Cauchy for Dirichlet Nodes (Displacement Boundary):
<img height="70" alt="Screenshot 2023-07-27 at 00 42 58" src="https://github.com/christopheradnel414/2D-Elastic-Solid-DCPSE-Simulation/assets/41734037/34f4ac2b-c501-4d77-b175-994021ad1846">

There are many methods that can be used to solve the Static Navier-Cauchy Equations (e.g, finite difference, finite element, SPH, etc) which I discussed in more detail on my journal (https://journals.itb.ac.id/index.php/jets/article/view/17606). Here, we are only interested in using DCPSE as our discretization method to solve the Navier-Cauchy equation. DCPSE operator as introduced by Schrader,2010 (https://publications.mpi-cbg.de/Schrader_2010_4838.pdf) approximates the value and spatial gradients of a field function as a linear combination of the neighbouring nodes function values. This is written as follows:

<img height="200" alt="Screenshot 2023-07-27 at 01 03 56" src="https://github.com/christopheradnel414/2D-Elastic-Solid-DCPSE-Simulation/assets/41734037/d503a3fb-bb3c-4904-a4cd-be936ec17188">

The detail on the computation of the DCPSE operator is given on the journal. However, the main idea is that to approximate the spatial gradient at any particular point, it is simply the weighted sum of the neighbouring nodes' function value. The weights $\eta_{ij}$ can be easily computed as detailed on the journal and this leaves us only to solve the huge system of linear equations after discretizing the Navier-Cauchy equations using DCPSE. To solve this huge system system of equations, we are using Eigen library's sparse matrix solver capability.


