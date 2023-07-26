#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include "readgeom.hpp"
#include "functions.hpp"

using namespace std;

void readgeom  (double& E,
                double& v,
                double& h,
                vector<double>& x,
                vector<double>& y,
                vector<double>& nx,
                vector<double>& ny,
                vector<int>& nxi,
                vector<int>& nyi,
                vector<double>& dispX,
                vector<double>& dispY,
                vector<int>& dispXi,
                vector<int>& dispYi,
                vector<double>& forceX,
                vector<double>& forceY,
                vector<int>& forceXi,
                vector<int>& forceYi,
                vector<double>& bx,
                vector<double>& by,
                string geomfilename,
                string geomparamfilename)
{

    // Reading the geometry data file
    cout <<"Reading geometry file ... ";

    auto start = chrono::steady_clock::now();

    ifstream file;

    vector<double> param_temp;
    double reader;

    file.open(geomparamfilename);

    if (!file) 
    {
    cerr << "Unable to open file " << geomparamfilename << endl;
    exit(1);   // call system to stop
    }

    while (file >> reader)
    {
        param_temp.push_back(reader);
    }
    file.close();

    E = param_temp[0];
    v = param_temp[1];
    h = param_temp[2];
    
    string xs,ys,nxs,nys,dispXs,dispYs,forceXs,forceYs,bxs,bys;
    file.open(geomfilename);

    if (!file) 
    {
    cerr << "Unable to open file " << geomfilename << endl;
    exit(1);   // call system to stop
    }

    while (file>>xs>>ys>>nxs>>nys>>dispXs>>dispYs>>forceXs>>forceYs>>bxs>>bys)
    {
        // reading x and y
        x.push_back(stod(xs));
        y.push_back(stod(ys));

        // reading nx
        if (is_number(nxs))
        {
            nx.push_back(stod(nxs));
            nxi.push_back(1);
        }
        else
        {
            nx.push_back(0.0);
            nxi.push_back(0);
        }
        
        // reading ny
        if (is_number(nys))
        {
            ny.push_back(stod(nys));
            nyi.push_back(1);
        }
        else
        {
            ny.push_back(0.0);
            nyi.push_back(0);
        }

        // reading dispX
        if (is_number(dispXs))
        {
            dispX.push_back(stod(dispXs));
            dispXi.push_back(1);
        }
        else
        {
            dispX.push_back(0.0);
            dispXi.push_back(0);
        }

        // reading dispY
        if (is_number(dispYs))
        {
            dispY.push_back(stod(dispYs));
            dispYi.push_back(1);
        }
        else
        {
            dispY.push_back(0.0);
            dispYi.push_back(0);
        }

        // reading forceX
        if (is_number(forceXs))
        {
            forceX.push_back(stod(forceXs));
            forceXi.push_back(1);
        }
        else
        {
            forceX.push_back(0.0);
            forceXi.push_back(0);
        }

        // reading forceY
        if (is_number(forceYs))
        {
            forceY.push_back(stod(forceYs));
            forceYi.push_back(1);
        }
        else
        {
            forceY.push_back(0.0);
            forceYi.push_back(0);
        }

        // reading bx and by
        bx.push_back(stod(bxs));
        by.push_back(stod(bys));
        
    }
    file.close();

    auto end = chrono::steady_clock::now();
    cout << "Finished (" <<chrono::duration_cast<chrono::milliseconds>(end-start).count()<<" ms)"<< endl;

    cout << "Number of Particles: " <<x.size()<<endl;
}