#include <iostream>
using namespace std;

void Swap(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition3(int arr[], int left, int right)
{
    //前后下标法,基准值还是取最右
    int div = left;
    for(int i = left; i < right; i++){
	if(arr[i] < arr[right]){
	    Swap(arr + i, arr + div);
	    div++;
	}
    }
    Swap(arr + div, arr + right);
    return div;
}

int partition2(int arr[], int left, int right)
{
    //挖坑法
    int begin = left;
    int end = right;
    int pivot = arr[right];
    while(begin < end){
	while(begin < end && arr[begin] <= pivot){
	   begin++; 
	}
	arr[end] = arr[begin];
	while(begin < end && arr[end] >= pivot){
	    end--;
	}
	arr[begin] = arr[end];
    }
    arr[begin] = pivot;
    return begin;
}

int partition1(int arr[], int left, int right)
{
    //hover
    int begin = left;
    int end = right;
    while(begin < end){
	while(begin < end && arr[begin] <= arr[right]){
	    begin++;
	}
	while(begin < end && arr[end] >= arr[right]){
	    end--;
	}
	Swap(arr + begin, arr + end);
    }
    Swap(arr + begin, arr + right);
    return begin;
}

void _QuickSort(int arr[], int left, int right)
{
    if(left >= right){
	//区间内只有一个数或没有数
	return;
    }
    //int div = partition1(arr, left, right);
    //int div = partition2(arr, left, right);
    int div = partition3(arr, left, right);
    _QuickSort(arr, left, div - 1);
    _QuickSort(arr, div + 1, right);
}

void QuickSort(int arr[], int len)
{
    _QuickSort(arr, 0, len - 1);
}

int main()
{
    int arr[] = {2,3,52,1,4,5,2,4,6,8,3,21};
    int len = sizeof(arr) / sizeof(arr[0]);
    QuickSort(arr, len);
    for(int i = 0; i < len; i++){
	cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}
