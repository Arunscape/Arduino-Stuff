#include <iostream>

using namespace std;

int main() {
  int d1, d2, d3, d4;

  cin >> d1 >> d2 >> d3 >> d4;

  if ((d1 + d2*2+ d3*3+ d4*4)%5 ==0){
    cout << "yes" << endl;
  }
  else{ cout << "no" << endl; }



  return 0;
}
