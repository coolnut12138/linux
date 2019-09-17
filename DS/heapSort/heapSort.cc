#include <iostream>
using namespace std;

void Swap(int* a, int* b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void AdjustDown(int arr[], int size, int root){
    int left = 2 * root + 1;
    int right = 2 * root + 2;
    int max;
    if(left >= size){
	return;
    }
    if(right < size && arr[right] > arr[left]){
	max = right;
    }else{
	max = left;
    }

    if(arr[root] >= arr[max]){
	return;
    }
    Swap(arr + root, arr + max);
    AdjustDown(arr, size, max);
}

void CreateHeap(int arr[], int size){
    for(int i = (size - 1 - 1) / 2; i >= 0; i--){
	AdjustDown(arr, size, i);
    }
}

void HeapSort(int arr[], int size){
    CreateHeap(arr, size);
    for(int i = 0; i < size; i++){
	Swap(arr, arr + size - 1 - i);
	AdjustDown(arr, size - 1 - i, 0);
    }
}

int main()
{
    int arr[] = { 7, 5, 4, 3, 4, 6, 5, 63, 54, 2, -1 };
    int size = sizeof(arr) / sizeof(arr[0]);

    HeapSort(arr, size);

    for(int i = 0; i < size; i++){
	cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}
