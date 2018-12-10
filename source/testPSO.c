#include "PSO.h"

#define MAX_STEP 30000

#define DIMENSION 10

#define NUM 100

//
#define X_MIN -5.12

#define X_MAX  5.12
//


double sphere(double *x, int n){
  double ret = 0.0;
  int    i;

  for(i = 0; i < n; i++)
    ret += x[i] * x[i];

  return ret;
}


void doAllProcess(Population *p){
  int t = 0, j;

  initAll(p, NUM, DIMENSION, sphere);
  printf("1\n");

  printf("gbest_value : %lf\n", p->gbest_value);

  while(t++ < MAX_STEP){
    updateV(p);

    updateX(p);

    updatePbest(p, sphere);

    updateGbest(p, sphere);

    printf("Step : %d\n", t);
  }

  printf("GBest Value : %lf\n", p->gbest_value);

  printf("GBest:");
  for(j = 0; j < DIMENSION; j++)
    printf("[%d] : %lf\n", j, p->gbest[j]);

  termAll(p);
}


int main(void){
  Population population;

  doAllProcess(&population);

  return 0;
}

