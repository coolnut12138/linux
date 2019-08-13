#include <iostream>
using namespace std;

void Swap(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void selectSort(int arr[], int size)
{
    for(int i = 0; i < size; i++){
	int max = 0;	//下标
	for(int j = 0; j < size - i; j++){
	    if(arr[j] > arr[max]){
		max = j;
	    }
	}
	Swap(arr + max, arr + size - i - 1);
    }
}

int main()
{
    int arr[] = {2,3,52,1,4,5,2,4,6,8,3,21};
    int size = sizeof(arr) / sizeof(arr[0]);
    selectSort(arr, size);
    for(const auto& e : arr){
	cout << e << " ";
    }
    cout << endl;
    return 0;
}
