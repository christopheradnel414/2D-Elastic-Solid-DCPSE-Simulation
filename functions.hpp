#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <vector>
#include "functions.hpp"

using namespace std;

bool is_number(const std::string& s);

double findmax(vector<double> vect);

double findmin(vector<double> vect);

void calcStressStrain  (int boundaryplotmethod,
                        vector<double>& epsxx,
                        vector<double>& epsyy,
                        vector<double>& epsxy,
                        vector<double>& sigmax,
                        vector<double>& sigmay,
                        vector<double>& sigmaxy,
                        vector<double>& vonmises,
                        vector<double> dUxdX,
                        vector<double> dUxdY,
                        vector<double> dUydX,
                        vector<double> dUydY,
                        double lamda,
                        double Mu,
                        vector<int> dispXi,
                        vector<int> dispYi,
                        vector<int> forceXi,
                        vector<int> forceYi);

void writeresult   (int boundaryplotmethod,
                    vector<double> x,
                    vector<double> y,
                    vector<double> vonmises,
                    vector<double> sigmax,
                    vector<double> sigmay,
                    vector<double> sigmaxy,
                    vector<double> Ux,
                    vector<double> Uy,
                    double h,
                    vector<int> dispXi,
                    vector<int> dispYi,
                    vector<int> forceXi,
                    vector<int> forceYi,
                    string resultfilename);

#endif