#include <iostream>
using namespace std;

void Swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void adjustDown(int arr[], int size, int root)
{
    int left = 2 * root + 1;
    int right = 2 * root + 2;
    int max;
    //左孩子不存在
    if(left >= size){
	return;
    }
    //右孩子存在且大于左孩子
    if(right < size && arr[right] > arr[left]){
	max = right;
    }else {
	max = left;
    }

    if(arr[max] > arr[root]){
	Swap(arr + max, arr + root);
    }
    adjustDown(arr, size, max);
}

void createHeap(int arr[], int size)
{
    for(int i = (size - 1 - 1) / 2; i >= 0; i--){
	adjustDown(arr, size, i);
    }
}

void heapSort(int arr[], int size)
{
    createHeap(arr, size);
    for(int i = 0; i < size; i++){
	Swap(arr, arr + size - i - 1);	//分治的思想，没调整一次，就少一个元素，排过的就已经有序了
	adjustDown(arr, size - i - 1, 0);
    }
}

int main()
{
    int arr[] = { 7, 5, 4, 3, 4, 6, 5, 63, 54, 2, -1 };
    int size = sizeof(arr) / sizeof(arr[0]);

    heapSort(arr, size);

    for(int i = 0; i < size; i++){
	cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}
