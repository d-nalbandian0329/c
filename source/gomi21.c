#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "testPSO2.h" 

//Function
// Sphere function.  [-5.12, 5.12]
extern double sphere(double *p, int n);

// Rastrigin function.  [-5.0, 5.0]
extern double rastrigin(double *p, int n);


// Rosenbrock function.  [-2.048, 2.048]
extern double rosenbrock(double *p, int n);

/*
// A pointer to a function that has two arguments and retuning double.
typedef double (*FUNC_PTR)(double *, int);
enum FUNC{SPHERE, RASTRIGIN, ROSENBROCK};

FUNC_PTR func_ptr_ary[] = {sphere, rastrigin, rosenbrock};
*/

void test(void){
  double ary1[] = {0, 0, 0, 0};
  double ary2[] = {0, 0};
  double ary3[] = {1, 1, 1, 1, 1, 1, 1};

  printf("sphere_min     : %f\n", (func_ptr_ary[SPHERE])(ary1, 4));
  printf("rastrigin_min  : %f\n", (func_ptr_ary[RASTRIGIN])(ary2, 2));
  printf("rosenbrock_min : %f\n", (func_ptr_ary[ROSENBROCK])(ary3, 7));
}

int main(void){

  test();

  return 0;
}

