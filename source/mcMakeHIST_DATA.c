#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BIN_WIDTH 100

#define MEMORY_ERROR_MESSAGE "OutofMemoryException!!\n"

#define errorEXE(str) printf(str),exit(EXIT_FAILURE)

#define def_calloc(ptr,cast,num,size) \
                      do{\
                        if((ptr=(cast)calloc(num,size))==NULL)\
                          errorEXE(MEMORY_ERROR_MESSAGE);\
                      }while(0)


void openF(FILE **fp, const char *fname, const char *mode){
  if((*fp = fopen(fname, mode)) == NULL)
    errorEXE("FileNotFound!!");
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
  char buff[200];
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

void makeHIST(FILE *s_fp, FILE *d_fp, int unit, int step, int bin){
  int *buff_data, *hist;
  int i, j;

  def_calloc(buff_data, int *, step, sizeof(int));
  def_calloc(hist, int *, unit/bin, sizeof(int));

  while(fscanf(s_fp, "%d,%d", &i, &j) != EOF)
    ++(buff_data[i]);

  for(i = 0; i < step; i++)
    ++(hist[buff_data[i]/bin]);

  for(i = 0; i < unit/bin; i++)
    fprintf(d_fp, "%d,%d\n", i*bin, hist[i]);

  free(buff_data);
  free(hist);
}

void doAllProcess(char *fname, char *stack_path){
  FILE *s_fp, *d_fp;
  char buff_stack[200], buff_base[200];
  int  unit, step;
  int  bin;  // Bin-width of histgram.

  openF(&s_fp, fname, "r");

  baseName(buff_base, fname);

  unit = (int)extractPARA(buff_base, "_U", "_T");
  step = (int)extractPARA(buff_base, "_T", "_RASTER");

  bin = unit / BIN_WIDTH;

  sprintf(buff_stack, "%sHIST_%s", stack_path, buff_base);
  
  remove(buff_stack);

  openF(&d_fp, buff_stack, "a");

  makeHIST(s_fp, d_fp, unit, step, bin);

  fclose(s_fp);
  fclose(d_fp);
}



int main(int argc, char *argv[]){
  char *fname = argv[1];
  char *stack_path = argv[2];

  doAllProcess(fname, stack_path);

  return 0;
}

