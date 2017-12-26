#include <iostream>
#include <algorithm> // for max()

using namespace std;

int main() {
  // declare your variables
  int n;

  cin >> n;

  int steps[n]={0};
  int maxtumble=0;
  int tumble=0;

  for(int i=0;i<n;i++){
    cin >> steps[i];
  }

  for(int i=0;i<n-1;i++){
    if(steps[i]>steps[i+1]){
      tumble+= steps[i]-steps[i+1];
    }

    else{
      tumble=0;
    }

    if (tumble > maxtumble){
      maxtumble=tumble;
    }
  }

  cout << maxtumble << endl;

  return 0;
}
