#include "zpid.h"
#include "platform.h"

#define abs(x) ((x)>0?(x):(-(x)))

float zpid(zpid_t* p, float sp, float pv)
{
    float err = sp - pv;
    float sum_err;
//    float output;

//    if(abs(err) < p->lim_err)         //
    if((abs(p->sum_err * p->ki) > p->lim_sum_err) && (p->sum_err * err > 0)) {
        sum_err = 0;
        // saturated, do nothing
    }
    else {
        p->sum_err += err * p->inteval;
        sum_err = p->sum_err;
    }
    float d = (err - p->prev_err) / p->inteval;
    p->prev_err = err;

    return p->kp * err + p->ki * sum_err + p->kd * d;
}
