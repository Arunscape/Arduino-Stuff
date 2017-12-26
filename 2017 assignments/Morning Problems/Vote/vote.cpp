#include <iostream>

using namespace std;

int main(){
  int n = 0;
  int vote[1001];
  // keep reading until we see 0
  while (true){
    cin >> vote[n];
    if (vote[n] == 0){
      break;
    }
    ++n;
  }

  int votecount[1001]={0};

  for (int i=0; i < n; i++){
    votecount[vote[i]]++;
  }

//get maxperson
  int max=0;
  int maxperson;
  for (int i=1; i<=1000; i++){
    if (votecount[i] > max){
      max = votecount[i];
      maxperson = i;
    }
  }

  //check for tie
  bool tie = 0;
  for (int i=1; i <=1000; i++) {
    if (votecount[i] == max and i != maxperson) {
      tie = 1;
    }
  }

  if (tie){
    cout << "tie" << endl;
  }
  else{
    cout << maxperson << endl;
  }

  return 0;
}
