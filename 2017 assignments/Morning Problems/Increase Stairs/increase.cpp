#include <iostream>

using namespace std;

int main() {
  int n;
  int array[1000];

  cin >> n;
  for (int i = 0; i < n; ++i) {
    cin >> array[i];
  }
  int counter=0;
  // now compute and print the answer
  for (int j=0; j<n-1; j++){
     if(array[j]==array[j+1]){
      ;
    }
    else if(array[j]>array[j+1]){
      ;
    }
    else if (array[j]<array[j+1]){
      counter+= array[j+1]-array[j];
    }
  }
  cout << counter << endl;
  return 0;
}
