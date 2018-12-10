// Mutual connected Neuronal Network proto-type.
// This program manage process by step count.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Mersenne Twister.
#include "MT.h"

// Self made Linked list and Queue.
#include "QUEUE.h"


// Units count.
#define NUM 100

// Connect count between units.
#define CN 5

// Depression correlation.
#define DEP 0.7

// Max step count.
#define C_LIMIT 3000

//
#define NET_NUM 20

//
#define HOGE 100


// Functional macro.
// Uniform distribution  -> range:[0,1]
#define uniformMT() genrand_real1()
// Uniform distribution  -> range:[0,1)
#define uniformMT2() genrand_real2()

// Swap value.
#define swap(type,x,y) do{ type t = x; x = y; y = t; }while(0)

// Unit data type. One unit is equal to one neuron.
typedef struct unit{
  unsigned int flag;    // Fire flag.
  unsigned int d_flag;  // Evaluation criteria that this unit is depression neuron wheather or not.
  double **probset;     // To stack fire-probability that propagate to partner unit..
  List   prob_list;     // To stack fire-probability that received from partner.
}Unit;


// Manage data type.
// To manage all process.
typedef struct manage{
  unsigned int step_count;  // Overall step-count.
  Queue f_queue;    // To manage index of fire event target.
  Queue p_queue;    // To manage index of propagate event target.
  Queue dep_queue;  // To manage index of depression event target.
}Manage;


// All fire count at overall network.
int all_fire;


// Sum of fire-probability to propagate partner.
double SIGMA = 1.0;


// Initialized all.
static void init(Unit all_unit[NUM], Manage *);

// Allocate probability-set.
static void getProbSet(double ***, int, int);

// Free probability-set.
static void freeProbSet(double ***, int);

// Terminate all.
static void terminate(Unit all_unit[NUM], Manage *);


// Ignite specified unit.
static int setFlag(Unit all_unit[NUM]);

// Reset fire flag.
static void resetFlag(Unit *);

// Set depression flag.
static void setDepFlag(Unit *);

static void setDep(Unit all_unit[NUM], int);


// Make links between units.
static void makeLink(Unit all_unit[NUM], int);

// If CN%2==0, use this method.
static int evenCN(double probset[CN][2]);

// If CN%2!=0, use this method.
static int oddCN(double probset[CN][2]);

// Set probability to propagate partner unit.
static void setProb(Unit *);

// Make network using makeLink() and setProb().
static void makeNet(Unit all_unit[NUM]);


// Judge that the unit will fire or not.
static void fire(Unit all_unit[NUM], Manage *manager, int idx);

// Propagate process.
static void propagate(Unit all_unit[NUM], Manage *manager, int idx);

// Depress partner unit.
static void depression(Unit all_unit[NUM], Manage *manager, int idx);


// Open File.
// @Para1 : File pointer to access specified file.
// @Para2 : File name.
// @Para3 : File open mode.
void openF(FILE **, char *, char *);


// Manage process of fire and propagate.
static void manageStep(Unit all_unit[NUM], Manage *manager);

static void doAllProcess(Unit all_unit[NUM], Manage *manager);


int main(void){
  Unit   all_unit[NUM];
  Manage manager;

  init_genrand((unsigned int)time(NULL));uniformMT();

  doAllProcess(all_unit, &manager);

  return 0;
}


// Initialized all.
static void init(Unit all_unit[NUM], Manage *manager){
  int i;

  for(i = 0; i < NUM; i++){
    getProbSet(&all_unit[i].probset, CN, 2);
    initList(&all_unit[i].prob_list);
    all_unit[i].flag = all_unit[i].d_flag = 0U;
  }

  manager->step_count = 1U;

  initQueue(&manager->f_queue);
  initQueue(&manager->p_queue);
  initQueue(&manager->dep_queue);
}

static void init_Probset(Unit all_unit[NUM]){
  int i;

  for(i = 0; i < NUM; i++){
    getProbSet(&all_unit[i].probset, CN, 2);
    all_unit[i].d_flag = 0U;
  }
}

static void init_Unit(Unit all_unit[NUM], Manage *manager){
  int i;

  for(i = 0; i < NUM; i++){
    initList(&all_unit[i].prob_list);
    all_unit[i].flag = 0U;
  }

  manager->step_count = 1U;

  initQueue(&manager->f_queue);
  initQueue(&manager->p_queue);
  initQueue(&manager->dep_queue);
}


// Allocate probability-set.
static void getProbSet(double ***p, int row, int col){
  int i;

  def_calloc(*p, double **, row, sizeof(double *));
  for(i = 0; i < row; i++)
    def_calloc((*p)[i], double *, col, sizeof(double));
}

// Free probability-set.
static void freeProbSet(double ***p, int row){
  int i;

  if(*p!=NULL){
    for(i = 0; i < row; i++)
      free((*p)[i]);
    free(*p);
  }

  *p = NULL;
}

// Terminate all.
static void terminate(Unit all_unit[NUM], Manage *manager){
  int i;

  for(i = 0; i < NUM; i++){
    freeProbSet(&all_unit[i].probset, CN);
    termList(&all_unit[i].prob_list);
  }

  termQueue(&manager->f_queue);
  termQueue(&manager->p_queue);
  termQueue(&manager->dep_queue);
}

static void terminate_Probset(Unit all_unit[NUM]){
  int i;

  for(i = 0; i < NUM; i++)
    freeProbSet(&all_unit[i].probset, CN);
}

static void terminate_Unit(Unit all_unit[NUM], Manage *manager){
  int i;

  for(i = 0; i < NUM; i++)
    termList(&all_unit[i].prob_list);

  termQueue(&manager->f_queue);
  termQueue(&manager->p_queue);
  termQueue(&manager->dep_queue);
}

// Ignite specified unit.
static int setFlag(Unit all_unit[NUM]){
  int idx = (int)(uniformMT2()*NUM);
  all_unit[idx].flag = 1U;
  return idx;
}

// Reset fire flag.
static void resetFlag(Unit *unit){
  unit->flag = 0U;
}

// Set depression flag.
static void setDepFlag(Unit *unit){
  unit->d_flag = 1U;
}

// Set depression neuron.
static void setDep(Unit all_unit[NUM], int n){
  int i = 0;

  while(i < n){
    int idx = (int)(NUM*uniformMT2());

    if(!all_unit[idx].d_flag){
      setDepFlag(&all_unit[idx]);
      ++i;
    }
  }
}

// Make link between all units.
static void makeLink(Unit *unit, int idx){
  int i;

  for(i = 0; i < CN; i++)
    unit->probset[i][0] = -1;

  i = 0;
  while(i < CN){
    int temp = (int)(uniformMT() * NUM);
    int k = 0;

    // Avoid for storing duplicated value.
    while(k < i){
      if(temp==(int)(unit->probset[k][0]) || temp==idx)
	break;

      ++k;
    }

    if(k == i){
      unit->probset[i][0] = temp;
      ++i;
    }
  }

}

// If CN%2==0, use this method.
// index 0->CN/2-1  : source
// index CN/2->CN-1 : destination
static int evenCN(double probset[CN][2]){
  double diff_range = SIGMA / CN * 0.8;
  int idx[CN];
  int i;

  for(i = 0; i < CN; i++)
    idx[i] = i;

  for(i = 0; i < CN; i++){
    int var = (int)(uniformMT()*(CN-1));
    swap(int, idx[i], idx[var]);
  }

  for(i = 0; i < CN/2; i++){
    // diff range is [-diff_range, +diff_range].
    double diff = uniformMT() * diff_range * 2 - diff_range;
    
    probset[idx[i]][1]      += diff;
    probset[idx[i+CN/2]][1] -= diff;
  }

  return idx[CN-1];
}

// If CN%2!=0, use this method.
static int oddCN(double probset[CN][2]){
  // diff range is [-diff_range, +diff_range].
  double diff_range = SIGMA / CN * 0.8;
  double diff = uniformMT() * diff_range * 2 - diff_range;
  int idx[CN-1], n, i, j;

  if(CN < 1)  return -1;

  n = evenCN(probset);

  j = 0;
  for(i = 0; i < CN-1; i++){
    if(n == i)  continue;

    if((0 < probset[i][1] - diff) && (probset[i][1] - diff < 1))
      idx[j++] = i;
  }

  if(!j) return -1;

  i = (int)(uniformMT() * (j-1));

  probset[n][1]      += diff;
  probset[idx[i]][1] -= diff;

  return 0;
}

// Make probability to propagate units in next layer.
static void setProb(Unit *unit){
  double ave = SIGMA / CN;
  int (*func)(double [CN][2]) = CN%2 ? oddCN : evenCN;
  int i;

  for(i = 0; i < CN; i++)
    unit->probset[i][1] = ave;

  //(*func)(unit->probset);
}

// Make network using makeLink() and setProb().
static void makeNet(Unit all_unit[NUM]){
  int i, j;

  setDep(all_unit, (int)(NUM*0.2));

  for(i = 0; i < NUM; i++){
    makeLink(&all_unit[i], i);
    setProb(&all_unit[i]);
  }
}

// Judge that the unit will fire or not.
static void fire(Unit all_unit[NUM], Manage *manager, int idx){
  List *list = &all_unit[idx].prob_list;
  int  i = 0;

  // If this unit has fired already, its list must be terminated.
  // So duplicated element in fire-queue will never fire.
  while(i < getSize(list)){
    if(getData(list, i) >= uniformMT()){
      all_unit[idx].flag = 1U;
      break;
    }

    ++i;
  }

  if(all_unit[idx].flag){
    enqueue(&manager->p_queue, idx);
    resetFlag(&all_unit[idx]);
    ++all_fire;

    if(all_unit[idx].d_flag)
      enqueue(&manager->dep_queue, idx);
  }

  if(getSize(&all_unit[idx].prob_list))
    termList(&all_unit[idx].prob_list);
}

// Propagate process.
static void propagate(Unit all_unit[NUM], Manage *manager, int idx){
  int i;

  for(i = 0; i < CN; i++){
    addNode(&(all_unit[(int)(all_unit[idx].probset[i][0])].prob_list),
	all_unit[idx].probset[i][1], 0);

    enqueue(&manager->f_queue, all_unit[idx].probset[i][0]);
  }
}

// Depress partner unit.
static void depression(Unit all_unit[NUM], Manage *manager, int idx){
  int i, j;

  i = 0;
  while(i < CN){
    List *list = &all_unit[(int)(all_unit[idx].probset[i][0])].prob_list;

    j = 0;
    while(j < getSize(list)){
      setData(list, j, (Data)(getData(list, j)*DEP));
      ++j;
    }

    ++i;
  }

}

// Garbage for debug.
void test(Manage *manager){
  printf("Step_Count : %u\n", manager->step_count);
  printf("Fire_Count : %dF\n\n", all_fire);
}


// Open File.
// @Para1 : File pointer to access specified file.
// @Para2 : File name.
// @Para3 : File open mode.
void openF(FILE **fp, char *fname, char *fmode){
  if((*fp = fopen(fname, fmode)) == NULL)
    errorEXE("File not found!!\n");
}


// Manage process of fire and propagate.
static void manageStep(Unit all_unit[NUM], Manage *manager){
  int idx = setFlag(all_unit);
  int i;

  enqueue(&manager->f_queue, idx);

  // While either queue have any element, this loop continue.
  while(size(&manager->f_queue) || size(&manager->p_queue)){
    int limit;

    if(manager->step_count > C_LIMIT) 
      break;

    all_fire = 0;
    limit = size(&manager->f_queue);

    for(i = 0; i < limit; i++){
      idx = (int)dequeue(&manager->f_queue);
      fire(all_unit, manager, idx);
    }

    limit = size(&manager->p_queue);
    for(i = 0; i < limit; i++){
      idx = dequeue(&manager->p_queue);
      propagate(all_unit, manager, idx);
    }

    limit = size(&manager->dep_queue);
    for(i = 0; i < limit; i++){
      idx = dequeue(&manager->dep_queue);
      depression(all_unit, manager, idx);
    }

    ++(manager->step_count);
  }

}

static void doAllProcess(Unit all_unit[NUM], Manage *manager){
  FILE *fp, *fp_buff;
  char fname[200];
  int  sum, i, j;

  fp = fp_buff = NULL;

  sprintf(fname, "/Users/iwaitoshiya/Desktop/MCNN_DATA/U%d/CN%d/Depression/CSV/SurvivalP/MCNN_U%d_T%d_SurvivalP_CN%d.csv",
               NUM, CN, NUM, C_LIMIT, CN);

  fp_buff = fopen(fname, "r");
  if(fp_buff!=NULL)
    fclose(fp_buff);
  remove(fname);

  openF(&fp, fname, "a");

  for(SIGMA = 0.1; SIGMA <= 5.01; SIGMA += 0.1){
    sum = 0;
    for(i = 0; i < NET_NUM; i++){
      init_Probset(all_unit);
      makeNet(all_unit);

      for(j = 0; j < HOGE; j++){
        init_Unit(all_unit, manager);
        manageStep(all_unit, manager);
        terminate_Unit(all_unit, manager);
        sum += all_fire;
      }

      terminate_Probset(all_unit);
    }

    fprintf(fp, "%lf,%lf\n", SIGMA, (double)sum/HOGE/NET_NUM/NUM);
  }

  fclose(fp);
}

