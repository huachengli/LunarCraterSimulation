#include "Sampling.h"
#include <iostream>

int main()
{
    char name[20] = "particle-OPIK.data";
// #ifdef _DEBUG
//     fprintf(stdout,"%s\n",name);
// #endif
    ScatterSample particleOPIK(name,8);
    particleOPIK.Bandwidth();
    fprintf(stdout,"%d\n",particleOPIK.size);
    for(int k=0;k<3;k++)
    {
        fprintf(stdout,"%f\n",particleOPIK.bandwidth[k]);
    }

    double Vel[4];

    for(int k=0;k<20;k++)
    {
        particleOPIK.GetSample(Vel,12);
        // fprintf(stdout,"")
    }

    // for(int k=500;k<900;k+= 4)
    // {
    //     fprintf(stdout,"%f %f %f %f \n",particleOPIK.Prob[0+k],particleOPIK.Prob[1+k],particleOPIK.Prob[2+k],particleOPIK.Prob[3+k]);
    // }

    for(int k=0;k<particleOPIK.size;k++)
    {
        if(particleOPIK.Prob[k]>2.0)
            fprintf(stdout,"%f \n",particleOPIK.Prob[k]);
    }
    return 0;
}