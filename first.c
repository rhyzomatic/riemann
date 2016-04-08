#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<pthread.h>

#define e 2.718281828459

int calc(int dim, int P[dim][dim], int d, int *x, int **a){
    int s = 0;
    int i;
    for (i = 0; i < (d - 1); i++){
        s += P[i][d-1]*a[i][d-i-2];
    }
    s *= 2;
    s += P[d-1][d-1]*x[d-1]*x[d-1];

    return s;
}


int rec(int dim, int P[dim][dim], double *sum, int d, int *x, int **a, int cur_sum, int lower_bound, int upper_bound, int div_low, int div_upp){
    bool descend = (d < dim);
    int i, j, lbound, ubound;
    if (d == 1){
        lbound = div_low;
        ubound = div_upp;
    }
    else {
        lbound = lower_bound;
        ubound = upper_bound;
    }
    for (i = lbound; i < ubound; i++){
        for (j = 0; j < d - 1; j++){
            a[j][d-j-2] = i*x[j];
        }
        x[d - 1] = i;
        int s = calc(dim, P, d, x, a);
        cur_sum += s;
        //printf("sum: %lf\n", *sum);

        if (descend){
            rec(dim, P, sum, d+1, x, a, cur_sum, lower_bound, upper_bound, div_low, div_upp);
        }
        else {
            //printf("%d %d %d %d\n", x[0], x[1], x[2], x[3]);
            //printf("%d %d %d %d %d %d\n", a[0][0], a[0][1], a[0][2], a[1][0], a[1][1], a[2][0]);
            //printf("%d\n", cur_sum);
            *sum += pow(e, (double) -cur_sum);
        }
        cur_sum -= s;
    }
    return 0;
}

double *helper(void *dimh, void *Ph, void *upper_boundh, void *lower_boundh, void *div_lowh, void *div_upph){
    int dim = (int) dimh;
    int P[dim][dim] = *Ph;
    int upper_bound = (int) upper_bound;
    int lower_bound = (int) lower_bound;
    int div_low = (int) div_lowh;
    int div_upp = (int) div_upph;

    double sum;
    int x[dim];
    int *a[dim-1]; //TODO: maybe make this 2
    int i;
    int j = 0;
    for (i = dim - 1; i > 0; i--){
        a[j] = malloc(sizeof(int) * i);
        j++;
    }

    pthread_detach(pthread_self());
    rec(dim, P, &sum, 1, x, a, 0, lower_bound, upper_bound, div_low, div_upp);
    return sum;
}

int read_P(int dim, int P[dim][dim]){
    int i, j;
    char *line = NULL;
    size_t size;
    char *tok;
    for (i = 0; i < dim; i++){
        getline(&line, &size, stdin);

        tok = strtok(line, " ");
        P[i][0] = atoi(tok);
        for (j = 1; j < dim; j++){
            tok = strtok(NULL, " ");
            P[i][j] = atoi(tok);
        }
    }

    return 0;
}



int main(){
    char *n = NULL;
    int dim;
    size_t size;

    getline(&n, &size, stdin);
    dim = atoi(n);
    /*int P[4][4] = {{5,  2,  0,  0},
                   {2,  5,  2, -2},
                   {0,  2,  5,  0},
                   {0, -2,  0,  5}};
                   */

    /*int P[9][9] = {{7, 2, 0, 0, 6, 2, 0, 0, 6},
                   {2, 7, 0, 0, 2, 6, 0, 0, 2},
                   {0, 0, 7, -2, 0, 0, 6, -2, 0},
                   {0, 0, -2, 7, 0, 0, -2, 6, 0},
                   {6, 2, 0, 0, 7, 2, 0, 0, 6},
                   {2, 6, 0, 0, 2, 7, 0, 0, 2},
                   {0, 0, 6, -2, 0, 0, 7, -2, 0},
                   {0, 0, -2, 6, 0, 0, -2, 7, 0},
                   {6, 2, 0, 0, 6, 2, 0, 0, 7}};*/

    int P[dim][dim];
    read_P(dim, P);

    int x[dim];
    int *a[dim - 1];
    


    int range = 4;

    int cores = 4;
    int core;
    int per_core = range / cores;
    int lower_bound = -(range/2);
    int upper_bound = -lower_bound + 1;
    int start = lower_bound;
    double sums[cores];

    for (core = 0; core < cores-1; core++){
        sums[core] = helper((void*) dim, (void *)P, (void*) lower_bound, (void *) upper_bound, (void *) start, (void *) start+per_core);
        start += per_core;
    }
    //helper(dim, P, &sums[cores-1], 1, x, a, 0, lower_bound, upper_bound, start, upper_bound);

    int i;
    double sum;
    for (i = 0; i < cores; i++){
        sum += sums[i];
    }

    printf("%14.14f\n", sum);

    return 0;
}

