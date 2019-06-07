#include <iostream>
#include <stack>
using namespace std;

void Swap(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
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

void QuickSortNor(int arr[], int size)
{
   stack<int> st;
   st.push(size - 1);
   st.push(0);
   while(!st.empty()){
	int left = st.top();
	st.pop();
	int right = st.top();
	st.pop();
	if(left >= right){
	    continue;
	}else{
	    int d = partition1(arr, left, right);
	    st.push(right);
	    st.push(d + 1);
	    st.push(d - 1);
	    st.push(left);
	}
   }
}

int main()
{
    int arr[] = {2,3,52,1,4,5,2,4,6,8,3,21};
    int len = sizeof(arr) / sizeof(arr[0]);
    QuickSortNor(arr, len);
    for(int i = 0; i < len; i++){
	cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}
