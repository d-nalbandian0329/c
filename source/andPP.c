#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Number of unit.
#define NUM 2

// Learning loop counts + 1.
#define L_NUM 100

// Threshold.
double THETA = 0.0;
double ETA   = 0.0;



// Return uniform random numbers. [0,1]
static double uniform();

// Initiaize each parameter.
static void init(double *, double *);

// Stack input data and teacher signal to buffer.
static void inputData(const char *, double [L_NUM][NUM+1]);

// Receive input data from buffer.
static void setData(double [L_NUM][NUM+1], int, double *, double *);

// Activate function.
static int activateFunc(double);

// Update synaptic weight.
static void update(double *, double *, double, double);

// Do learning.
static void learning(double *,double *, double, double);


// Test network.
static void test();


// Do all process.
void doAllProcess();



int main(void){
  srand((unsigned int)time(NULL));rand();

  doAllProcess("/Users/iwaitoshiya/u226136/AND_TestData.csv");

  return 0;
}



// Return uniform random numbers. [0,1]
static double uniform(){
  return (rand()+1.0) / (RAND_MAX+1.0);
}

// Initiaize synaptic weight.
static void init(double *x, double *w){
  int i;

  x[NUM-1] = 1;

  THETA = uniform();

  for(i = 0; i < NUM; i++)
    w[i] = uniform();

  w[NUM] = -THETA;

  ETA = 0.3 + uniform() * 0.2;
}

// Recieve input data and teacher signal.
static void inputData(const char *fname, double buff[L_NUM][NUM+1]){
  FILE *fp;
  int  i;

  if((fp = fopen(fname, "r")) == NULL){
    printf("File Not Found!!\n");
    exit(EXIT_FAILURE);
  }

  printf("B_sscanf\n");
  i = 0;
  while(fscanf(fp, "%lf,%lf,%lf", &buff[i][0], &buff[i][1], &buff[i][2]) != EOF)
    ++i;

  printf("A_sscanf\n");

  fclose(fp);
}

// Receive input data from buffer.
static void setData(double buff[L_NUM][NUM+1], int idx, double *x, double *t){
  int i;

  printf("BA_idx%d\n", idx);
  for(i = 0; i < NUM; i++)
    x[i] = buff[idx][i];

  *t = buff[idx][i];

  printf("SA_%d\n", idx);
}

// Activate function.
static int activateFunc(double sum){
  return (sum >= 0.000) ? 1 : -1;
}

// Update synaptic weight.
static void update(double *x, double *w, double t, double sum){
  int i;

  for(i = 0; i < NUM; i++)
    w[i] += ETA * (t - sum) * x[i];
}

// Do learning.
void learning(double *x, double *w, double t, double sum){
  if(activateFunc(sum)==-1)
    update(x, w, t, sum);
}

// Test network.
static void test(double *w){
  int result = 0;
  int ans;

  printf("\n----- Result -----\n");

  result = activateFunc(0*w[0] + 0*w[1] + 1*w[2]);
  ans = (result==1 ? 1 : 0);
  printf("%d AND %d : %d\n", 0, 0, ans);

  result = activateFunc(0*w[0] + 1*w[1] + 1*w[2]);
  ans = (result==1 ? 1 : 0);
  printf("%d AND %d : %d\n", 0, 1, ans);

  result = activateFunc(1*w[0] + 0*w[1] + 1*w[2]);
  ans = (result==1 ? 1 : 0);
  printf("%d AND %d : %d\n", 1, 0, ans);

  result = activateFunc(1*w[0] + 1*w[1] + 1*w[2]);
  ans = (result==1 ? 1 : 0);
  printf("%d AND %d : %d\n", 1, 1, ans);

  printf("w1 : %lf\nw2 : %lf\n", w[0], w[1]);

  printf("THETA : %lf\n", THETA);
  printf("ETA   : %lf\n", ETA);
}

// Do all process.
void doAllProcess(const char *fname){
  double x[NUM+1];
  double w[NUM+1];
  double buff[L_NUM][NUM+1];
  double t;
  int i, j;

  init(x, w);

  inputData(fname, buff);

  for(i = 0; i < L_NUM; i++){
    double sum = 0.0;

    setData(buff, i, x, &t);

    for(j = 0; j < NUM+1; j++)
      sum += w[i] * x[i];

    learning(x, w, t, sum);
  }

  test(w);
}
