#include <iostream>

using namespace std;

int main() {


 int digits;
  //unsigned int sum=0;
  //unsigned int carry=0;

  cin >>digits;

 int num1[digits];
 int num2[digits];

   int numout[digits];

  for( int i=0; i<digits;i++){
    cin>>num1[i];
  }
  for( int i=0; i<digits;i++){
    cin>>num2[i];
  }

  for( int i=digits-1;i>-1;i--){
    //cout << "i " << "is" << i << endl;
    if(num1[i]>=num2[i]){
      numout[i]=num1[i]-num2[i];
    }
    else{
      num1[i-1]--;
      num1[i]+=10;
      numout[i]=num1[i]-num2[i];
    }
  //  numout[i]=sum;
    //cout << "number " << i << " is " << sum << endl;
  }
  //numout[0]=carry;


  for( int i=0;i<digits;i++){
    cout << numout[i] << " ";
  }




  return 0;
}
