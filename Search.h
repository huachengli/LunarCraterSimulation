#ifndef _SYSTEM_2BODY_SEARCH_H
#define _SYSTEM_2BODY_SEARCH_H
#include <iostream>
#include <cmath>
#include <omp.h>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <memory.h>
#include <unistd.h>
#include <sys/time.h>


#include "element.h"
#include "Sampling.h"
#include "param.h"

#define Name(X) #X
#define Out(X) {string xname=Name(X);cout<<xname.substr(0,xname.size())<<": "<<X<<endl;}

using std::string;
void accel(ObjectBase & E,Satellite & M,Particle & T,double * acc);
void checkVelocity(double * _p,const double * _v);
int getTime();
int SearchParticle(int Numcase,const string & outE,const string & outM);
#endif 