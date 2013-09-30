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
	return round(x);
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

	cout << "test" << endl;

	return 0;
}

