// Linear separate datas by Perceptron.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Repeat count of learning.
#define NUM 400


// Return uniform random number.
static double uniform();

// Initialize coefficient.
// @Para : A pointer to coefficient array.
static void init(double *);

// Function f.
// @Para1 : A pointer to coefficient array.
// @Para2 : Xn
// @Para3 : Yn
static int f(double *, double, double);

// Dynamic allocate.
static void getMemory(double **, int);

// x, y, t
static int readCSV(const char *, double **, double **, double **);

// Update coefficient.
static void update(double *, double *);

// Linearly separate by Perceptron.
// @Para : A pointer to coefficient array.
void p_learning(double *);

// Show result.
// @Para : A pointer to coefficient array.
static void showResult(double *);


double ce[3] = {0.0};



int main(void){
  p_learning(ce);

  showResult(ce);

  return 0;
}



// Return uniform random number. [0, 1]
static double uniform(){
  return (rand()+1.0) / (RAND_MAX+1.0);
}


// Initialize coefficient.
static void init(double *ce){
  srand((unsigned int)time(NULL));rand();

  ce[0] = uniform() * 20 - 10;
  ce[1] = uniform() * 20 - 10;
  ce[2] = uniform() * 20 - 10;
}

// Function f.
static int f(double *ce, double x, double y){
  return ((ce[0]*x + ce[1]*y + ce[2])>=0 ? 1 : -1);
}

static void getMemory(double **p, int n){
  if((*p = (double *)calloc(n, sizeof(double))) == NULL){
    printf("Memory Error!!\n");
    exit(EXIT_FAILURE);
  }
}

static int readCSV(const char *fname, double **x, double **y, double **t){
  FILE   *fp;
  double buff[3] = {0.0};
  int    size, i;

  if((fp = fopen(fname, "r")) == NULL){
    printf("File not found!!\n");
    exit(EXIT_FAILURE);
  }

  // CSV file header means row counts.
  fscanf(fp, "%d", &size);

  getMemory(x, size);
  getMemory(y, size);
  getMemory(t, size);

  i = 0;
  while(fscanf(fp, "%lf,%lf,%lf", &buff[0], &buff[1], &buff[2]) != EOF){
    (*x)[i] = buff[0];
    (*y)[i] = buff[1];
    (*t)[i] = buff[2];

    ++i;
  }

  fclose(fp);

  return size;
}

// Update coefficient.
static void update(double *ce, double *data){
  // data [0->2] : x, y, t

  ce[0] += data[2] * data[0];  // a_i+1_ = a_i_ + t_n_ * x_n_
  ce[1] += data[2] * data[1];  // b_i+1_ = b_i_ + t_n_ * y_n_
  ce[2] += data[2];            // c_i+1_ = c_i_ + t_n_
}

// Linearly separate by Perceptron.
void p_learning(double *ce){
  const char *fname = "/Users/iwaitoshiya/u226136/PP_TestData.csv";
  double *x, *y, *t;
  int i, j, n;

  init(ce);

  x = y = t = NULL;
  n = readCSV(fname, &x, &y, &t);

  for(j = 0; j < NUM; j++){
    for(i = 0; i < n; i++){
      double data[3] = {x[i], y[i], t[i]};

      if(f(ce, data[0], data[1]) != data[2])
        update(ce, data);
    }
  }

  free(x);
  free(y);
  free(t);
}

// Show result.
static void showResult(double *ce){
  printf("<Result>\n");
  printf("F(X, Y) = %lfX + %lfY + %lf\n", ce[0], ce[1], ce[2]);
}
