#include "Sampling.h"


void Over(FILE * fp,int n)
{
    char buf[1024];
    for(int k=0;k<n;k++)
    {
        fgets(buf,sizeof(buf),fp);
        fprintf(stdout,"%s\n",buf);
    }
}

// int ReadCsv(const char* name,int head)
// {
//     int LEN = 0;
//     memset(A,0,MaxLen*sizeof(double));
//     memset(E,0,MaxLen*sizeof(double));
//     memset(I,0,MaxLen*sizeof(double));

//     FILE * fp = fopen(name,"r");
//     if(NULL==fp)
//     {
//         fprintf(stdout,"Can not open Csv file!\n");
//         return 0;
//     }
//     else
//     {
//         Over(fp,head);
//         double a,e,i;
//         while(EOF!=fscanf(fp,"%lf %lf %lf",&a,&e,i))
//         {
//             A[LEN] = a;
//             E[LEN] = e;
//             I[LEN] = i;
//             LEN ++;
//         }

//         if(fp!=NULL)
//         {
//             fclose(fp);
//             return LEN;
//         }
//     }
    
//     return 0;
// }

ScatterSample::ScatterSample(const char * name,int head)
{
    size = 0;
    memset(A,0,MaxLen*sizeof(double));
    memset(E,0,MaxLen*sizeof(double));
    memset(I,0,MaxLen*sizeof(double));

    std::mt19937 gen((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    for(int k=0;k<Numthread;k++)
    {
        MutiSeed[k] = std::mt19937(gen());
    }

    FILE * fp = fopen(name,"r");
    if(NULL==fp)
    {
        fprintf(stdout,"Can not open Csv file!\n");
    }
    else
    {
        Over(fp,head);
        double a,e,i;
        double con1,con2,M;
        double Ux,Uy,Uz,U,ctga;

        double Sum = 0;
        int k =0;

        
        while(EOF!=fscanf(fp,"%lf %lf %lf",&a,&e,&i) && k<MaxLen)
        {
#ifdef _DEBUG
        if((k++)%100==1)
            fprintf(stdout,"%d %f %f %f\n",k,a,e,i);
#endif
            con1 = 1.0 - 1.0 / (a* (1 + e));
            con2 = 1.0 / (a * (1 - e)) - 1;
            if(con1 * con2 >= 0)
            {
                i = _PI_*i/180.0;
                A[size] = a;
                E[size] = e;
                I[size] = i;
                ctga = sqrt(con1*con2);
                M = sqrt(a*(1-e*e));

                Ux = M*ctga;
                Uy = M*cos(i) - 1.0;
                Uz = M*sin(i);
                U  = sqrt(3.0 - 1.0/a - 2.0*M*cos(i));
                Prob[size] = U/sqrt(Ux*Ux + Uz*Uz);
                Sum += Prob[size];
                ProbCum[size] = Sum;
                size ++;
            }
        }

        if(fp!=NULL)
        {
            fclose(fp);
        }
    }

}

double ScatterSample::GetSample(double * Vel,int tid)
{
    static std::normal_distribution<double> DistNorm(0,1.0);
    static std::uniform_real_distribution<double> UniDist(0.0,ProbCum[size-1]);
    static std::bernoulli_distribution BerDist(0.5);
    double a,e,i;
    double con1,con2;

    while(1)
    {
        double u = UniDist(MutiSeed[tid]);
        int L = 0;
        int R = size - 1;
        int mid = 0;
        if(u<ProbCum[0])
        {
            L = 0;
        }
        else
        {
            while(L+1<R)
            {
                mid = int (L+R)/2;
                if(ProbCum[L] <= u && u< ProbCum[mid])
                {
                    R = mid;
                }
                else
                {
                    L = mid;
                }
            }
        }
        
        
#ifdef _DEBUG
    fprintf(stdout,"%f %d: %f %d: %f %f\n",u,L,ProbCum[L],R,ProbCum[R],Prob[R]);
#endif
        a = DistNorm(MutiSeed[tid]) * bandwidth[0] + A[L];
        e = DistNorm(MutiSeed[tid]) * bandwidth[1] + E[L];
        i = DistNorm(MutiSeed[tid]) * bandwidth[2] + I[L];

        con1 = 1.0 - 1.0 / (a* (1 + e));
        con2 = 1.0 / (a * (1 - e)) - 1;

// #ifdef _DEBUG
//     fprintf(stdout,"%f %f %f\n",a,e,i);
// #endif
        if(con1 >= 0 && con2>=0 && e <= 1.0 && e>=0 && i<= 0.5*_PI_ && i>= 0) break;

    } 

    double ctga = sqrt(con1*con2);
    double M   = sqrt(a*(1-e*e));
    Vel[0] = M*ctga;
    Vel[1] = M*cos(i) - 1.0;
    Vel[2] = M*sin(i);
    
    if(BerDist(MutiSeed[tid]))
    {
        Vel[0] *= -1;
    //    Vel[2] *= -1;
    }
    if(BerDist(MutiSeed[tid]))
    {
	    Vel[2] *= -1;
    }
    return 3.0 - 1.0/a -2.0*M*cos(i);

}

double ScatterSample::SetMoonPosition(double * pos,int tid)
{
    static std::uniform_real_distribution<double> UniDist(0.0,1.0);
    double angle = UniDist(MutiSeed[tid])*2.0*_PI_;
    pos[0] = MoonEarth*sin(angle);
    pos[1] = MoonEarth*cos(angle);
    pos[2] = 0.0;
    return angle;
}

void ScatterSample::randomPosition(double * vec,double Norm,int tid)
{
    static std::normal_distribution<double> normal(0,1);
    double V = 0;
    for(int k=0;k<DIM;k++)
    {
        vec[k] = normal(MutiSeed[tid]);
        V += vec[k]*vec[k];
    }

    V = sqrt(V);
    for(int k=0;k<DIM;k++)
    {
        vec[k] *= Norm/V;
    } 
}

void ScatterSample::Bandwidth()
{
    double SumA,SumE,SumI;
    double S2A,S2E,S2I;

    SumA = SumE = SumI = 0.0;
    
    for(int k=0;k<size;k++)
    {
        SumA += A[k];
        SumE += E[k];
        SumI += I[k];
    }

    SumA *= 1.0/size;
    SumE *= 1.0/size;
    SumI *= 1.0/size;

    S2A = S2E = S2I = 0.0;

    for(int k=0;k<size;k++)
    {
        S2A += (A[k] - SumA)*(A[k] - SumA);
        S2E += (E[k] - SumE)*(E[k] - SumE);
        S2I += (I[k] - SumI)*(I[k] - SumI); 
    }

    bandwidth[0] = pow(size,-0.2)*1.06*sqrt(S2A/(size -1.0));
    bandwidth[1] = pow(size,-0.2)*1.06*sqrt(S2E/(size -1.0));
    bandwidth[2] = pow(size,-0.2)*1.06*sqrt(S2I/(size -1.0));

}

