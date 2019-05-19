//
// Created by huacheng on 2019/3/4.
//

#ifndef SYSTEM_2BODY_ELEMET_H
#define SYSTEM_2BODY_ELEMET_H
#include "param.h"
#include <math.h>
#define _USE_MATH_DEFINES

// ObjectBase 用于描述处于参考系中心静止不动的行星
// Satellite  派生自ObjectBase用于轨道基本确定的卫星
// Particle   派生自ObjectBase用于描述轨道需要仔细计算的陨石


class ObjectBase
{
public:
    double mass;
    double radiu;
    double position[DIM]{};
    double velocity[DIM]{};
    double clock;

    ObjectBase(const double _m, const double _r, const double *_p, const double * _v,const double _t): \
    mass(_m),radiu(_r),clock(_t)
    {
        for(int k=0;k<DIM;k++)
        {
            position[k] = _p[k];
            velocity[k] = _v[k];
        }
    }


    void acceleration(const double * _p,double * _acc)
    {
        double dis = 0;
        double vec[3];
        for(int k=0;k<DIM;k++)
        {
            vec[k] = - _p[k] + position[k];
            dis += vec[k]*vec[k];
        }
        dis = sqrt(dis*dis*dis);
        for(int k=0;k<DIM;k++)
        {
            _acc[k] = G*mass*vec[k]/dis;
        }
    };

    double getVelocity()
    {
        double V = 0;
        for(int k=0;k<DIM;k++)
        {
            V += velocity[k]*velocity[k];
        }
        return sqrt(V);
    }

    double getDistance(const double * _p)
    {
        double dis = 0;
        for(int k=0;k<DIM;k++)
        {
            dis += (_p[k] - position[k])*(_p[k]-position[k]);
        }
        return sqrt(dis);

    }

    int IMPACT(const double * _p,const double * _v)
    {
        double dis = 0;
        for(int k=0;k<DIM;k++)
        {
            dis += (_p[k] - position[k])*(_p[k]-position[k]);
        }
        return sqrt(dis)<=radiu;
    }
};


class Particle: public ObjectBase{
public:

    Particle(const double _m, const double _r, const double *_p, const double *_v, const double _t) : \
    ObjectBase(_m, _r, _p, _v, _t) {}

    void move(const double *_acceleration, const double h)
    {
        // 这里仅仅采用了一阶的前向欧拉方法
        //
        clock += h;
        for (int k = 0; k < DIM; k++)
        {
            position[k] += 0.5 * h * velocity[k];
            velocity[k] += h * _acceleration[k];
            position[k] += 0.5 * h * velocity[k];
        }
    }

};

class Satellite: public ObjectBase
{
public:
    double theta1;
    double theta2;
    double orbit;

    Satellite(ObjectBase & S, double _m, const double _r, const double *_p, const double *_v, const double _t,double _theta1,double _theta2) : \
    ObjectBase(_m, _r, _p, _v, _t),theta1(_theta1),theta2(_theta2){
        orbit = 0.0;
        for(int k=0;k<DIM;k++)
        {
            orbit += (S.position[k]-_p[k])*(S.position[k]-_p[k]);
        }
        orbit = sqrt(orbit);

        position[0] = S.position[0] + orbit*cos(theta2);
        position[1] = S.position[1] + orbit*sin(theta2);
        position[2] = S.position[2];
    };

    void move(const ObjectBase & S,double  h)
    {
        theta2 += theta1*h;

        position[0] = S.position[0] + orbit*cos(theta2);
        position[1] = S.position[1] + orbit*sin(theta2);
        position[2] = S.position[2];

        velocity[0] = - orbit*theta1*sin(theta2);
        velocity[1] =   orbit*theta1*cos(theta2);
        velocity[2] = 0;

        clock += h;
    };

};


#endif //SYSTEM_2BODY_ELEMET_H
