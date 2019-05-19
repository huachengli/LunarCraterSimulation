#ifndef _SAMPLING_H_
#define _SAMPLING_H_
// #define _DEBUG
#include <iostream>
#include <random>
#include <chrono>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <omp.h>
#include <cmath>
#include "param.h"
#define MaxThread 200
#define MaxLen 30000
#define _PI_ 3.1415927

using namespace std;
class ScatterSample
{
    public:
        double A[MaxLen];
        double E[MaxLen];
        double I[MaxLen];
        int size;

        double Prob[MaxLen];
        double ProbCum[MaxLen];
        double bandwidth[3];

        std::mt19937 MutiSeed[MaxThread];

        ScatterSample(const char * name,int head);
        double GetSample(double * Vel,int tid);
        void randomPosition(double * vec,double Norm,int tid);
        double SetMoonPosition(double * pos,int tid);
        void Bandwidth();
};

void Over(FILE * fp,int n);
// int ReadCsv(const char* name,int head);

#endif // _SAMPLING_H_
