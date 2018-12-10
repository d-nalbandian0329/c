#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void exError(char *str){
  printf("%s\n", str);
  exit(EXIT_FAILURE);
}

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

// Do all processes.
void doAllProcess(char *fname, char *stack_path){
  FILE   *fp1, *fp2;
  double alpha, beta, sigma, diff, target;
  double fclass, ratio;
  char   stackP[200], base[80];
  int    ln, cn;

  openF(&fp1, fname, "r");       // File name path.

  // Extract base-file-name from fullpath.
  baseName(base, fname);

  alpha = (double)(extractPARA(base, "ALPHA", "BETA"));
  beta  = (double)(extractPARA(base, "BETA", "CN"));
  sigma = (double)(extractPARA(base, "SIGMA", ".csv"));
  ln    = (int)(extractPARA(base, "LN", "DX"));
  cn    = (int)(extractPARA(base, "CN", "SIGMA"));

  diff = 0.02;
  target = (int)(sigma/cn/diff) * diff;

  openF(&fp2, stackP, "a");  // Stack area path.
  sprintf(stackP, "%sSIGMA_CN_LN%d.csv", stack_path, ln);

  while(fscanf(fp1, "%lf,%lf", &fclass, &ratio) != EOF){
    if(target==fclass)
      break;
  }

  fprintf(fp2, "%lf,%lf,%lf\n", alpha, beta, ratio);

  fclose(fp1);
  fclose(fp2);
}


int main(int argc, char *argv[]){
  FILE *fp, *fp2;
  char base[100], stack_path[100];
  char *fname = "/Users/iwaitoshiya/Desktop/DATA_CN10PMAX0.9980PMIN0.0001SIGMA1.000/LN50/ALPHA0.500/LN50DX0.100ALPHA0.500BETA4.900CN10SIGMA1.000.csv";
  char *path = "/Users/iwaitoshiya/Desktop/";
  double alpha, sigma, beta, gomi1, gomi2;
  int ln, cn;

  openF(&fp, fname, "r");
  fscanf(fp, "%lf,%lf", &gomi1, &gomi2);
  printf("LINE1 : %lf,%lf\n", gomi1, gomi2);

  baseName(base, fname);

  strcpy(stack_path, path);
  strcat(stack_path, base);
  openF(&fp2, stack_path, "a");
  fprintf(fp2, "%lf,%lf\n", gomi1, gomi2);
  printf("stack_path : %s\n", stack_path);

  printf("basename : %s\n", base);

  alpha = beta = sigma = 0.0;
  ln = cn = 0;

  alpha = (double)(extractPARA(base, "ALPHA", "BETA"));
  beta  = (double)(extractPARA(base, "BETA", "CN"));
  sigma = (double)(extractPARA(base, "SIGMA", ".csv"));
  ln    = (int)(extractPARA(base, "LN", "DX"));
  cn    = (int)(extractPARA(base, "CN", "SIGMA"));

  printf("ALPHA : %lf\n", alpha);
  printf("BETA  : %lf\n", beta);
  printf("SIGMA : %lf\n", sigma);
  printf("LN : %d\n", ln);
  printf("CN : %d\n", cn);

  fclose(fp);
  fclose(fp2);
  return 0;
}
