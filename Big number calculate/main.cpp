#include "inf_int.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

int main()
{
	inf_int a;
	inf_int b(100);
	inf_int c("145315");
	inf_int d("195152611341515331616136");;
	inf_int e = c + d;
	inf_int f("1111111111111111111111111111111111111111111111111111");
	inf_int g("1111111111111111111111111111111111111111111111111121");
	inf_int h = f - g;
	inf_int i = g - f;
	inf_int j("333333333333333333333333");
	inf_int k("73755");
	inf_int l("-73755");
	inf_int m = j * k;
	inf_int n = j * l;
	bool o = f < g;
	bool p = k < l;
	inf_int q("12345678901234567890");
	inf_int r("-123456789012345678901");
	inf_int s = q - r;
	inf_int t = r - q;
	inf_int u = q + r;
	inf_int v = q * a;
	
		cout << "a : " << a << endl;
		cout << "b : " << b << endl;
		cout << "c : " << c << endl;
		cout << "d : " << d << endl;
		cout << "c+d = e : " << e << endl;
		cout << "f : " << f << endl;
		cout << "g : " << g << endl;
		cout << "f-g = h : " << h << endl;
		cout << "g-f = i : " << i << endl;
		cout << "j : " << j << endl;
		cout << "k : "  << k << endl;
		cout << "l : " << l << endl;
		cout << "j*k = m : " << m << endl;
		cout << "j*l = n : " << n << endl;
		cout << "f < g = o : " << o << endl;
		cout << "k < l = p : " << p << endl;
		cout << "q : " << q << endl;
		cout << "r : " << r << endl;
		cout << "q-r = s : " << s << endl;
		cout << "r-q = t : " << t << endl;
		cout << "q+r = u : " << u << endl;
		cout << "a*q = v : " << v << endl;

	return 0;
}
