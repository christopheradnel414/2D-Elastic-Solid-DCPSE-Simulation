#include "readgeom.hpp"
#include "neighbourfindsimple.hpp"
#include "DCPSEgeneral.hpp"
#include "MatrixOperation.hpp"
#include "readparameter.hpp"
#include "functions.hpp"
#include <string>
#include <chrono>
#include <iostream>

using namespace std;

int main()
{
    auto start = chrono::steady_clock::now();

    // Reading Solver Parameters Data
    int boundaryplotmethod;
    string geomfilename;
    string geomparamfilename;
    string resultfilename;
    int neighbourmethod;
    int neighbournumber;
    double rcScale;
    int method_solver;
    int preconditioner_iterative_solver;
    double tolerance_iterative_solver;
    int iteration_limit_iterative_solver;
    int n_cpu_iterative_solver;

    readsolverparameterfile(boundaryplotmethod,geomfilename,geomparamfilename,
                            resultfilename,neighbourmethod,neighbournumber,
                            rcScale,method_solver,preconditioner_iterative_solver,
                            tolerance_iterative_solver,iteration_limit_iterative_solver,
                            n_cpu_iterative_solver);

    // Initializing the geometry parameters
    double E;
    double v;
    double h;

    vector<double> x;
    vector<double> y;
    vector<double> nx;
    vector<double> ny;
    vector<int> nxi;
    vector<int> nyi;
    vector<double> dispX;
    vector<double> dispY;
    vector<int> dispXi;
    vector<int> dispYi;
    vector<double> forceX;
    vector<double> forceY;
    vector<int> forceXi;
    vector<int> forceYi;
    vector<double> bx;
    vector<double> by;

    // Reading the geometry file
    readgeom   (E,v,h,x,y,nx,ny,nxi,nyi,dispX,dispY,dispXi,dispYi,
                forceX,forceY,forceXi,forceYi,bx,by,geomfilename,
                geomparamfilename);

    int particlenumber = x.size();

    // Nearest Neighbour Search
    double rc = rcScale*h;
    vector<vector<int>> neighbour(particlenumber);
    if (neighbourmethod == 1)
    {
        neighbourfind(neighbour,x,y,rc);
    }
    else if (neighbourmethod == 2)
    {
        neighbourfindlimited(neighbour,x,y,rc,neighbournumber);
    }
    for (int i = 0; i < particlenumber; i++)
    {
        neighbour[i].push_back(i);
    }

    double eps = h;

    // DCPSE Operator Constants
    vector<MatrixXd> Ainv = DCPSEcalcAinv(x,y,neighbour,eps);
    
    vector<vector<double>> EtaDx = DCPSEcalcEta(x,y,neighbour,eps,1,0,Ainv);
    vector<vector<double>> EtaDy = DCPSEcalcEta(x,y,neighbour,eps,0,1,Ainv);
    vector<vector<double>> EtaDx2 = DCPSEcalcEta(x,y,neighbour,eps,2,0,Ainv);
    vector<vector<double>> EtaDy2 = DCPSEcalcEta(x,y,neighbour,eps,0,2,Ainv);
    vector<vector<double>> EtaDxDy = DCPSEcalcEta(x,y,neighbour,eps,1,1,Ainv);

    // Calculating Lame Constants
    double lamda = v*E/((1+v)*(1-2*v));
    double Mu = E/(2*(1+v));

    // Equation Matrix Modelling
    VectorXd b;
    SparseMatrix<double> A;
    ArrangeEquationMatrix  (nx,ny,nxi,nyi,dispX,dispY,dispXi,dispYi,forceX,forceY,
                            forceXi,forceYi,bx,by,lamda,Mu,particlenumber,neighbour,
                            EtaDx,EtaDy,EtaDx2,EtaDy2,EtaDxDy,h,b,A);
    
    // Solving Matrix Model
    vector<double> Ux(particlenumber);
    vector<double> Uy(particlenumber);
    SolveMatrix(A,b,Ux,Uy,method_solver,preconditioner_iterative_solver,
                tolerance_iterative_solver,iteration_limit_iterative_solver,
                n_cpu_iterative_solver);

    cout <<"Max Positive X displacement: "<< findmax(Ux) << endl;
    cout <<"Max Negative X displacement: "<< findmin(Ux) << endl;
    cout <<"Max Positive Y displacement: "<< findmax(Uy) << endl;
    cout <<"Max Negative Y displacement: "<< findmin(Uy) << endl;

    // Solving for Displacement Gradient
    vector<double> dUxdX = DCPSEcalcGrad(Ux,neighbour,h,1,0,Ainv,EtaDx);
    vector<double> dUxdY = DCPSEcalcGrad(Ux,neighbour,h,0,1,Ainv,EtaDy);
    vector<double> dUydX = DCPSEcalcGrad(Uy,neighbour,h,1,0,Ainv,EtaDx);
    vector<double> dUydY = DCPSEcalcGrad(Uy,neighbour,h,0,1,Ainv,EtaDy);

    // Solving for Stress and Strain
    vector<double> epsxx;
    vector<double> epsyy;
    vector<double> epsxy;
    vector<double> sigmax;
    vector<double> sigmay;
    vector<double> sigmaxy;
    vector<double> vonmises;

    calcStressStrain(boundaryplotmethod,epsxx,epsyy,epsxy,sigmax,sigmay,sigmaxy,vonmises,dUxdX,dUxdY,
                     dUydX,dUydY,lamda,Mu,dispXi,dispYi,forceXi,forceYi);

    // Printing Result
    writeresult(boundaryplotmethod,x,y,vonmises,sigmax,sigmay,sigmaxy,Ux,Uy,h,dispXi,dispYi,
                forceXi,forceYi,resultfilename);

    auto end = chrono::steady_clock::now();
    cout << "Total Elapsed Time: " <<chrono::duration_cast<chrono::milliseconds>(end-start).count()<<" ms"<< endl;

}