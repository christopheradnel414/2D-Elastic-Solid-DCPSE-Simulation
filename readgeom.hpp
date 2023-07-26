#ifndef READGEOM_HPP
#define READGEOM_HPP

#include <vector>
#include <string>

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
                string geomparamfilename);

#endif