
#include <assert.h>
#include <algorithm>
#include <vector>
#include <iostream>

#include "Context.h";

using namespace std;

int main() {
    Matrix a, b, c;

    a[0] = 13;
    cout << a[0] << endl;
    assert(a[0] == 13);

    assert(b[0] == 0);
    b = a;
    assert(b[0] == 13);
    b *= a;
    assert(b[0] == 13*13);
    b += a;
    assert(b[0] == 13*13+13);
    b -= a;
    assert(b[0] == 13*13);

    Matrix d = a + b;
    assert(a[0] == 13);
    assert(b[0] == 13*13);
    assert(d[0] == 13*13+13);

	return 0;
}
