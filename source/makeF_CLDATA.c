#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Functional macro.
#define exError(str) printf(str),exit(EXIT_FAILURE)

// Open file.
void openF(FILE **fp, char *fname, char *fmode){
  if((*fp = fopen(fname, fmode)) == NULL)
    exError("File not found!!\n");
}

// Exchange string to double.
double exStoD(char *str){
  char buff[40];
  char *p1, *p2;
  int len1, len2;
  double int_part, f_part;
  int i;

  strcpy(buff, str);

  p1 = p2 = NULL;
  len1 = len2 = 0;

  if(strchr(buff, '.') == NULL){
    p1 = buff;
    p2 = NULL;
  }
  else{
    p1 = strtok(buff, ".");
    p2 = strtok(NULL, ".");
    len2 = strlen(p2);
  }

  len1 = strlen(p1);

  int_part = f_part = 0.0;
  for(i = 0; i < len1; i++)
    int_part += (p1[i]-'0') * pow(10, (len1-1-i));

  for(i = 0; i < len2; i++)
    f_part += (p2[i]-'0') / pow(10, (i+1));

  return (int_part + f_part);
}


// Extract parameter from the title of CSV data file.
double extractPARA(char *fname, char *s1, char *s2){
  char buff[40];
  int s_idx, e_idx;
  int i;

  s_idx = (int)(strstr(fname, s1) + strlen(s1) - fname);
  e_idx = (int)(strstr(fname, s2) - fname);

  for(i = s_idx; i < e_idx; i++)
    buff[i-s_idx] = fname[i];
  buff[i-s_idx] = '\0';

  return exStoD(buff);
}

void baseName(char *base, char *fname){
  char buff[200];
  char *p, *temp;

  p = temp = NULL;

  strcpy(buff, fname);
  temp = strtok(buff, "/");
  do{
    p = temp;
  }while((temp = strtok(NULL, "/")) != NULL);

  strcpy(base, p);
}

// Make CSV file.
// This file contains fire situation after more than once learning in "stdpNN.c".
void makeF_FILE(char *stack_path, char *prefix, int ln, double alpha, double beta, int data){
  FILE *fp;
  char buff[200];

  sprintf(buff, "%s%sSIGMA_CN_LN%d.csv", stack_path, prefix, ln);
  openF(&fp, buff, "a");

  fprintf(fp, "%lf,%lf,%d\n", alpha, beta, data);

  fclose(fp);
}


// Do all processes.
void doAllProcess(char *fname, char *path){
  FILE   *fp;
  double alpha, beta, sigma, diff, target;
  double fclass, ratio;
  double ave[3];
  char   stack_path[200], base[80];
  char   *prefix[2][3] = {{"PRE_ALL_",  "PRE_LF_",  "PRE_LL_"},
			  {"POST_ALL_", "POST_LF_", "POST_LL_"}};
  int    buff[3];  // 0:all_fire, 1:last_fire, 2:last_layer
  int    ln, cn, i;

  openF(&fp, fname, "r");

  // Extract base-file-name from fullpath.
  baseName(base, fname);

  alpha = (double)(extractPARA(base, "ALPHA", "BETA"));
  beta  = (double)(extractPARA(base, "BETA", "CN"));
  sigma = (double)(extractPARA(base, "SIGMA", ".csv"));
  ln    = (int)(extractPARA(base, "LN", "DX"));
  cn    = (int)(extractPARA(base, "CN", "SIGMA"));

  sprintf(stack_path, "%s", path);

  fscanf(fp, "%d,%d,%d", &buff[0], &buff[1], &buff[2]);

  for(i = 0; i < 3; i++){
    makeF_FILE(stack_path, prefix[0][i], ln, alpha, beta, buff[i]);
    ave[i] = buff[i];
  }

  while(fscanf(fp, "%d,%d,%d", &buff[0], &buff[1], &buff[2]) != EOF)
    for(i = 0; i < 3; i++)
      ave[i] += buff[i];

  // Calculate average of fire count.
  for(i = 0; i < 3; i++)
    ave[i] /= ln;

  for(i = 0; i < 3; i++)
    makeF_FILE(stack_path, prefix[1][i], ln, alpha, beta, (int)ave[i]);

  fclose(fp);
}



int main(int argc, char *argv[]){
  if(argc < 3)
    exError("Too few data!!\n");

  doAllProcess(argv[1], argv[2]);

  return 0;
}
