#include <iostream>
#include <string>
using namespace std;

int main() {
  int year;
  cin >> year;

 string ans;

  if ( (year%4==0 && year%100!=0) or (year%400==0) ){
    ans = "yes";
  }
  else{ans="no";}



  cout << ans << endl;

  return 0;
}
