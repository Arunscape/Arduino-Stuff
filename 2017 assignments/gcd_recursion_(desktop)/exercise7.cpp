#include <iostream>
#include "gcd.h"
using namespace std;

int main(){
  unsigned int num1, num2;

  //prompt:
  cout << "Enter two nonnegative integers (at least one being positive): ";

  cin >> num1 >> num2; //input numbers

  cout << "The greatest common divisor of " << num1 << " and " << num2 << " is: "
  << get_gcd(num1,num2) << "." << endl; //output result

  return 0;
}
