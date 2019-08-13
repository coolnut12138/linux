#include <iostream>
using namespace std;

void Swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void bubbleSort(int arr[], int size)
{
    for(int i = 0; i < size; i++)
    {
	for(int j = 0; j < size - i - 1; j++){
	    if(arr[j] > arr[j+1]){
		Swap(arr + j, arr + j + 1);
	    }
	}
    }
}

int main()
{
    int arr[] = {2,3,52,1,4,5,2,4,6,8,3,21};
    int len = sizeof(arr) / sizeof(arr[0]);
    bubbleSort(arr, len);
    for(int i = 0; i < len; i++){
	cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}
