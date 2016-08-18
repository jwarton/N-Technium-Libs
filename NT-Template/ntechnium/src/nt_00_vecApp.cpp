#include "nt_00_vecApp.h"

void vec3TestApp::init() {

	std::cout << "TEST APPLICATION FOR OPERATOR OVERLOAD FUNCTIONALITY" << endl;

	std::cout << "---- INITIALIZED VALUES ----------------------------\n" << endl;

	A.print("A        ");
	B.print("B        ");
	C.print("C        ");
	D->print("D        ");
	E->print("E        ");

	std::cout << "\n" << endl;
	std::cout << "---- STANDARD ARGUMENT TYPE -------------------------\n" << endl;

	C = A;
	C.print("C = A    ");
	C = A + B;
	C.print("C = A + B");
	C = A - B;
	C.print("C = A - B");
	C = A * B;
	C.print("C = A * B");
	C = A / B;
	C.print("C = A / B");
	std::cout << "\n" << endl;

	A += B;
	A.print("A += B   ");
	A -= B;
	A.print("A -= B   ");
	A *= B;
	A.print("A *= B   ");
	A /= B;
	A.print("A /= B   ");
	std::cout << "\n" << endl;

	bool compare;

	compare = C == A;
	std::cout << "C == A :  " << compare << endl;
	compare = C <= A;
	std::cout << "C <= A :  " << compare << endl;
	compare = C >= A;
	std::cout << "C >= A :  " << compare << endl;
	compare = C < A;
	std::cout << "C <  A :  " << compare << endl;
	compare = C > A;
	std::cout << "C >  A :  " << compare << endl;
	std::cout << "\n" << endl;

	C = A;
	compare = C == A;
	std::cout << "C == A :  " << compare << endl;
	compare = C <= A;
	std::cout << "C <= A :  " << compare << endl;
	compare = C >= A;
	std::cout << "C >= A :  " << compare << endl;
	compare = C < A;
	std::cout << "C <  A :  " << compare << endl;
	compare = C > A;
	std::cout << "C >  A :  " << compare << endl;
	std::cout << "\n" << endl;

	std::cout << "---- POINTER ARGUMENT TYPE --------------------------\n" << endl;

	C = D;
	C.print("C = D    ");
	C = *D + E;
	C.print("C = D + E");
	C = *D - E;
	C.print("C = D - E");
	C = *D * E;
	C.print("C = D * E");
	C = *D / E;
	C.print("C = D / E");
	std::cout << "\n" << endl;

	*D += E;
	D->print("D += E   ");
	*D -= E;
	D->print("D -= E   ");
	*D *= E;
	D->print("D *= E   ");
	*D /= E;
	D->print("D /= E   ");
	std::cout << "\n" << endl;

	compare = *D == E;
	std::cout << "D == E :  " << compare << endl;
	compare = *D <= E;
	std::cout << "D <= E :  " << compare << endl;
	compare = *D >= E;
	std::cout << "D >= E :  " << compare << endl;
	compare = *D < E;
	std::cout << "D <  E :  " << compare << endl;
	compare = *D > E;
	std::cout << "D >  E :  " << compare << endl;
	std::cout << "\n" << endl;

	*D = E;
	compare = *D == E;
	std::cout << "D == E :  " << compare << endl;
	compare = *D <= E;
	std::cout << "D <= E :  " << compare << endl;
	compare = *D >= E;
	std::cout << "D >= E :  " << compare << endl;
	compare = *D < E;
	std::cout << "D <  E :  " << compare << endl;
	compare = *D > E;
	std::cout << "D >  E :  " << compare << endl;
	std::cout << "\n" << endl;

	//A + B = C;
	//C.print("C");
	//B.print("B");
	//A.print("A");
	//
	//std::cout << "\n" << endl;
	//C = A + B;
	//C.print("C");
	//B.print("B");
	//A.print("A");

	//std::cout << "\n" << endl;
	//A + B = C;
	//C.print("C");
	//B.print("B");
	//A.print("A");
}