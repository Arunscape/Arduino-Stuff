#include <iostream>

using namespace std;

int main() {
  int low1, high1, low2, high2;
  cin >> low1 >> high1 >> low2 >> high2;
  bool intersect = 1;




 //    for (int j=low2; j<=high2; j++){
 //      if(i==j){
 //        intersect = 1;
 //        break;
 //      }
 //    }
 //  }
 //the code above is too slow
  // now solve the problem and print the answer

  if(high1<low2 or high2<low1){
    intersect=0;
  }




  if(intersect){
    cout<< "intersect" << endl;
  }

  else{
    cout<< "disjoint" << endl;
  }
  return 0;
}
