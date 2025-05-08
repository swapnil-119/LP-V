#include <iostream>
#include<stdio.h>
#include <vector>
#include <stack>
//****important to add following library to allow a programmer to use parallel paradigms*****
#include <omp.h>


using namespace std;

// Depth First Search (DFS) function to traverse the graph recursively
void dfs(int start, int* arr, int n, int visited[]) {
    // OpenMP parallel for loop to iterate through all vertices
    #pragma omp parallel for ordered
    for(int i = 0; i < n; i++) {
        // Ordered directive ensures ordered execution of the loop iterations
        #pragma omp ordered
        // Check if there is an edge from 'start' to 'i' and 'i' is not visited yet
        if (*(arr + (start * n) + i) == 1 && !visited[i]) {
            // Mark vertex 'i' as visited, print it, and recursively call DFS on 'i'
            visited[i] = 1;
            cout << i << " ";
            dfs(i, (int*)arr, n, visited);
        }
    }
}

int main() {
    cout << "Enter the number of vertices: ";
    int n;
    cin >> n;
	
    int arr[n][n] = {0}; // Initialize the adjacency matrix with zeros

    cout << "Enter the number of edges: ";
    int edges;
    cin >> edges;

    // Input the edges and update the adjacency matrix
    for(int j = 0; j < edges; j++) {
        int a, b;
        cout << "Enter the two edges:" << endl;
        cin >> a >> b;
        arr[a][b] = 1; // Mark the edges as connected
        arr[b][a] = 1; // Since it's an undirected graph, mark both directions
    }

    int visited[n] = {0}; // Initialize the visited array to keep track of visited vertices

    cout << "Enter the start vertex: ";
    int start;
    cin >> start;


    //clock_t strt = clock(); // Record the start time for measuring execution time
    
    cout << start << " "; // Print the start vertex
    visited[start] = 1; // Mark the start vertex as visited
    dfs(start, (int *)arr, n, visited); // Perform DFS traversal from the start vertex
    
    //clock_t stop = clock(); // Record the stop time for measuring execution time

    // Calculate and print the execution time
    //cout << "\nTime required: " << (double)(stop - strt) << " ms" << endl;

    return 0;
}


/*
g++ par_dfs.cpp -fopenmp
*/