/*
 * build.cpp
 *
 *  Created on: 23.9.2013
 *      Author: jancajthaml
 */

#include <iostream>;
#include <cmath>
#include <limits>

using namespace std;

double r(double x)
{
	return double((x>=0.5)?(int(x)+1):int(x));
}

double s(double x)
{
	if(x>=0.5){return ceil(x);}else{return floor(x);}
}

void test(char* name, double (*f)(double))
{
	int it = std::numeric_limits<int>::max()>>2;

	clock_t begin = clock();

	for(int i=0; i<it; i++)
	{
		f(double(i)/1000.0);
	}
	clock_t end = clock();

	cout << "test " << name << " " << double(end - begin) / CLOCKS_PER_SEC << endl;

}

int main(int argc, char **argv)
{

	test("r1",r);
	test("s1",s);
	test("r2",r);
	test("s2",s);
	test("r3",r);
	test("s3",s);
	return 0;
}

