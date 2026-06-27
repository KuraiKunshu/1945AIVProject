```c

        5
     /     \
   1 3     7 9 11
            /  \
          7     11 

 H = altezza (profondita)  //   2
 N = numero di nodi        //   6

 Max(N) = (2^H+1) - 1;     // 2^(2+1) - 1 = 7\
 Min(H) = log2(N + 1) - 1; // log2(7 + 1) - 1 = 2

#include <stdio.h>

int binary_search_recursive(int array[], int start, int end, int target) { // O(LogN)
    if (start > end) return -1;  // element not found

    int mid = start + (end - start) / 2;

    if (array[mid] == target)
        return mid;  // element found
    else if (array[mid] < target)
        return binary_search_recursive(array, mid + 1, end, target);  // search right half
    else
        return binary_search_recursive(array, start, mid - 1, target); // search left half
}

int main() {
    int array[] = {1, 3, 5, 7, 9, 11};
    int n = sizeof(array) / sizeof(int);
    int target = 7;

    int index = binary_search_recursive(array, 0, n - 1, target);
    if (index != -1)
        printf("Element found at index %d\n", index);
    else
        printf("Element not found\n");

    return 0;
}


```