
#define CHECK_BOUNDS
#define CHECK_DIMENTIONS

#include <iostream>
#include <assert.h>
#include "macierz.h"


int main()
{
	//macierz A(1, 2), B(2, 1), C(1, 1), D(1,1);
	macierz f({}, {});
	f.print();

	//A.set(0, 0, 1.0);
	//A.set(0, 1, 2.0);


	macierz F = { {},{} }; //{ {1.0,2.0,3.0} , {1.0,2.0,3.0} , {1.0,2.0,3.0} };
	//F.print();

	//B.set(0, 0, 3.0);
	//B.set(1, 0, 2.0);

	//C.set(0, 0, 7.0);

	//A.print();
	//B.print();
	//C.print();
	//D.print();

	//D = A * B;

	//D.print();

	//if (!(C == D)) std::cout << "nie rowne";
	
	return 0;
}

