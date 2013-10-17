
#include <assert.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <stdio.h>
#include "Context.h";

using namespace std;



int main()
{

	float f = 5.0f;

	cout << f << "A" << endl;

	int* i = (int*)&f;
	*i += 0x00800000;

	cout << f << "B" << endl;



    return 0 ;
}
