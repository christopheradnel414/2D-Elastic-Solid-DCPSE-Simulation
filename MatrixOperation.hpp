#ifndef MATRIXOPERATION_HPP
#define MATRIXOPERATION_HPP
#define NDEBUG
#define EIGEN_NO_STATIC_ASSERT

#include <vector>
#include "eigen-3.3.7/Eigen/Dense"
#include "eigen-3.3.7/Eigen/Sparse"
#include "eigen-3.3.7/Eigen/OrderingMethods"

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
                            SparseMatrix<double>& Aresult);

void SolveMatrix   (SparseMatrix<double> A,
                    VectorXd b,
                    vector<double>& Ux,
                    vector<double>& Uy,
                    int method,
                    int preconditioner_iterative_solver,
                    double tolerance_iterative_solver,
                    int iteration_limit_iterative_solver,
                    int n_cpu_iterative_solver);

#endif