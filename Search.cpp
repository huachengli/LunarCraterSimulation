#include "Search.h"

using namespace std;

double EarthPosition[3] = {0.0,0.0,0.0};
double MoonPosition[3]  = {MoonEarth,0.0,0.0};
double MoonVelocity[3]  = {0.0,0.0,0.0};

double ParticleVelocity[3];
double ParticlePosition[3];
double TestAcceleration[3];
clock_t STime;

double TestVelocity = minVelocity;
double h            = 0.01*MoonEarth/TestVelocity;
int CaseNum         = 0;
double kde[100];

#ifdef Orbit_CHECK
fstream   OUTc("OUTc",ios::out);
#endif

int SearchParticle(int Numcase,const string & outE,const string & outM)
{
    int iMoon = 0,iEarth = 0,iEscape = 0,iOther = 0;
    omp_set_num_threads(Numthread);
    omp_lock_t lock;
    omp_init_lock(&lock);

    fstream OUT1(outE,ios::out|ios::app);
    fstream OUT2(outM,ios::out|ios::app);
    
    // MutiThreadRandom MyRandomTable(Numthread,"kde-2d.csv",90,91);
    char name[20] = "particle-OPIK.data";
    ScatterSample MyRandomTable(name,7);
    MyRandomTable.Bandwidth();

    cout << "######" << "CASE::" << Numcase << "#####" << endl;
    getTime();
    if(!OUT1 || !OUT2)
    {
        cout << "fail to open the file" <<endl;
        return Threshold;
    }
    else
        cout << "open the file successfully!" << endl;

    #pragma omp parallel for reduction(+: iMoon,iEarth,iEscape),firstprivate(h,TestAcceleration,ParticleVelocity,ParticlePosition,MoonPosition)
    for(int i=0;i<pass;i++)
    {
        // 获取当前线程号
        int tid = omp_get_thread_num();
        
        // 利用已知数据采样
        double theta0 = MyRandomTable.SetMoonPosition(MoonPosition,tid);
        ObjectBase Earth(EarthMass,EarthRadiu,EarthPosition,MoonVelocity,0);
        Satellite  Moon(Earth,MoonMass,MoonRadiu,MoonPosition,MoonVelocity,0,MoonTheta,theta0);

        // double Vel = MyRandomTable.Sampling(tmp);
        // tmp.cpy(ParticleVelocity);

        double Vel = MyRandomTable.GetSample(ParticleVelocity,tid);
        //
        // 减去地球运动速度是为了
        // 下面的计算将以地球中兴位参照点
        // particle 的运动方向和地球运动方向
        // * 在赤道面上投影的夹角不大于 0.025*M_PI * -删去-
        // * 这是一个假设，还没有相关数据证明 *
        //

        MyRandomTable.randomPosition(ParticlePosition,TestDistance,tid);
        

        Vel *= EarthVel;
        for(int k=0;k<DIM;k++)
        {
            ParticleVelocity[k] *= EarthVel;
        }
    
        // 初始迭代步长设置
        h = 0.001*MoonEarth/Vel;

        checkVelocity(ParticlePosition,ParticleVelocity);
        Particle   Test(1,1,ParticlePosition,ParticleVelocity,0);

        for(int k = 0; k < maxStep; k++)
        {
            double temp = min(Moon.getDistance(Test.position),Earth.getDistance(Test.position));
            if(temp >= 1.5*MoonEarth)
            {
                if (h * Test.getVelocity() > 0.02 * MoonEarth)
                    h *= hbeta;
                else
                    h *= 1.0 / hbeta;
            }
            else
            {
                if (h * Test.getVelocity() > 0.1*MoonRadiu)
                    h *= hbeta;
                else
                    h *= 1.0 / hbeta;
            }


            accel(Earth, Moon, Test, TestAcceleration);
            Moon.move(Earth, h);
            Test.move(TestAcceleration, h);

#ifdef Orbit_CHECK
            if(Numcase==0 && Orbit_out==i)
            {
                for(int j=0;j<DIM;j++) OUTc << Test.position[j] << ",";
                for(int j=0;j<DIM;j++) OUTc << Moon.position[j] << ",";
                OUTc << endl;
            }
#endif

            if(Earth.IMPACT(Test.position, Test.velocity))
            {
                //cout << "Impact On earth at " << Test.clock << endl;

                iEarth ++;
                omp_set_lock(&lock);
                OUT1 << k << ",";

                for(int j=0;j<DIM;j++) OUT1 << ParticleVelocity[j] << ",";
                for(int j=0;j<DIM;j++) OUT1 << ParticlePosition[j] << ",";
                for(int j=0;j<DIM;j++) OUT1 << Test.position[j] << ",";

                OUT1 << Test.clock << endl;
                omp_unset_lock(&lock);

                break;

            }
            else if (Moon.IMPACT(Test.position, Test.velocity))
            {
                //cout << "Impact On Moon at " << Test.clock << endl;
                iMoon ++;
                omp_set_lock(&lock);

                OUT2 << k << ",";
                for(int j=0;j<DIM;j++) OUT2 << ParticleVelocity[j] << ",";
                for(int j=0;j<DIM;j++) OUT2 << ParticlePosition[j] << ",";
                for(int j=0;j<DIM;j++) OUT2 << Test.position[j] << ",";
                for(int j=0;j<DIM;j++) OUT2 << Test.velocity[j] << ",";
                for(int j=0;j<DIM;j++) OUT2 << Moon.velocity[j] << "," ;
                for(int j=0;j<DIM;j++) OUT2 << Moon.position[j] << "," ;
                OUT2 << Test.clock << endl;
                omp_unset_lock(&lock);

                break;
            }
            else if (Earth.getDistance(Test.position) > 50 * MoonEarth)
            {
                //cout << "Test out of 0-50 MoonEarth Scale!!!" << endl;
                iEscape ++;
                break;
            }
        }

    }

    OUT1.close();
    OUT2.close();
    omp_destroy_lock(&lock);

    Out( iEarth);
    Out( iMoon);
    Out( iEscape);
    iOther = pass-iEarth-iMoon-iEscape;
    Out( iOther);

    Out((clock()-STime)/CLOCKS_PER_SEC);
    cout << endl;
    return iMoon;
}


void accel(ObjectBase & E,Satellite & M,Particle & T,double * acc)
{
    double temp1[DIM],temp2[DIM];
    E.acceleration(T.position,temp1);
    M.acceleration(T.position,temp2);
    for(int k=0;k<DIM;k++)
    {
        acc[k] = temp1[k] + temp2[k];
    }
}

void checkVelocity(double * _p,const double * _v)
{
    double _c = 0;
    for(int k=0;k<DIM;k++) _c += _p[k]*_v[k];

    if(_c>=0)
    {
        for(int k=0;k<DIM;k++)  _p[k] = -_p[k];
    }
}

int getTime()
{
    int ret = -1;
    struct timeval stuTimeVal;
    memset(&stuTimeVal,0,sizeof(struct timeval));
    ret = gettimeofday(&stuTimeVal,NULL);
    if(0 == ret)
    {
        printf("\n tv_sec[%lu] tv_usec[%lu]\n",stuTimeVal.tv_sec , stuTimeVal.tv_usec);
    }
    return 0;
}

