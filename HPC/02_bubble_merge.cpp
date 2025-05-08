#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>
#include <iomanip>  // for setprecision

using namespace std;

// Parallel Bubble Sort using odd-even transposition
void parallelOddEvenBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        #pragma omp parallel for
        for (int j = (i % 2); j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Merge function
void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> temp;
    int left = l, right = m + 1;

    while (left <= m && right <= r) {
        if (arr[left] <= arr[right]) {
            temp.push_back(arr[left++]);
        } else {
            temp.push_back(arr[right++]);
        }
    }

    while (left <= m) temp.push_back(arr[left++]);
    while (right <= r) temp.push_back(arr[right++]);

    for (int i = l; i <= r; i++) {
        arr[i] = temp[i - l];
    }
}

// Parallel Merge Sort
void mergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSort(arr, l, m);
            #pragma omp section
            mergeSort(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter the elements: ";
    for (int i = 0; i < n; i++) cin >> arr[i];

    vector<int> arrBubble = arr;
    vector<int> arrMerge = arr;

    // Time Bubble Sort
    clock_t bubbleStart = clock();
    parallelOddEvenBubbleSort(arrBubble);
    clock_t bubbleEnd = clock();

    // Time Merge Sort
    clock_t mergeStart = clock();
    mergeSort(arrMerge, 0, n - 1);
    clock_t mergeEnd = clock();

    // Print sorted arrays
    cout << "Sorted array using Bubble Sort: ";
    for (int num : arrBubble) cout << num << " ";
    cout << endl;

    cout << "Sorted array using Merge Sort: ";
    for (int num : arrMerge) cout << num << " ";
    cout << endl;

    // Calculate durations
    double bubbleDuration = double(bubbleEnd - bubbleStart) / CLOCKS_PER_SEC;
    double mergeDuration = double(mergeEnd - mergeStart) / CLOCKS_PER_SEC;

    // Print time with required formatting
    cout << fixed << setprecision(5);
    cout << "Bubble sort time in seconds: " << bubbleDuration << endl;
    cout << "Merge sort time in seconds: " << mergeDuration << endl;

    return 0;
}


//OP:-
// Enter the number of elements: 6
// Enter the elements: 9 2 5 1 3 7
// Sorted array using Bubble Sort: 1 2 3 5 7 9 
// Sorted array using Merge Sort: 1 2 3 5 7 9 
// Bubble sort time in seconds: 0.00000
// Merge sort time in seconds: 0.00001