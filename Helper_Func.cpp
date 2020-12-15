#include <math.h>
#include<iostream>
#include <utility>
#include "Helper_Func.hpp"

double factorial(int16_t n){
    // calculates n! = n*(n-1)*(n-2).....*1
    double temp = 1;
    for(int16_t i = 1;i<=n;i++){
        temp*=i;
    }
    return temp;
}
double poisson_pdf(int16_t mu, int16_t x){
    double num = exp(-mu)*pow(mu, x);
    double den = factorial(x);
    return num/den;
}