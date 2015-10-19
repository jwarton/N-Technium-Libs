#include "nt_08_afireApp-v01.h"

void afApp01::init() {
	
	printf("\n\n");
	printf("///////////////////////////////////////////////////////////////\n");
	printf("/////////////////////////////////////////// AF TEST APPLICAION:\n\n");
	
	printf("CREATE A 5-BY-3 MATRIX OF RANDOM floatS ON THE GPU\n");
	af::array A = randu(5, 3, f32);
	af_print(A);
	
	printf("ELEMENT-WISE ARITHMETIC\n");
	af::array B = sin(A) + 1.5;
	af_print(B);

	printf("NEGATE THE FIRST THREE ELEMENTS OF SECOND COLUMN\n");
	B(seq(0, 2), 1) = B(seq(0, 2), 1) * -1;
	af_print(B);

	printf("FOURIER TRANSFORM THE RESULT\n");
	af::array C = fft(B);
	af_print(C);

	printf("GRAB LAST ROW\n");
	af::array c = C.row(af::end);
	af_print(c);

	printf("CREATE 2-BY-3 MATRIX FROM HOST DATA\n");
	float d[] = { 1, 2, 3, 4, 5, 6 };
	af::array D(2, 3, d, afHost);
	af_print(D);

	printf("COPY LAST COLUMN ONTO FIRST\n");
	D.col(0) = D.col(af::end);
	af_print(D);

	// Sort A
	printf("SORT A AND PRINT SORTED ARRAY AND CORRESPONDING INDICES\n");
	af::array vals, inds;
	sort(vals, inds, A);
	af_print(vals);

	af_print(inds);

	printf("///////////////////////////////////////////////////////////////\n\n");
}