
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define true 1
#define false 0

typedef int boolean;
typedef struct
{
    boolean exists;
    boolean belongs;
} cell_backpack;

// A utility function to find the vertex with minimum
// distance value, from the set of vertices not yet included
// in shortest path tree
int minDistance(int dist[], boolean sptSet[], int n)
{
    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < n; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

/*
ijkstra() is a function that implements Dijstra's algorithm for finding
the shortest distance between two points on a graph.

Parameters:
grf : the pointer to the array representing the graph. The element grf[i][j]
contains the distance between vertices i and j of the graph, if there is an edge of the node
i to node j. If there is no edge from i to j, then the value of grf[i][j]
is INT_MAX (the largest possible value of an integer)
n : size of the grf array, that is, grf is an nxn matrix;
s : initial vertex;
v_f : final vertex

Return Value: Shortest distance between s and v_f.
Note: 0 =< s and v_f < n. Return the distance 0 (zero) if the values ​​of
s and v_f are not valid.

*/

int dijkstra(int **grf, int n, int s, int v_f)
{
    /* IMPLEMENT YOUR CODE HERE */
    if (s < 0 || v_f >= n)
    {
        return 0;
    }
    int *dist = (int *)malloc(n * sizeof(int)); // The output array.  dist[i] will hold the
    // shortest
    // distance from src to i

    boolean *sptSet = (boolean *)malloc(n * sizeof(boolean)); // sptSet[i] will be true if vertex i is
                                                              // included in shortest
    // path tree or shortest distance from src to i is
    // finalized

    // Initialize all distances as INFINITE and stpSet[] as
    // false
    for (int i = 0; i < n; i++)
        dist[i] = INT_MAX, sptSet[i] = false;

    // Distance of source vertex from itself is always 0
    dist[s] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < n - 1; count++)
    {
        // Pick the minimum distance vertex from the set of
        // vertices not yet processed. u is always equal to
        // src in the first iteration.
        int u = minDistance(dist, sptSet, n);

        // Mark the picked vertex as processed
        sptSet[u] = true;

        // Update dist value of the adjacent vertices of the
        // picked vertex.
        for (int v = 0; v < n; v++)

            // Update dist[v] only if is not in sptSet,
            // there is an edge from u to v, and total
            // weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (!sptSet[v] && grf[u][v] != INT_MAX && dist[u] != INT_MAX && dist[u] + grf[u][v] < dist[v])
                dist[v] = dist[u] + grf[u][v];
    }

    int retval = dist[v_f];
    free(dist);
    free(sptSet);

    return retval;
}

/*
backpack() returns a matrix P[n+1,K+1] with the possible solutions to the problem of
backpack for an n-size S-arrangement and a K-size backpack.
P is a matrix such that P[i,j].exists = true, if there is a solution to the
knapsack problem with first i
(0 <= i <= n) elements and a backpack of size k (0 <= k <= K);
and P[i,j].belongs = true, if the ith element belongs to the solution.

Parameters:
S: an array of size n that stores item sizes.
n: array size S
K: backpack size

Return value: pointer to array P.

Note: Valid values for S, n, and K are, respectively,
S != NULL, n > 0, K > 0. The function must return NULL if the values
of any of the parameters is not valid.

Hint: don't forget to make a copy of S, called, for example,
S2, which includes an item of size 0 (zero). For example, if
S[] = {2,3,5,6}, S2 will be S2[] = {0,2,3,5,6}.
*/

cell_backpack **backpack(int *S, int n, int K)
{
    /* IMPLEMENT YOUR CODE */
    if (S == NULL || n <= 0 || K <= 0)
    {
        return NULL;
    }
    // The value of subset[i][j] will be true if
    // there is a subset of set[0..j-1] with sum
    // equal to i
    cell_backpack **subset;
    subset = (cell_backpack **)malloc((n + 1) * sizeof(cell_backpack *));
    for (int i = 0; i <= n; ++i)
    {
        subset[i] = (cell_backpack *)malloc((K + 1) * sizeof(cell_backpack));
    }

    // If sum is 0, then answer is true
    for (int i = 0; i <= n; i++)
    {
        subset[i][0].exists = true;
    }

    // If sum is not 0 and set is empty,
    // then answer is false
    for (int i = 1; i <= K; i++)
    {
        subset[0][i].exists = false;
    }

    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= K; j++)
            subset[i][j].belongs = false;
    }

    // Fill the subset table in bottom up manner
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= K; j++)
        {
            if (j < S[i - 1])
                subset[i][j].exists = subset[i - 1][j].exists;
            if (j >= S[i - 1])
                subset[i][j].exists = subset[i - 1][j].exists || subset[i - 1][j - S[i - 1]].exists;
        }
    }
    int tempK = K;
    if (subset[n][K].exists == true)
    {
        while (n != 0)
        {
            if (subset[n - 1][tempK].exists == true)
            {
                n = n - 1;
            }
            else if (subset[n][tempK - 1].exists == true)
            {
                // for(int j=0;j<=K;j++)
                subset[n][S[n - 1]].belongs = true;
                tempK = tempK - (S[n - 1]);
                n = n - 1;
            }
            else
            {
                tempK = tempK - (S[n - 1]);
                n = n - 1;
            }
        }
    }

    return subset;
}

/* selectBackpackItems() loops through an array P[n+1,K+1] with the
possible solutions to the problem of
backpack for an n-size S-arrangement and a K-size backpack and
adds to an array received as a parameter the items that fill
the backpack completely.

Parameters:
Q: P[n+1,K+1] with possible solutions to the problem of
backpack for an n-size S-arrangement and a K-size backpack.
items: pointer to the array that will contain the items that populate
the backpack.
S: an array of size n that stores item sizes.
n: array size S
K: backpack size

Return value: none, as desired values must be included
in the items array passed as a parameter.

*/

void selectBackpackItems(int *itens, cell_backpack **P, int *S, int n, int K)
{
    int i, k, index = 0;
    if (P[n][K].exists == false) // solution does not exist
        return;
    /* IMPLEMENT YOUR CODE */
    for (i = n; i >= 0; i--)
    {
        /* code */
        for (k = 0; k <= K; k++)
        {
            /* code */
            if (P[i][k].exists == true && P[i][k].belongs == true)
            {
                itens[index]=S[i-1];
                index++;
            }
        }
    }
}

/*
printMatrizP() was created to print in a "beautiful" way
the matrix P returned by the backpack() function
*/

void printMatrizP(cell_backpack **p, int *s, int n, int K)
{
    int i, j;

    printf("\n   P   ");
    for (i = 0; i < K + 1; i++)
        printf("  %d  ", i);
    printf("\n");

    for (i = 0; i < n + 1; i++)
    {
        if (i == 0)
            printf("k_0 = 0 ");
        else
            printf("k_%d =%2d ", i, s[i - 1]);
        for (j = 0; j < K + 1; j++)
        {
            printf("%c/", (p[i][j].exists) ? 'T' : 'F');
            printf("%c  ", (p[i][j].belongs) ? 'T' : 'F');
        }
        printf("\n");
    }
}

int main()
{
    int **grf_dist;
    int i, j, ini, fim, n = 7;
    int noErros = 0, noTestes = 0;

    // Prepare data for testing the dijkstra() function
    grf_dist = (int **)malloc(n * sizeof(int *));
    for (i = 0; i < n; ++i)
    {
        grf_dist[i] = (int *)malloc(n * sizeof(int));
    }

    for (i = 0; i < n; ++i)
        for (j = 0; j < n; ++j)
            grf_dist[i][j] = INT_MAX;

    grf_dist[0][1] = 6;
    grf_dist[0][2] = 8;
    grf_dist[0][3] = 7;
    grf_dist[1][3] = 100;
    grf_dist[1][4] = 99;
    grf_dist[2][5] = 1;
    grf_dist[3][2] = 200;
    grf_dist[3][4] = 400;
    grf_dist[3][5] = 500;
    grf_dist[4][6] = 200;
    grf_dist[5][4] = 600;
    grf_dist[5][6] = 1;

    // for(i = 0; i < n; ++i)
    //   {
    //     for(j = 0; j < n; ++j)
    //       printf("grf_dist[%d][%d] = %d ", i, j, grf_dist[i][j]);
    //     printf("\n");
    //    }

    noTestes++;
    ini = 0;
    fim = 6;
    printf("Distância menor de %d a %d: %d\n", ini, fim, dijkstra(grf_dist, n, ini, fim));
    if (dijkstra(grf_dist, n, ini, fim) == 10)
    {
        printf("Teste %d: OK\n", noTestes);
    }
    else
    {
        printf("Teste %d: NOK: ", noTestes);
        printf("Distância menor de %d a %d: %d\n", ini, fim, 10);
        noErros++;
    }
    noTestes++;
    ini = 0;
    fim = 5;
    printf("Distância menor de %d a %d: %d\n", ini, fim, dijkstra(grf_dist, n, ini, fim));

    if (dijkstra(grf_dist, n, ini, fim) == 9)
    {
        printf("Teste %d: OK\n", noTestes);
    }
    else
    {
        printf("Teste %d: NOK: ", noTestes);
        printf("Distância menor de %d a %d: %d\n", ini, fim, 9);
        noErros++;
    }

    noTestes++;
    ini = 0;
    fim = 4;
    printf("Distância menor de %d a %d: %d\n", ini, fim, dijkstra(grf_dist, n, ini, fim));

    if (dijkstra(grf_dist, n, ini, fim) == 105)
    {
        printf("Teste %d: OK\n", noTestes);
    }
    else
    {
        printf("Teste %d: NOK:", noTestes);
        printf("Distância menor de %d a %d: %d\n", ini, fim, 105);
        noErros++;
    }

    noTestes++;
    ini = -1;
    fim = 4;
    printf("Distância menor de %d a %d: %d\n", ini, fim, dijkstra(grf_dist, n, ini, fim));

    if (dijkstra(grf_dist, n, ini, fim) == 0)
    {
        printf("Teste %d: OK\n", noTestes);
    }
    else
    {
        printf("Teste %d: NOK:", noTestes);
        printf("Distância menor de %d a %d: %d\n", ini, fim, 0);
        noErros++;
    }

    noTestes++;
    ini = 0;
    fim = 14;
    printf("Distância menor de %d a %d: %d\n", ini, fim, dijkstra(grf_dist, n, ini, fim));

    if (dijkstra(grf_dist, n, ini, fim) == 0)
    {
        printf("Teste %d: OK\n", noTestes);
    }
    else
    {
        printf("Teste %d: NOK:", noTestes);
        printf("Distância menor de %d a %d: %d\n", ini, fim, 0);
        noErros++;
    }

    // Prepara dados para testar a função mochila() e selecionaItensMochila()
    noTestes++;
    int r[] = {2,3,5,6}; // itens a serem possivelmente salvos na celula_mochila
    int K = 9;
    n = 4;
    int itensmochila[] = {-1,-1,-1,-1};
    cell_backpack ** p = backpack(r,n,K);

    if(p != NULL)
    {
        printMatrizP(p,r,n,K);
        selectBackpackItems(itensmochila, p, r,n,K);

        printf("Itens mochila de tamanho 9: ");
        for(i=0; i < 4; ++i)
            if(itensmochila[i]!=-1)
                printf("%d ",itensmochila[i]);
        printf("\n");

        if(itensmochila[0] == 6 && itensmochila[1] == 3)
        {
            printf("Teste %d: OK\n", noTestes);
        }
        else {
            printf("Teste %d: NOK:", noTestes);
            printf("Itens mochila de tamanho 9: 6 3\n");
            noErros++;
        }
    }
    else {
        printf("Teste %d: NOK:", noTestes);
        printf("Itens mochila de tamanho 9: 6 3\n");
        noErros++;
    }

    noTestes++;
    int s[] = {1,2,3,5,6}; // itens a serem possivelmente salvos na celula_mochila
    K = 9;
    n = 5;
    int itensmochila2[] = {-1,-1,-1,-1,-1};
    p = backpack(s,n,K);

    if(p != NULL)
    {
        printMatrizP(p,s,n,K);
        selectBackpackItems(itensmochila2, p,s,n,K);
        printf("Itens mochila de tamanho 9: ");
        for(i=0; i < 5; ++i)
            if(itensmochila2[i]!=-1)
                printf("%d ",itensmochila2[i]);
        printf("\n");
        if(itensmochila2[0] == 5 && itensmochila2[1] == 3 && itensmochila2[2] == 1)
        {
            printf("Teste %d: OK\n", noTestes);
        }
        else {
            printf("Teste %d: NOK:", noTestes);
            printf("Itens mochila de tamanho 9: 5 3 1\n");
            noErros++;
        }
    }
    else {
        printf("Teste %d: NOK:", noTestes);
        printf("Itens mochila de tamanho 9: 5 3 1\n");
        noErros++;
    }

    noTestes++;
    int itensmochila3[] = {-1,-1,-1,-1,-1};
    p = backpack(s,n,7);
    if(p != NULL)
    {
        selectBackpackItems(itensmochila3, p,s,n,7);
        printf("Itens mochila de tamanho 7: ");
        for(i=0; i < 5; ++i)
            if(itensmochila3[i]!=-1)
                printf("%d ",itensmochila3[i]);
        printf("\n");
        if(itensmochila3[0] == 5 && itensmochila3[1] == 2)
        {
            printf("Teste %d: OK\n", noTestes);
        }
        else {
            printf("Teste %d: NOK:", noTestes);
            printf("Itens mochila de tamanho 7: 5 2\n");
            noErros++;
        }
    }
    else {
        printf("Teste %d: NOK:", noTestes);
        printf("Itens mochila de tamanho 7: 5 2\n");
        noErros++;
    }

// Testa verificação dos parâmetros
    noTestes++;
    p = backpack(s,0,K);
    if(p==NULL)
    {
        printf("Teste %d: OK\n", noTestes);
    }
    else{
        printf("Teste %d: NOK:", noTestes);
        printf("p == NULL\n");
        noErros++;
    }

    noTestes++;
    p = backpack(NULL,5,K);
    if(p==NULL)
    {
        printf("Teste %d: OK\n", noTestes);
    }
    else{
        printf("Teste %d: NOK:", noTestes);
        printf("p == NULL\n");
        noErros++;
    }

    noTestes++;
    p = backpack(s,5,-1);
    if(p==NULL)
    {
        printf("Teste %d: OK\n", noTestes);
    }
    else{
        printf("Teste %d: NOK:", noTestes);
        printf("p == NULL\n");
        noErros++;
    }

    return 0;
}