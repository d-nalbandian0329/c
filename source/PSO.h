// Particle Swarm Optimization header file.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "MT.h"


#define W  0.7298

#define C1 1.49445

#define C2 1.49445



#define MEMORY_ERROR_MESSAGE "OutOfMemoryException!!\n"

#define exitEXE(str) printf(str),exit(EXIT_FAILURE)

#define def_calloc(ptr,cast,num,size) \
  do{\
    if((ptr=(cast)calloc(num,size))==NULL)\
      exitEXE(MEMORY_ERROR_MESSAGE);\
  }while(0)

// [0,1]
#define uniformMT() genrand_real1()


typedef struct particle_type{
  double *v;
  double *x;
  double *pbest;
  double pbest_value;
}Particle;


typedef struct population_type{
  Particle *particle;
  double   *gbest;
  double   gbest_value;
  int num;
  int dimension;
}Population;



void initAll(Population *population, int num, int dimension, double (*func)(double *, int));

void initParticle(Population *population, int i, int dimension, double (*func)(double *, int));

void termAll(Population *population);

void termParticle(Particle *particle);


void updateV(Population *population);

void updateVi(Population *population, int i);

void updateVij(Population *population, int i, int j);


void updateX(Population *population);

void updateXi(Population *population, int i);

void updateXij(Population *population, int i, int j);


void updatePbest(Population *population, double (*func)(double *, int));

void updatePbesti(Population *populaiton, int i, double (*func)(double *, int));


void updateGbest(Population *population, double (*func)(double *, int));


//---------------------------------------------------



//
// @Para1 : 
// @Para2 : 
// @Para3 : 
void initAll(Population *population, int num, int dimension, double (*func)(double *, int)){
  int i, j;

  init_genrand(time(NULL));
  uniformMT();

  population->num       = num;
  population->dimension = dimension;

  def_calloc(population->particle, Particle *, population->num, sizeof(Particle));

  for(i = 0; i < num; i++)
    initParticle(population, i, dimension, func);

  i = (int)(uniformMT() * (num - 1));


  def_calloc(population->gbest, double *, dimension, sizeof(double));
  for(j = 0; j < dimension; j++)
    population->gbest[j] = population->particle[i].pbest[j];

  population->gbest_value = (*func)(population->gbest, dimension);
}

void initParticle(Population *population, int i, int dimension, double (*func)(double *, int)){
  double L = X_MAX - X_MIN;
  double restriction = 0.2;
  int    j;

  def_calloc(population->particle[i].v, double *, dimension, sizeof(double));
  for(j = 0; j < dimension; j++){
    population->particle[i].v[j] = uniformMT() * (2*L*restriction) - (L*restriction);
    printf("v : %lf\n", population->particle[i].v[j]);
  }

  def_calloc(population->particle[i].x, double *, dimension, sizeof(double));
  for(j = 0; j < dimension; j++){
    population->particle[i].x[j] = (X_MAX-X_MIN) * uniformMT() + X_MIN;
    printf("x : %lf\n", population->particle[i].x[j]);
  }

  def_calloc(population->particle[i].pbest, double *, dimension, sizeof(double));
  for(j = 0; j < dimension; j++){
    population->particle[i].pbest[j] = population->particle[i].x[j];
    printf("pbest : %lf\n", population->particle[i].pbest[j]);
  }

  population->particle[i].pbest_value = (*func)(population->particle[i].pbest, j);
}



void termAll(Population *population){
  int i;

  for(i = 0; i < population->num; i++)
    termParticle(&(population->particle[i]));

  free(population->particle);
  free(population->gbest);

  population->num = population->dimension = 0;
  population->gbest_value = 0.0;
}

void termParticle(Particle *particle){
  free(particle->v);
  free(particle->x);
  free(particle->pbest);

  particle->v = particle->x = particle->pbest = NULL;
  particle->pbest_value = 0.0;
}


void updateV(Population *population){
  int i;

  for(i = 0; i < population->num; i++)
    updateVi(population, i);
}

void updateVi(Population *population, int i){
  int j;

  for(j = 0; j < population->dimension; j++)
    updateVij(population, i, j);
}

void updateVij(Population *population, int i, int j){
  double vij = population->particle[i].v[j];
  double xij = population->particle[i].x[j];
  double pbestij = population->particle[i].pbest[j];
  double gbestj  = population->gbest[j];

  //printf("vij(t) : %lf\n", vij);

  population->particle[i].v[j]
    = W * vij + C1*uniformMT()*(pbestij-xij) + C2*uniformMT()*(gbestj-xij);

  //printf("vij(t+1) : %lf\n", population->particle[i].v[j]);
}


void updateX(Population *population){
  int i;

  for(i = 0; i < population->num; i++)
    updateXi(population, i);
}

void updateXi(Population *population, int i){
  int j;

  for(j = 0; j < population->dimension; j++)
    updateXij(population, i, j);
}

void updateXij(Population *population, int i, int j){
  double vij = population->particle[i].v[j];
  double xij = population->particle[i].x[j];

  population->particle[i].x[j] = xij + vij;

  // Absorb.
  if(population->particle[i].x[j] > X_MAX)
    population->particle[i].x[j] = X_MAX;

  else if(population->particle[i].x[j] < X_MIN)
    population->particle[i].x[j] = X_MIN;
}



void updatePbest(Population *population, double (*func)(double *, int)){
  int i;

  for(i = 0; i < population->num; i++)
    updatePbesti(population, i, func);
}

void updatePbesti(Population *population, int i, double (*func)(double *, int)){
  double value = (*func)(population->particle[i].x, population->dimension);
  int j;

  if(population->particle[i].pbest_value > value){
    for(j = 0; j < population->dimension; j++)
      population->particle[i].pbest[j] = population->particle[i].x[j];

    population->particle[i].pbest_value = value;
  }
}


void updateGbest(Population *population, double (*func)(double *, int)){
  double *p = NULL;
  int    i, j;

  for(i = 0; i < population->num; i++){
    if(population->gbest_value > population->particle[i].pbest_value)
      p = &(population->particle[i].pbest[i]);
  }

  if(p){
    for(j = 0; j < population->dimension; j++)
      population->gbest[j] = p[j];

    population->gbest_value = (*func)(p, population->dimension);
  }
}

