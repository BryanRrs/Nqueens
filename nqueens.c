#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 8
#define POP 100
#define TOURNAMENT 10
#define MUTATION 0.8

typedef unsigned int uint;

typedef struct NQUEENS{
  int arr[SIZE];
}queens;

typedef struct Results{
  int index;
  int fitness;
}results;

void queenPrinter(queens* q);
void randomize(queens* q);
void popGenerator(queens* pop[], int n);
double randDouble();
void mutate(queens* q);
int in(int i, queens* q);
void crossover(queens* p1, queens* p2, queens* s);
void shuffle(queens* pop[], size_t n);
int fitness(queens* q);
void tournament(queens* winners[], queens* population[]);
void bestFitnessPop(queens* pop[], results* results);
uint rand_interval(unsigned int min, unsigned int max);

int main(void){
  srand(time(0));
  queens* population[POP];
  results results;
  popGenerator(population, POP);
  bestFitnessPop(population, &results);
  queens* winners[TOURNAMENT];
  popGenerator(winners, TOURNAMENT);
  int iterations = 0;
  queens prev_best;

  //MAIN LOOP
  while(results.fitness != 0){
    if(iterations%50 == 0){
        printf("iterations: %d, fitness:%d\n", iterations, results.fitness);
    }
    
    //leader
    for (int i = 0; i<SIZE; i++){
        prev_best.arr[i] = population[results.index]->arr[i];
    }
    tournament(winners, population);
    
    //NEXT GEN
    for(int i=0; i<POP; i++){
      uint p1 = rand_interval(0, TOURNAMENT-1);
      uint p2 = rand_interval(0, TOURNAMENT-1);
      crossover(winners[p1], winners[p2], population[i]);
      mutate(population[i]);
    }

    //leader
    uint leader = rand_interval(0, POP-1);
    for (int i = 0; i<SIZE; i++){
        population[leader]->arr[i] = prev_best.arr[i];
    }

    bestFitnessPop(population, &results);
    iterations++;
  }
  printf("\nBoard Size:%d\n",SIZE);
  printf("Total iterations: %d\n", iterations);
  queenPrinter(population[results.index]);
  for (int i = 0; i<POP; i++){
    free(population[i]);
  }
  for (int i = 0; i<TOURNAMENT; i++){
    free(winners[i]);
  }
  return EXIT_SUCCESS;
}

void queenPrinter(queens* q){
  printf("[");
  for (int i = 0; i<(SIZE-1); i++){
    printf("%d, ", q->arr[i]);
  }
  printf("%d]\n", q->arr[SIZE-1]);
}

void randomize(queens* q){
  for (int i = 0; i<SIZE; i++){
    q->arr[i] = i+1;
  }
  for (int i = 0; i<SIZE; i++){
    int x = rand()%SIZE;
    int y = rand()%SIZE;
    int l = q->arr[x];
    q->arr[x] = q->arr[y];
    q->arr[y] = l;
  }
} 

void popGenerator(queens* pop[], int n){
  for (int i=0; i<n; i++){
    queens* tq = malloc(sizeof(struct NQUEENS));
    randomize(tq);
    pop[i] = tq;
  }
}

int rand_lim(int limit) {
    int divisor = RAND_MAX/(limit+1);
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}

double randDouble(){
  return (rand()/(double)RAND_MAX);
}

void mutate(queens* q){
  if (randDouble()>=MUTATION){
    uint from = rand_interval(0, SIZE-1);
    uint to = rand_interval(0, SIZE-1);
    int temp = q->arr[from];
    q->arr[from] = q->arr[to];
    q->arr[to] = temp;
  }
}

int in(int x, queens* q){
  for (int i=0; i<SIZE; i++){
    if(q->arr[i] == x){
      return 1;
    }
  }
  return 0;
}

void crossover(queens *p1, queens *p2, queens *s){
  uint x1 = rand_interval(0, SIZE-1);
  uint x2 = 0;
  for(int i = 0; i<SIZE; i++){
    s->arr[i] = 0;
  }
  while(x2 < x1){
    x2 = rand_interval(0, SIZE-1);
  }
  for (int i = x1; i <= x2; i++){
    s->arr[i] = p1->arr[i]; 
  }
  int j = 0;
  for (int i=0;i<SIZE; i++){
    if (in(p2->arr[i], s)==0) {
        while (s->arr[j] != 0) {
            j++;}
        s->arr[j] = p2->arr[i];
    }
  }
  if (in(0, s)==1){
      puts("p1");
      queenPrinter(p1);
      puts("p2");
      queenPrinter(p2);
      puts("s");
      queenPrinter(s);
      printf("x1:%d, x2:%d\n", x1,x2);
      exit(1);
  }
}

void shuffle(queens* pop[], size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          queens* t = pop[j];
          pop[j] = pop[i];
          pop[i] = t;
        }
    }
}

int fitness(queens* q){
  int f = 0;
  for(int i=0; i<SIZE; i++){
    for(int j = 0; j<SIZE; j++){
      if(j!=i){
        if (q->arr[i] - i == q->arr[j]-j){
          f++;
        }
        else if(q->arr[i] + i == q->arr[j] + j){
          f++;
        }
      }
    }
  }
  return f;
}


void tournament(queens* winners[], queens* population[]){
  shuffle(population, POP);
  for(int i = 0; i<TOURNAMENT; i++){
    int best_fit = SIZE*SIZE;
    int index = 0;
    for(int j= i*(POP/TOURNAMENT); j<(i+1)*(POP/TOURNAMENT); j++){
      int f = fitness(population[j]);
      if (f<best_fit){
        best_fit = f;
        index = j;
      }
    }
    for (int j = 0; j<SIZE; j++){
        winners[i] -> arr[j] = population[index]->arr[j];
    }
  }
}

void bestFitnessPop(queens* population[], results* results){
  int best_fit = SIZE*SIZE;
  int index = 0;
  for(int i=0; i<POP; i++){
    int tf = fitness(population[i]);
    if(tf<best_fit){
      best_fit = tf;
      index = i;
    }
  }
  results->fitness = best_fit;
  results->index = index;
}

uint rand_interval(uint min, uint max)
{
    int r;
    const uint range = 1 + max - min;
    const uint buckets = RAND_MAX / range;
    const uint limit = buckets * range;

    /* Create equal size buckets all in a row, then fire randomly towards
     * the buckets until you land in one of them. All buckets are equally
     * likely. If you land off the end of the line of buckets, try again. */
    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}
