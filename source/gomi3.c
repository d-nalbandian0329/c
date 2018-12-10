#include <stdio.h>
#include <string.h>
#include <math.h>


double exStoD(char *str){
  char buff[40];
  char *p1, *p2;
  int len1, len2;
  double int_part, f_part;
  int i;

  printf("1\n");
  strcpy(buff, str);

  printf("buffer : %s\n", buff);

  p1 = p2 = NULL;
  len1 = len2 = 0;

  if(strchr(buff, '.') == NULL){
    p1 = buff;
    p2 = NULL;
    printf("p1 : %s\n", p1);
  }
  else{
    p1 = strtok(buff, ".");
    p2 = strtok(NULL, ".");
    len2 = strlen(p2);
  }

  len1 = strlen(p1);
  printf("p1_len : %d\n", len1);

  int_part = f_part = 0.0;
  for(i = 0; i < len1; i++)
    int_part += (p1[i]-'0') * pow(10, (len1-1-i));

  for(i = 0; i < len2; i++)
    f_part += (p2[i]-'0') / pow(10, (i+1));

  return (int_part + f_part);
}

double testDAYO(char *str){
  double ret;
  char *p1, *p2;
  int n1, n2;
  double int_part = 0;
  double f_part = 0.0;
  int i;
  char buff[40];

  p1 = p2 = NULL;

  strcpy(buff, str);

  p1 = strtok(buff, ".");
  p2 = strtok(NULL, ".");

  n1 = strlen(p1);
  n2 = strlen(p2);

  printf("%s\n%s\n", p1, p2);

  for(i = 0; i < n1; i++)
    int_part += (p1[i]-'0') * pow(10, (n1-1-i));

  for(i = 0; i < n2; i++)
    f_part += (p2[i]-'0') / pow(10, (i+1));

  printf("int_part : %f\n", int_part);
  printf("f_part   : %f\n", f_part);

  ret = int_part + f_part;

  return ret;
}








int main(void){
  char *fname = "LN100DX0.100ALPHA0.500BETA4.300CN30SIGMA1.000.csv";
  char *hoge = NULL;
  char buff[20];
  char *endptr;
  double beta;
  int idx1, idx2, i;

  printf("fname : %s\n", fname);
  printf("BETA : %s\n", hoge = strstr(fname, "BETA"));
  printf("idx : %d\n", idx1 = (int)(hoge-fname));

  idx1 += strlen("BETA");
  idx2 = (int)(strstr(fname, "CN")-fname);

  for(i = idx1; i < idx2; i++)
    buff[i-idx1] = fname[i];
  buff[i-idx1] = '\0';

  printf("BETA : %s\n", buff);

  printf("2-0 : %d\n", (int)('2'-'0'));

  //  printf("BETA : %lf\n", testDAYO(buff));

  //printf("123456 : %d\n", (int)testDAYO("123456"));

  printf("--- exStoD ---\n");
  printf("BETA   : %lf\n", exStoD("4.3000"));
  printf("123456 : %d\n", (int)exStoD("123456"));

  return 0;
}
