#ifndef ford_fulkerson_h
#define ford_fulkrson_h

#include <iostream>
#include <vector>
#include <fstream>
#include <limits.h>
#include <string.h>
#include <queue>
#include <stdlib.h>
#include <stdio.h>

#define V 9

bool bfs(int rGraph[V][V], int s, int t, int parent[]);
int fordFulkerson(int graph[V][V], int s, int t);
#endif
