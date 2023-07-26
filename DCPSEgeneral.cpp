#include <vector>
#include <iostream>
#include <cmath>
#include <chrono>
#include "DCPSEgeneral.hpp"
#include "eigen-3.3.7/Eigen/Dense"

using namespace std;
using namespace Eigen;


vector<MatrixXd> DCPSEcalcAinv (vector<double> x,
                                vector<double> y,
                                vector<vector<int>> neighbour,
                                double e)
{
    // Initial Checking
    int particlenumber = x.size();
    for (int i = 0; i < particlenumber; i++)
    {
        if (neighbour[i].size() < 9)
        {
            cout <<"DCPSE Operator Error: Neighbour size insufficient"<<endl;
            exit(EXIT_FAILURE);
        }
    }

    // Begin Calculation
    cout << "Calculating DCPSE Constant A inverse ...";

    auto start =chrono::steady_clock::now();

    double dx1;
    double dy1;
    double dx2;
    double dy2;

    double V1;
    double V2;
    double V3;
    double V4;
    double V5;
    double V6;
    double V7;
    double V8;
    double V9;

    vector<MatrixXd> Ainv(particlenumber);

    for (int i = 0; i < particlenumber; i++)
    {
        int neighboursize = neighbour[i].size();

        // Calculating V matrix for each particle
        // P = [1,x,y,xy,x^2,y^2,x^2y,xy^2,x^2y^2]
        MatrixXd V(neighboursize,9);

        for (int j = 0; j < neighboursize; j++)
        {
            dx1 = x[i] - x[neighbour[i][j]];
            dy1 = y[i] - y[neighbour[i][j]];
            dx2 = dx1/e;
            dy2 = dy1/e;

            V1 = 1;
            V2 = dx2;
            V3 = dy2;
            V4 = dx2*dy2;
            V5 = pow(dx2,2);
            V6 = pow(dy2,2);
            V7 = pow(dx2,2)*dy2;
            V8 = dx2*pow(dy2,2);
            V9 = pow(dx2,2)*pow(dy2,2);

            V(j,0) = V1;
            V(j,1) = V2;
            V(j,2) = V3;
            V(j,3) = V4;
            V(j,4) = V5;
            V(j,5) = V6;
            V(j,6) = V7;
            V(j,7) = V8;
            V(j,8) = V9;
        }
        
        // Calculating E matrix for each particle
        MatrixXd E = MatrixXd::Zero(neighboursize,neighboursize);

        for (int j = 0; j < neighboursize; j++)
        {
            dx1 = x[i] - x[neighbour[i][j]];
            dy1 = y[i] - y[neighbour[i][j]];

            E(j,j) = exp(-(pow(dx1,2) + pow(dy1,2)) /(2*pow(e,2)));
        }

        MatrixXd B = E*V;

        MatrixXd A = B.transpose()*B;

        Ainv[i] = A.inverse();
    }

    auto end = chrono::steady_clock::now();
    cout << "Finished (" <<chrono::duration_cast<chrono::milliseconds>(end-start).count()<<" ms)"<< endl;

    return Ainv;

}



vector<vector<double>> DCPSEcalcEta(vector<double> x,
                                    vector<double> y,
                                    vector<vector<int>> neighbour,
                                    double e,
                                    int m,
                                    int n,
                                    vector<MatrixXd> Ainv)
{
    VectorXd b(9);

    if (m == 1 && n == 0)
    {
        b << 0,-1,0,0,0,0,0,0,0;
    }
    else if (m == 0 && n == 1)
    {
        b << 0,0,-1,0,0,0,0,0,0;
    }
    else if (m == 1 && n == 1)
    {
        b << 0,0,0,1,0,0,0,0,0;
    }
    else if (m == 2 && n == 0)
    {
        b << 0,0,0,0,2,0,0,0,0;
    }
    else if (m == 0 && n == 2)
    {
        b << 0,0,0,0,0,2,0,0,0;
    }
    else
    {
        cout << "DCPSE Constant function error: m or n too high" <<endl;
    }

    // Begin Calculation
    cout << "Calculating DCPSE Operator Constants ("<<m<<","<<n<<") ... ";

    auto start =chrono::steady_clock::now();

    double dx1;
    double dy1;
    double dx2;
    double dy2;

    int particlenumber = x.size();

    vector<vector<double>> Eta(particlenumber);

    for (int i = 0; i < particlenumber; i++)
    {
        VectorXd a = Ainv[i]*b;

        Eta[i] = vector<double>(neighbour[i].size());

        for (int j = 0; j < neighbour[i].size(); j++)
        {
            dx1 = x[i] - x[neighbour[i][j]];
            dy1 = y[i] - y[neighbour[i][j]];
            dx2 = dx1/e;
            dy2 = dy1/e;

            Eta[i][j] = ((a[0] + a[1]*dx2 + a[2]*dy2 + a[3]*dx2*dy2 + a[4]*pow(dx2,2) + a[5]*pow(dy2,2) + a[6]*pow(dx2,2)*dy2 + a[7]*dx2*pow(dy2,2) + a[8]*pow(dx2,2)*pow(dy2,2))*exp(-pow(dx2,2) - pow(dy2,2)));
        }
    }

    auto end = chrono::steady_clock::now();
    cout << "Finished (" <<chrono::duration_cast<chrono::milliseconds>(end-start).count()<<" ms)"<< endl;

    return Eta;
}

vector<double> DCPSEcalcGrad   (vector<double> w,
                                vector<vector<int>> neighbour,
                                double e,
                                int m,
                                int n,
                                vector<MatrixXd> Ainv,
                                vector<vector<double>> Eta)
{
    // Begin Calculation
    cout << "Calculating DCPSE Gradient ("<<m<<","<<n<<") ... ";

    auto start =chrono::steady_clock::now();

    double resulttemp;

    int particlenumber = w.size();

    vector<double> result(particlenumber);

    for (int i = 0; i < particlenumber; i++)
    {
        resulttemp = 0;
        for (int j = 0; j < neighbour[i].size(); j++)
        {
            resulttemp = resulttemp + (w[i] + w[neighbour[i][j]])*Eta[i][j];
        }
        result[i] = resulttemp/pow(e,m+n);
    }

    auto end = chrono::steady_clock::now();
    cout << "Finished (" <<chrono::duration_cast<chrono::milliseconds>(end-start).count()<<" ms)"<< endl;
    
    return result;
}