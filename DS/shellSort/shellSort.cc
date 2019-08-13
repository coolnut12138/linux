#include <iostream>
using namespace std;

void insertSortWithGap(int arr[], int size, int gap){
    for(int i = gap; i < size; i++){
	int key = arr[i];
	int j;
	for(j = i - gap; j >= 0 && arr[j] > key; j -= gap){
	    arr[j + gap] = arr[j];
	}
	arr[j+gap] = key;
    }
}

int main()
{
    int arr[] = {2,3,52,1,4,5,2,4,6,8,3,21};
    int size = sizeof(arr) / sizeof(arr[0]);

    int gap = size;
    while(1){
	gap = (gap / 3) + 1;
	insertSortWithGap(arr, size, gap);
	if(gap == 1){
	    break;
	}
    }
    for(const auto& e : arr){
	cout << e << " ";
    }
    cout << endl;
    return 0;
}
