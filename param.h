//
// Created by huacheng on 2019/3/4.
//

#ifndef SYSTEM_2BODY_UNIT_H
#define SYSTEM_2BODY_UNIT_H
#include <cmath>
// 基本物理参数
#define DIM 3
#define G 6.67e-11
#define SunMass 1.989e30      // kg
#define SunEarth 1.496e11 // m
#define AU 1.496e11       // m


//地月系基本轨道参数
#define EarthVel     29800.0
#define MoonEarth    3.844e8
#define MoonMass     7.349e22
#define EarthMass    5.965e24
#define EarthRadiu   6.371e6
#define MoonRadiu0    1.738e6
#define MoonTheta0    2*3.141516/(60*60*24*27.32)
//运行时刻参数

#define hbeta        0.95              // 时间步长变化速率
#define maxStep      1000000           // 迭代最大步数
// #define TestDistance 4*MoonEarth      // 陨石距离地球初始距离
#define minVelocity  50.0           // 陨石最小相对速度 参数废弃
#define maxVelocity  40000.0          // 陨石最大相对速度 参数废弃
#define dv           2000.0          // 陨石速度间隔 参数废弃
#define Numthread    160             // 使用线程个数
#define pass         1500000         // 每个case计算陨石颗数
#define Threshold    20000           // 搜索上限
#define Orbit_CHECK  0
#define Orbit_out    100


#define B1 1.32442894
#define B2 8.51306745
#define N1 1.71543008
#define N2 0.63325449

#define L1 0.3
#define R1 4.0

const double TestDistance= 5.0*MoonEarth;
const double MoonRadiu = 1.0*MoonRadiu0;
const double MoonTheta = MoonTheta0 * pow(MoonRadiu0/MoonRadiu,1.5);
#endif //SYSTEM_2BODY_UNIT_H
