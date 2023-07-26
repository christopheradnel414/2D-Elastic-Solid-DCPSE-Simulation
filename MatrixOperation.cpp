#include <vector>
#include <iostream>
#include <cmath>
#include <chrono>
#include <omp.h>
#include "eigen-3.3.7/Eigen/Dense"
#include "eigen-3.3.7/Eigen/Sparse"
#include "eigen-3.3.7/Eigen/OrderingMethods"
#include "MatrixOperation.hpp"

using namespace std;
using namespace Eigen;

void ArrangeEquationMatrix (vector<double> nx,
                            vector<double> ny,
                            vector<int> nxi,
                            vector<int> nyi,
                            vector<double> dispX,
                            vector<double> dispY,
                            vector<int> dispXi,
                            vector<int> dispYi,
                            vector<double> forceX,
                            vector<double> forceY,
                            vector<int> forceXi,
                            vector<int> forceYi,
                            vector<double> bx,
                            vector<double> by,
                            double lamda,
                            double Mu,
                            int particlenumber,
                            vector<vector<int>> neighbour,
                            vector<vector<double>> EtaDx,
                            vector<vector<double>> EtaDy,
                            vector<vector<double>> EtaDx2,
                            vector<vector<double>> EtaDy2,
                            vector<vector<double>> EtaDxDy,
                            double e,
                            VectorXd& bresult,
                            SparseMatrix<double>& Aresult)
{
    cout << "Arranging Equation Matrix ... ";

    auto start =chrono::steady_clock::now();
    
    typedef Triplet<double> T;
    vector<T> tripletlist;

    VectorXd b = VectorXd::Zero(2*particlenumber);
    
    int row;
    int col;
    double data;

    double temp1;
    double temp2;
    double temp3;
    double temp4;

    for (int i = 0; i < particlenumber; i++)
    {
        // Displacement Boundary X
        if (dispXi[i] == 1)
        {
            row = 2*i;
            col = 2*i;
            data = 1.0;

            tripletlist.push_back(T(row,col,data));

            b(2*i) = dispX[i];
        }

        // Displacement Boundary Y
        if (dispYi[i] == 1)
        {
            row = 2*i + 1;
            col = 2*i + 1;
            data = 1.0;

            tripletlist.push_back(T(row,col,data));

            b(2*i+1) = dispY[i];
        }

        // Force Boundary X
        if (forceXi[i] == 1)
        {
            temp1 = 0.0;
            temp2 = 0.0;
            
            for (int j = 0; j < neighbour[i].size(); j++)
            {
                // A[2*i][2*i] =  A[2*i][2*i] + (lamda + 2*Mu)*nx[i]*DCPSEEtaDx[i][j]/e**(1) + Mu*ny[i]*DCPSEEtaDy[i][j]/e**(1)
                temp1 = temp1 + (lamda + 2*Mu)*nx[i]*EtaDx[i][j]/e + Mu*ny[i]*EtaDy[i][j]/e;
                
                // A[2*i][2*i+1] = A[2*i][2*i+1] + lamda*nx[i]*DCPSEEtaDy[i][j]/e**(1) + Mu*ny[i]*DCPSEEtaDx[i][j]/e**(1)
                temp2 = temp2 + lamda*nx[i]*EtaDy[i][j]/e + Mu*ny[i]*EtaDx[i][j]/e;
                
                // A[2*i][2*neighbour[i][j]] = (lamda + 2*Mu)*nx[i]*DCPSEEtaDx[i][j]/e**(1) + Mu*ny[i]*DCPSEEtaDy[i][j]/e**(1)
                row = 2*i;
                col = 2*neighbour[i][j];
                data = (lamda + 2*Mu)*nx[i]*EtaDx[i][j]/e + Mu*ny[i]*EtaDy[i][j]/e;

                tripletlist.push_back(T(row,col,data));
            
                // A[2*i][2*neighbour[i][j]+1] = lamda*nx[i]*DCPSEEtaDy[i][j]/e**(1) + Mu*ny[i]*DCPSEEtaDx[i][j]/e**(1)
                row = 2*i;
                col = 2*neighbour[i][j]+1;
                data = lamda*nx[i]*EtaDy[i][j]/e + Mu*ny[i]*EtaDx[i][j]/e;

                tripletlist.push_back(T(row,col,data));
            
                if (j == neighbour[i].size()-1)
                    {
                        row = 2*i;
                        col = 2*i;
                        data = temp1;

                        tripletlist.push_back(T(row,col,data));
                        
                        row = 2*i;
                        col = 2*i+1;
                        data = temp2;

                        tripletlist.push_back(T(row,col,data));
                    }
                    
            }
            b(2*i) = forceX[i];
        }

        // Force Boundary Y
        if (forceYi[i] == 1)
        {
            temp1 = 0.0;
            temp2 = 0.0;
            
            for (int j = 0; j < neighbour[i].size(); j++)
            {               
                // A[2*i+1][2*i] =  A[2*i+1][2*i] + lamda*ny[i]*DCPSEEtaDx[i][j]/e**(1) + Mu*nx[i]*DCPSEEtaDy[i][j]/e**(1)
                temp1 = temp1 + lamda*ny[i]*EtaDx[i][j]/e + Mu*nx[i]*EtaDy[i][j]/e;
                
                // A[2*i+1][2*i+1] = A[2*i+1][2*i+1] + (lamda + 2*Mu)*ny[i]*DCPSEEtaDy[i][j]/e**(1) + Mu*nx[i]*DCPSEEtaDx[i][j]/e**(1)
                temp2 = temp2 + (lamda + 2*Mu)*ny[i]*EtaDy[i][j]/e + Mu*nx[i]*EtaDx[i][j]/e;
                
                // A[2*i+1][2*neighbour[i][j]] = lamda*ny[i]*DCPSEEtaDx[i][j]/e**(1) + Mu*nx[i]*DCPSEEtaDy[i][j]/e**(1)
                row = 2*i+1;
                col = 2*neighbour[i][j];
                data = lamda*ny[i]*EtaDx[i][j]/e + Mu*nx[i]*EtaDy[i][j]/e;

                tripletlist.push_back(T(row,col,data));
                
                // A[2*i+1][2*neighbour[i][j]+1] = (lamda + 2*Mu)*ny[i]*DCPSEEtaDy[i][j]/e**(1) + Mu*nx[i]*DCPSEEtaDx[i][j]/e**(1)
                row = 2*i+1;
                col = 2*neighbour[i][j]+1;
                data = (lamda + 2*Mu)*ny[i]*EtaDy[i][j]/e + Mu*nx[i]*EtaDx[i][j]/e;

                tripletlist.push_back(T(row,col,data));
                
                if (j == neighbour[i].size()-1)
                {
                    row = 2*i+1;
                    col = 2*i;
                    data = temp1;

                    tripletlist.push_back(T(row,col,data));
                    
                    row = 2*i+1;
                    col = 2*i+1;
                    data = temp2;

                    tripletlist.push_back(T(row,col,data));
                }
            }
            b(2*i+1) = forceY[i];
        }
        
        // Inner Particles
        if ((forceXi[i] == 0) && ((forceYi[i] == 0) && ((dispXi[i] == 0) && (dispYi[i] == 0))))
        {
            temp1 = 0.0;
            temp2 = 0.0;
            temp3 = 0.0;
            temp4 = 0.0;
            
            for (int j = 0; j < neighbour[i].size(); j++)
            {
                // A[2*i][2*i] = A[2*i][2*i] + (lamda + 2*Mu)*DCPSEEtaDx2[i][j]/e**(2) + Mu*DCPSEEtaDy2[i][j]/e**(2)
                temp1 = temp1 + (lamda + 2*Mu)*EtaDx2[i][j]/pow(e,2) + Mu*EtaDy2[i][j]/pow(e,2);
                
                // A[2*i][2*i+1] = A[2*i][2*i+1] + (lamda + Mu)*DCPSEEtaDxDy[i][j]/e**(2)
                temp2 = temp2 + (lamda + Mu)*EtaDxDy[i][j]/pow(e,2);
                
                // A[2*i][2*neighbour[i][j]] = (lamda + 2*Mu)*DCPSEEtaDx2[i][j]/e**(2) + Mu*DCPSEEtaDy2[i][j]/e**(2)
                row = 2*i;
                col = 2*neighbour[i][j];
                data = (lamda + 2*Mu)*EtaDx2[i][j]/pow(e,2) + Mu*EtaDy2[i][j]/pow(e,2);

                tripletlist.push_back(T(row,col,data));
                
                // A[2*i][2*neighbour[i][j]+1] = (lamda + Mu)*DCPSEEtaDxDy[i][j]/e**(2)
                row = 2*i;
                col = 2*neighbour[i][j]+1;
                data = (lamda + Mu)*EtaDxDy[i][j]/pow(e,2);

                tripletlist.push_back(T(row,col,data));

                
                // A[2*i+1][2*i] = A[2*i+1][2*i] + (lamda + Mu)*DCPSEEtaDxDy[i][j]/e**(2)
                temp3 = temp3 + (lamda + Mu)*EtaDxDy[i][j]/pow(e,2);
                
                // A[2*i+1][2*i+1] = A[2*i+1][2*i+1] + (lamda + 2*Mu)*DCPSEEtaDy2[i][j]/e**(2) + Mu*DCPSEEtaDx2[i][j]/e**(2)
                temp4 = temp4 + (lamda + 2*Mu)*EtaDy2[i][j]/pow(e,2) + Mu*EtaDx2[i][j]/pow(e,2);
                
                // A[2*i+1][2*neighbour[i][j]] = (lamda + Mu)*DCPSEEtaDxDy[i][j]/e**(2)
                row = 2*i+1;
                col = 2*neighbour[i][j];
                data = (lamda + Mu)*EtaDxDy[i][j]/pow(e,2);

                tripletlist.push_back(T(row,col,data));
                
                // A[2*i+1][2*neighbour[i][j]+1] = (lamda + 2*Mu)*DCPSEEtaDy2[i][j]/e**(2) + Mu*DCPSEEtaDx2[i][j]/e**(2)
                row = 2*i+1;
                col = 2*neighbour[i][j]+1;
                data = (lamda + 2*Mu)*EtaDy2[i][j]/pow(e,2) + Mu*EtaDx2[i][j]/pow(e,2);

                tripletlist.push_back(T(row,col,data));
                
                
                if (j == neighbour[i].size()-1)
                {   
                    row = 2*i;
                    col = 2*i;
                    data = temp1;

                    tripletlist.push_back(T(row,col,data));
                    
                    row = 2*i;
                    col = 2*i+1;
                    data = temp2;

                    tripletlist.push_back(T(row,col,data));
                    
                    row = 2*i+1;
                    col = 2*i;
                    data = temp3;

                    tripletlist.push_back(T(row,col,data));
                    
                    row = 2*i+1;
                    col = 2*i+1;
                    data = temp4;

                    tripletlist.push_back(T(row,col,data));

                }
            }
            b(2*i) = bx[i];
            b(2*i+1) = by[i];
        }
        
    }
    SparseMatrix<double> A(2*particlenumber,2*particlenumber);
    A.setFromTriplets(tripletlist.begin(), tripletlist.end());

    Aresult = A;
    bresult = b;

    auto end = chrono::steady_clock::now();
    cout << "Finished (" <<chrono::duration_cast<chrono::milliseconds>(end-start).count()<<" ms)"<< endl;
}

void SolveMatrix   (SparseMatrix<double> A,
                    VectorXd b,
                    vector<double>& Ux,
                    vector<double>& Uy,
                    int method,
                    int preconditioner_iterative_solver,
                    double tolerance_iterative_solver,
                    int iteration_limit_iterative_solver,
                    int n_cpu_iterative_solver)
{
    cout << "Solving Matrix Model ";

    auto start =chrono::steady_clock::now();

    A.makeCompressed();

    if (method == 1)
    {
        cout << "(Direct LU) ... " <<endl;
        
        SparseLU <SparseMatrix<double>, COLAMDOrdering<int>> solver;

        solver.compute(A);

        VectorXd U = solver.solve(b);

        int j = 0;
        int k = 0;

        for (int i = 0; i < b.rows(); i++)
        {
            if (i%2 == 0)
            {
                Ux[j] = U(i);
                j++;
            }
            if (i%2 == 1)
            {
                Uy[k] = U(i);
                k++;
            }
        }
    }
    else if (method == 2)
    {
        initParallel();
        omp_set_num_threads(n_cpu_iterative_solver);
        setNbThreads(n_cpu_iterative_solver);

        if (preconditioner_iterative_solver == 1)
        {
            cout << "(BiCGSTAB Jacobian) ... ";

            BiCGSTAB <SparseMatrix<double,RowMajor>> solver;

            solver.setTolerance(tolerance_iterative_solver);

            if (iteration_limit_iterative_solver == 0)
            {
                solver.setMaxIterations(b.rows()/5);
            }
            else
            {
                solver.setMaxIterations(iteration_limit_iterative_solver);
            }

            solver.compute(A);

            VectorXd U = solver.solve(b);
            cout <<endl<< "#iterations:     " << solver.iterations() << endl;
            cout << "estimated error: " << solver.error()      << endl;

            int j = 0;
            int k = 0;

            for (int i = 0; i < b.rows(); i++)
            {
                if (i%2 == 0)
                {
                    Ux[j] = U(i);
                    j++;
                }
                if (i%2 == 1)
                {
                    Uy[k] = U(i);
                    k++;
                }
            }
        }
        else if (preconditioner_iterative_solver == 2)
        {
            cout << "(BiCGSTAB IncompleteLUT) ... ";

            BiCGSTAB <SparseMatrix<double,RowMajor>,IncompleteLUT<double>> solver;
            solver.preconditioner().setDroptol(0.001);

            solver.setTolerance(tolerance_iterative_solver);

            if (iteration_limit_iterative_solver == 0)
            {
                solver.setMaxIterations(b.rows()/5);
            }
            else
            {
                solver.setMaxIterations(iteration_limit_iterative_solver);
            }

            solver.compute(A);

            VectorXd U = solver.solve(b);
            cout <<endl<< "#iterations:     " << solver.iterations() << endl;
            cout << "estimated error: " << solver.error()      << endl;

            int j = 0;
            int k = 0;

            for (int i = 0; i < b.rows(); i++)
            {
                if (i%2 == 0)
                {
                    Ux[j] = U(i);
                    j++;
                }
                if (i%2 == 1)
                {
                    Uy[k] = U(i);
                    k++;
                }
            }
        }
        else
        {
            cout << "Solving Error : Iterative Solver Preconditioner Undefined" << endl;
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        cout << "Solving Error : Solver Method Undefined" << endl;
        exit(EXIT_FAILURE);
    }
    auto end = chrono::steady_clock::now();
    cout << "Matrix Solver Finished (" <<chrono::duration_cast<chrono::milliseconds>(end-start).count()<<" ms)"<< endl;
}