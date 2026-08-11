#pragma once //header guard(헤더가드) : 중복 정의 오류 방지

#ifndef CYCLONE_PRECISION_H
#define CYCLONE_PRECISION_H

#include <float.h>
#include <math.h>
#include <cmath>

namespace cyclone
{
    #if 0 // 단지 정밀도를 뭘로 할지 편하게 하기 위해 만들어 둔 스위치임. 1로 바꾸면 단정밀도(float), 0으로 하면 배정밀도(double)
   
    #define SINGLE_PRECISION
    typedef float real;

    /** Defines the highest value for the real number. */
    #define REAL_MAX FLT_MAX

    /** Defines the precision of the square root operator. */
    #define real_sqrt sqrtf
    /** Defines the precision of the absolute magnitude operator. */
    #define real_abs fabsf
    /** Defines the precision of the sine operator. */
    #define real_sin sinf

    /** Defines the precision of the cosine operator. */
    #define real_cos cosf

    /** Defines the precision of the exponent operator. */
    #define real_exp expf
    /** Defines the precision of the power operator. */
    #define real_pow powf

    /** Defines the precision of the floating point modulo operator. */
    #define real_fmod fmodf
    
    /** Defines the number e on which 1+e == 1 **/
    #define real_epsilon FLT_EPSILON

    #define R_PI 3.14159f
    //정밀도가 바뀌게 될 경우를 대비하여 다 재정의 해준다.
    #else
    #define DOUBLE_PRECISION
    typedef double real;
    #define REAL_MAX DBL_MAX
    #define real_sqrt sqrt
    #define real_abs fabs
    #define real_sin sin
    #define real_cos cos
    #define real_exp exp
    #define real_pow pow
    #define real_fmod fmod
    #define real_epsilon DBL_EPSILON
    #define R_PI 3.14159265358979
#endif
}

#endif // CYCLONE_PRECISION_H