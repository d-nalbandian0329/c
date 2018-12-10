#include <stdio.h>
#include <stdlib.h>


#define errorEXE(str) printf(str),exit(EXIT_FAILURE)

// Open file stream.
void openF(FILE **fp, const char *fname, const char *mode){
  if((*fp = fopen(fname, mode)) == NULL)
    errorEXE("\nFileNotFound!!\n");
}


// Do all process.
void doAllProcess(const char *fname, const char *stack_path){
  FILE   *fp_src, *fp_dest;
  double Cn;
  int    tau;

  openF(&fp_src,  fname,      "r");
  openF(&fp_dest, stack_path, "a");

  while(fscanf(fp_src, "%d,%lf", &tau, &Cn) != EOF)
    fprintf(fp_dest, "%d,%lf\n", tau, Cn);

  fclose(fp_src);
  fclose(fp_dest);
}


// Main method.
int main(int argc, char *argv[]){
  const char *fname = argv[1];
  const char *stack_path = argv[2];

  doAllProcess(fname, stack_path);

  return 0;
}

