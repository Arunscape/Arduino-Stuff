#include <iostream>

using namespace std;

int main() {
//   int digits;
//   cin >> digits;
//
//
//   unsigned int num1[digits];
//   for(int i=0; i<digits;i++){
//     cin >> num1[i];
//   }
//
//   unsigned int num2[digits];
//   for(int i=0; i<digits;i++){
//     cin >> num2[i];
//   }
//
//   //add the numbers together
//   unsigned int numout[digits+1];
//
//   unsigned int tempnum=0;
//   unsigned int carryover=0;
//
//   for(int i=digits-1; i>-1; i--){
//     tempnum=num1[i]+num2[i];
//     if(tempnum>9){
//       carryover=1;
//       tempnum-=10;
//       numout[i+1]+=carryover;
//     }
//
//     else{
//     carryover=0;
//     }
//     numout[i]+=tempnum;
//     // /cout >> tempnum;
//
//   }
//
//     for(int i=0; i<digits;i++){
//       cout << numout[i] << " ";
//     }
//     cout << endl;
// //
//
// //tetsing
//
//   cout << "num1: " ;
//   for(int i=0; i<digits;i++){
//     cout << num1[i];
//   }
//   cout << endl;
//
//   cout << "num2: " ;
//   for(int i=0; i<digits;i++){
//     cout << num2[i];
//   }
//   cout << endl;
//
//
//   //cout << numout << endl;

  int digits;
  int sum=0;
  int carry=0;

  cin >>digits;

  int num1[digits];
  int num2[digits];

  int numout[digits+1];

  for(int i=0; i<digits;i++){
    cin>>num1[i];
  }
  for(int i=0; i<digits;i++){
    cin>>num2[i];
  }

  for(int i=digits-1;i>-1;i--){
    sum=num1[i]+num2[i]+carry;
    carry=sum/10;
    numout[i+1]=sum%10;
  }
  numout[0]=carry;


  for(int i=0;i<digits+1;i++){
    cout << numout[i] << " ";
  }




  return 0;
}
