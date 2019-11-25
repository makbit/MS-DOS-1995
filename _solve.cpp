#include <iostream.h>
#include "_system.h"


void main()
{
 Simplex sim(3, 2);

 cin >> sim;
 sim.Solve();
 cout << sim;

}