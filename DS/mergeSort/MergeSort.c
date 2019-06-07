#include<stdio.h>
#include <stdlib.h>
void Merge(int arr[], int left, int mid, int right, int extra[])
{
    int size = right - left;
    int left_index = left;
    int right_index = mid;
    int extra_index = 0;

    while(left_index < mid && right_index < right){
	if(arr[left_index] <= arr[right_index]){ 
	    extra[extra_index] = arr[left_index];
	    left_index++;
	}
	else{
	    extra[extra_index] = arr[right_index];
	    right_index++;
	}
	extra_index++;
    }

    while(left_index < mid){
	extra[extra_index++] = arr[left_index++];
    }

    while(right_index < right){
	extra[extra_index++] = arr[right_index++];
    }
    
    int i;
    for(i = 0; i < size; i++){
	arr[left + i] = extra[i];
    }
}

void __MergeSort(int arr[], int left, int right, int extra[])
{
    if(left >= right){
	return;	//区间内没有数了，返回。
    }
    if(left + 1 == right){
	return;	//区间只有一个数，满足有序。
    }

    int mid = left + (right - left) / 2;
    __MergeSort(arr, left, mid, extra);
    __MergeSort(arr, mid, right, extra);
    Merge(arr, left, mid, right, extra);
}

void MergeSort(int arr[], int size)
{
    int *extra = (int *)malloc(sizeof(int) * size);
    __MergeSort(arr, 0, size, extra);
    free(extra);
}

int main()
{
    int arr[] = {9, 5, 2, 7, 3, 6, 8, 1};
    int size = sizeof(arr) / sizeof(arr[0]);
    MergeSort(arr, size);
    int i;
    for(i = 0; i < size; ++i){
	printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}
