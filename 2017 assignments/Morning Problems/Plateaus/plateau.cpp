#include <iostream>
#include <algorithm> // for max()

using namespace std;

int main() {
  int n;
  cin >> n;

  int array[100000]={0};
  int maxplateau=0;
  int plateau=0;

  for (int i=0;i<n;i++){
    cin >> array[i];
  }



  for(int i=0;i<n-1;i++){
    if(array[i]==array[i+1]){
      plateau++;
    }

    else{
      plateau=0;
    }

    if (plateau > maxplateau){
      maxplateau=plateau;
    }
  }

//literally just changed code from falling but idk why here I need to add one
//lol
  cout << maxplateau+1 << endl;

  return 0;
}
