#include "Matrix4.h"
#include "tests.h"
#include <cstdio>
//run the test suite
int main(int argc, char * argv[])
{
	printf("Running tests...\n");
	//run the matrix tests
	matrix4Tests();
	return 0;
}

bool matrix4Tests()
{
	printf("Testing Matrix4\n");
	allocTest();
	multiplyTest();
	if (compareTest())
	{
		printf("SUCCESSFULL\n");
	}
	else 
	{
		printf("UNSUCCESSFULL\n");
	}
	if (additionTest())
	{
		printf("SUCCESSFULL\n");
	}
	else 
	{
		printf("UNSUCCESSFULL\n");
	}
	if (additionAndSetTest())
	{
		printf("SUCCESSFULL\n");
	}
	else 
	{
		printf("UNSUCCESSFULL\n");
	}
	if (vertexMultiplyTest())
	{
		printf("SUCCESSFULL\n");
	}
	else 
	{
		printf("UNSUCCESSFULL\n");
	}

	return true;
}
bool allocTest()
{
	printf("Alloc test\n");
	const float data[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	/*for (int i = 0; i < 16; i++)
	{
		data[i] = i;
	}*/
	Matrix4 mat(data);
	mat.print();
	return true;
}

bool multiplyTest()
{
	printf("Multiply test\n");
	const float data1[16] = {1, 3, 0, 2, 2, 8, 0, 12, 5, 9, 6, 7, 3, 1, 7, 9};
	const float data2[16] = {8, 2, 0, 9, 9, 5, 1, 1, 1, 2, 2, 9, 4, 5, 7, 82};
	Matrix4 m1(data1);
	Matrix4 m2(data2);
	Matrix4 result = m1 * m2;
	m1.print();
	printf("\n");
	m2.print();
	printf("\n");
	result.print();
	return true;
}

bool compareTest()
{
	printf("Compare test...\n");
	bool res = true;
	const float data1[16] = {1, 3, 0, 2, 2, 8, 0, 12, 5, 9, 6, 7, 3, 1, 7, 9};
	Matrix4 m1(data1);
	Matrix4 m2(data1);
	res = m1 == m2;
	const float data2[16] = {8, 2, 0, 9, 9, 5, 1, 1, 1, 2, 2, 9, 4, 5, 7, 82};
	Matrix4 m3(data2);
	if (m1 == m3)
		res = false;
	return res;
}

bool additionTest()
{
	bool res = true;
	printf("Addition test\n");
	const float data1[16] = {1, 3, 0, 2, 2, 8, 0, 12, 5, 9, 6, 7, 3, 1, 7, 9};
	Matrix4 m1(data1);
	const float data2[16] = {2, 1, 10, 5, 3, 8, 20, 5, 1, 1, 1, 4, 0, 9, 27, 1};
	Matrix4 m2(data2);
	const float correctResult[16] = {3, 4, 10, 7, 5, 16, 20, 17, 6, 10, 7, 11,3, 10, 34, 10};
	Matrix4	correctResultM(correctResult);
	res = ((m1 + m2) == correctResultM);
	return res;
}

bool additionAndSetTest()
{
	printf("Addition and set test\n");
	bool res = true;
	const float data1[16] = {1, 3, 0, 2, 2, 8, 0, 12, 5, 9, 6, 7, 3, 1, 7, 9};
	Matrix4 m1(data1);
	const float data2[16] = {2, 1, 10, 5, 3, 8, 20, 5, 1, 1, 1, 4, 0, 9, 27, 1};
	Matrix4 m2(data2);
	const float correctResult[16] = {3, 4, 10, 7, 5, 16, 20, 17, 6, 10, 7, 11,3, 10, 34, 10};
	Matrix4 correctResultM(correctResult);
	m1 += m2;
	res = (m1 == correctResultM);
	return res;
}
bool vertexMultiplyTest()
{
	printf("Vertex Multiply test\n");
	bool result = true;
	const float data1[16] = {1, 3, 0, 2, 2, 8, 0, 12, 5, 9, 6, 7, 3, 1, 7, 9};
	Matrix4 m1(data1);
	Vertex vec(0, 2, 3, 9);
	Vertex res = m1 * vec;
	Vertex correct(46, 52, 81, 126);
	res.print();
	for (int i = 0; i < 4; i++)
	{
		if (res.v[i] != correct.v[i])
		{
			result = false;
			break;
		}
	}
	return result;
}
