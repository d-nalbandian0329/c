#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Number of data.
#define NUM 50

#define MAX  30
#define MIN -30

// Return random number in range [0,1].
static double uniform();

// Return real number in specified range at random.
// @Para1 : start
// @Para2 : end
static double rangeUNI(double, double);

static double f(double, double);

static int sigPlus(double, double);

// @Para1 : A const pointer to file name.
// @Para2 : A const Pointer to delimiter character.
static void makeCSVFile(const char *, const char *);


int main(void){
  const char *fname = "/Users/iwaitoshiya/u226136/PP_TestData.csv";
  const char *DELIMITER = ",";

  srand((unsigned int)time(NULL));rand();

  makeCSVFile(fname, DELIMITER);

  return 0;
}


// Return random number in range [0,1].
static double uniform(){
  return (rand()+1.0) / (RAND_MAX+1.0);
}

// Return real number in specified range at random.
// @Para1 : start
// @Para2 : end
static double rangeUNI(double min, double max){
  return (min + uniform() * (max-min));
}

static double f(double x, double y){
  return (2 * x - y + 1);
}

static int sigPlus(double x, double y){
  return (f(x,y)>=0 ? 1 : -1);
}

// @Para1 : A const pointer to file name.
// @Para2 : A const Pointer to delimiter character.
static void makeCSVFile(const char *fname, const char *DELIMITER){
  FILE *fp;
  int  i;

  if((fp = fopen(fname, "w")) == NULL){
    printf("File Not Found!!\n");
    exit(EXIT_FAILURE);
  }

  // Row count of this CSV file.
  fprintf(fp, "%d\n", NUM);

  i = 0;
  while(i++ < NUM){
    double x = rangeUNI(MIN, MAX);
    double y = rangeUNI(MIN, MAX);

    fprintf(fp, "%lf%s%lf%s%lf\n", x, DELIMITER, y, DELIMITER
	    , (double)sigPlus(x,y));
  }

  fclose(fp);
}
