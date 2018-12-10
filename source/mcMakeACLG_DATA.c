#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MEMORY_ERROR_MESSAGE "OutofMemoryException!!\n"

#define errorEXE(str) printf(str),exit(EXIT_FAILURE)

#define def_calloc(ptr,cast,num,size) \
                      do{\
                        if((ptr=(cast)calloc(num,size))==NULL)\
                          errorEXE(MEMORY_ERROR_MESSAGE);\
                      }while(0)

typedef unsigned char Data;

double *cn_all;


// Allocate primitive type array.
void get_array_2d(Data ***p, int row, int col){
  int i;

  def_calloc(*p, Data **, row, sizeof(Data *));

  for(i = 0; i < row; i++)
    def_calloc((*p)[i], Data *, col, sizeof(Data));
}

// Free 2D array.
void free_array_2d(Data **p, int row){
  int i;

  for(i = 0; i < row; i++)
    free((p)[i]);
  free(p);
}

// Open file.
void openF(FILE **fp, char *fname, char *fmode){
  if((*fp = fopen(fname, fmode)) == NULL)
    errorEXE("File not found!!\n");
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

// Autocorrelation function.
double funcCn(unsigned char *p, int tau, int T){
  double ret = 0.0;
  int    t;

  for(t = 0; t < T - tau; t++)
    ret += p[t] * p[t+tau];

  return ret / (T-tau);
}


// Make CSV file.
void makeAC_FILE(unsigned char *p, char *stack_path, int step_no){
  FILE *fp;
  char buff[200];
  int  tau, t_limit;

  sprintf(buff, "%s", stack_path);
  openF(&fp, buff, "w");

  t_limit = (int)(step_no*0.95);
  for(tau = 0; tau < t_limit; tau++){
    double temp = funcCn(p, tau, step_no);
    cn_all[tau] += temp;
    fprintf(fp, "%d,%lf\n", tau, temp);
  }
  fclose(fp);
}


void doAllProcess(char *fname, char *stack_path){
  FILE   *fp;
  Data   **all_data;           // A pointer to 2D array.  NUM * Step_Count.
  char   buff_path[200], buff_fname[200], buff_dir[200];
  char   command[200];
  double sigma;
  int    cn, tau, t_limit;     // tau. Bin width of autocorrelation.
  int    unit_no, step_no;     // Number of unit and step.
  int    buff_unit, buff_step; // Buffer.
  int    i;

  openF(&fp, fname, "r");

  baseName(buff_fname, fname);

  sigma   = extractPARA(buff_fname, "SIGMA", ".csv");
  cn      = extractPARA(buff_fname, "_CN", "SIGMA");
  unit_no = extractPARA(buff_fname, "U", "_T");
  step_no = extractPARA(buff_fname, "T", "_RASTER");

  get_array_2d(&all_data, unit_no, step_no);

  def_calloc(cn_all, double *, (int)(step_no*0.95), sizeof(double));

  i = buff_unit = buff_step = 0;
  while(fscanf(fp, "%d,%d", &buff_step, &buff_unit) != EOF)
    ++(all_data[buff_unit][buff_step]);

  fclose(fp);



  // Make directory.
  sprintf(buff_dir, "%sLineGraph/SIGMA%.3f/", stack_path, sigma);
  sprintf(command, "mkdir -p %s", buff_dir);

  system(command);

  // Make CSV file.
  for(i = 0; i < unit_no; i++){
    sprintf(buff_path, "%sUNIT%d_%s", buff_dir, i, buff_fname);

    makeAC_FILE(all_data[i], buff_path, step_no);
  }

  sprintf(buff_path, "%sUNITALL_%s", buff_dir, buff_fname);
  openF(&fp, buff_path, "w");

  for(i = 0; i < (int)(step_no*0.95); i++)
    fprintf(fp, "%d,%lf\n", i, cn_all[i]/unit_no);

  fclose(fp);


  free(cn_all);
  free_array_2d(all_data, unit_no);
}


int main(int argc, char *argv[]){
  doAllProcess(argv[1], argv[2]);
  return 0;
}

