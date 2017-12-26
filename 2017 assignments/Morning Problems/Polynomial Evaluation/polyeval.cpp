#include <iostream>
#include <cmath>
using namespace std;

int main() {
  int power;
  int eval;
  int modulo;

 cin >> power;
 cin >> eval;
 cin >> modulo;

//debugging

//cout << power << " " << eval << " " << modulo << endl;


 int coeff[power]={0};
 int pows[power]={0};
 long int ans=0;

 pows[0]=1;

 for (int i=0; i<=power;i++){
   cin >> coeff[i];

   if (i> 0){
     pows[i]=(pows[i-1]*eval % modulo);
   }

   pows[i]=pows[i] %modulo;
   //cout<< pows[i];
 }

//cout << endl;

//debug
 // for (int i=0; i<=power;i++){
 //    cout<< coeff[i];
 // }

 // cout << endl;

 // long int temp=0;
 // for (int i=0;i<=power;i++){
 //    temp=coeff[i]* (long int)pow(eval,i) % modulo;
 //    temp = temp % modulo;
 //    ans+= temp;
 //    ans=ans%modulo;
 //    cout << i << " " << temp << " " << ans << endl;
 // }

 for (int i=0; i<=power; i++){
   ans += coeff[i]*pows[i] %modulo;
 }


  cout << ans % modulo << endl;
  return 0;
}
