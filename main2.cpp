#include "Search.h"
#include <string>
#include "param.h"
// 仅仅做启动程序用
// 具体参数设置： param.h
// 
using namespace std;
int main()
{
    string ename("outEarth.txt");
    string mname("outMoon.txt");

    int Sum = 0;
    int Case = 0;

    while(Sum < Threshold)
    {
        Sum += SearchParticle(Case++,ename,mname);
    }
    return 0;
}
