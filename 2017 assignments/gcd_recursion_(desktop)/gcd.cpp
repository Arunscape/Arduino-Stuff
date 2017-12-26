unsigned int get_gcd(unsigned int a, unsigned int b){
	if(b==0){return a;}
	else{return get_gcd(b,a%b);}
}

/*
the function above is based on euclid's algorithm, which the worksheet helped
with. Basically, if b is zero, then gcd(a,b)=a
otherwise, gcd(a,b)=gcd(b,a mod b)
the function will recursively call itself until a%b=0, (input argument b in the
next recursively called function=0), and return a
*/
