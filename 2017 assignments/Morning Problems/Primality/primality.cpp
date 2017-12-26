#include <iostream>
#include <cmath> // for sqrt

using namespace std;

// given a positive integer n <= 4,000,000 this returns the smallest
// integer d such that d*d > n
//
// not needed to solve the problem, but some might find it helpful
unsigned int upper_sqrt(unsigned int n) {
  unsigned int d(sqrt((double) n));

  // should iterate at most one or twice
  while (d*d <= n) {
    ++d;
  }

  return d;
}

int main() {
  // read in the integer
  bool prime =1;
  unsigned int n;
  cin >> n ;
  // NOTE: look *carefully* at the maximum value of the number
  // in the problem description

  // see if the integer it is prime

  // print the answer


  for( unsigned int i = 2; i <= upper_sqrt(n); i++){
    if(n % i == 0){
      prime = false;
      break;
    }
  }

  if(n==1){
    prime=0;
  }

  if(n==2){
    prime=1;
  }




  if (prime){
    cout << "prime" << endl;
  }
  else{
    cout << "not prime" << endl;
  }
  return 0;
}
