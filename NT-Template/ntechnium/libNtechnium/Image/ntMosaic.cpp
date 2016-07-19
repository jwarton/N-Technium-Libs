#include "ntMosaic.h"

using namespace arma;
using namespace std;
using namespace af;

ntMosaic::ntMosaic(){}
ntMosaic::ntMosaic(std::string url_IMG, std::string path_Tiles):
	url_IMG(url_IMG), path_Tiles(path_Tiles)
{
	t_LOAD = clock();
	///
	const char * file = url_IMG.c_str();
	matrix = af::loadImage(file, false);
	///
	t_LOAD = clock() - t_LOAD;
	print_T(t_LOAD, "LOAD IMAGE   ");

	dim_X = matrix.dims(1);
	dim_Y = matrix.dims(0);

	init();
}
void ntMosaic::init() {
	//int scFx = 1;
	//matrix = af::resize(matrix, dim_Y * scFx, dim_X * scFx);
	dim_X = matrix.dims(1);
	dim_Y = matrix.dims(0);
	//img_00->set_Dim(500);
	//img_00->save("C:\\Users\\jwarton\\00_Workspace\\01_NTlibs\\NT-Template\\ntechnium\\output\\c3\\01\\test.jpg");
	//matrix = img_00->img_IN;
	//tile(1500,1500);
	//tile(50, 50);

	int div_y = 20;
	int div_x = mapRange(0, dim_X, 0, dim_Y, div_y);

	t_TILE = clock();
	///
	//fun_00(200, 200, 1);
	//fun_00(div_y, div_x, 10);
	fun_00(20, 20, 8);
	///
	t_TILE = clock() - t_TILE;
	print_T(t_TILE, "PROCESS IMAGE");
}

void ntMosaic::save(std::string url) {

	t_SAVE = clock();
	///
	const char * file = url.c_str();
	af::array img_OUT = af::resize(matrix, matrix.dims(0), matrix.dims(1));
	img_OUT = af::flip(img_OUT, 0);
	af::saveImage(file, matrix);
	///
	t_SAVE = clock() - t_SAVE;
	print_T(t_SAVE, "SAVE IMAGE   ");
}
void ntMosaic::run() {}

void ntMosaic::tile(int cols, int rows) {

	af::array cell;		/// matrix cropped region of image matrix
	af::array tile;		/// matrix containing current image from tile library

	int tile_W = (floor(dim_X / cols));
	int tile_H = (floor(dim_Y / rows));

	int count = 0;

	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {

				/// DEFINE MATRIX FOR CURRENT CELL
				cell = matrix(af::seq(tile_H * j, tile_H * j + tile_H), af::seq(tile_W * i, tile_W * i + tile_W), af::span);
				int avg = af::mean<int>(cell);

				/// DEFINE TILE PATH FOR COORESPONDING CELL GRAYSCALE VALUE
				stringstream ss;
				ss << std::setw(3) << std::setfill('0');
				ss << avg;
				string mean = ss.str();

				string url = path_Tiles + mean + ".jpg";
				const char * file = url.c_str();

				/// CONFIRM THAT TILE EXISTS FOR CELL VALUE
				if (isFile(url)) {
					std::cout << count << " | "<< avg << "\n" << endl;

					tile = af::loadImage(file, false);
					tile = af::resize(tile, tile_H, tile_W);

					//af::join()

					//af::array conditions(5,5);
					//af::replace(matrix, conditions, tile);

					//AFAPI af_err af_assign_gen(af_array * 	out,
					//	const af_array 	lhs,
					//	const dim_t 	ndims,
					//	const af_index_t * 	indices,
					//	const af_array 	rhs
					//	)

					for (int k = 0; k < tile_W; k++) {
						for (int l = 0; l < tile_H; l++) {

							int t_k = (tile_W * i) + k;
							int t_l = (tile_H * j) + l;

							if (t_k < dim_X && t_l < dim_Y) {
								matrix(t_l, t_k) = tile(l, k);
							}
						}
					}
					count += 1;
				}
				else {
					std::cout << avg << " VALUE DOES NOT EXIST \n" << endl;
				}
		}
	}
}
void ntMosaic::fun_00(int cols, int rows, int gen){

	af::array cell;		/// matrix cropped region of image matrix

	int tile_W = (floor(dim_X / cols));
	int tile_H = (floor(dim_Y / rows));

	cell = build_01(cols, rows, matrix, gen);
	matrix = cell;
}
af::array  ntMosaic::build_01(int cols, int rows, af::array mat_00, int gen) {
	/// FUNCTION MANAGED ASSEMBLY OF RECURSIVE SUBDIVISIONS
	//::array mat_00;	/// matrix cropped region of image matrix
	af::array mat_IN;	/// matrix containing current image from tile library
	af::array mat_XX;	/// matrix of previously joined columns
	af::array mat_c0;	/// matrix of previous cells in current column
	af::array mat_r0;	/// matrix containing tile | join to mat_0

	if (gen > 0) {
		gen -= 1;

		int dim_y = floor(mat_00.dims(0) / cols);
		int dim_x = floor(mat_00.dims(1) / rows);

		for (int i = 0; i < cols; i++) {
			for (int j = 0; j < rows; j++) {

				int ind_x0 = dim_x * i;
				int ind_x1 = dim_x * i + dim_x -1;
				int ind_y0 = dim_y * j;
				int ind_y1 = dim_y * j + dim_y -1;
				///
				mat_IN = mat_00(af::seq(ind_y0, ind_y1), af::seq(ind_x0, ind_x1), af::span);
				int avg = af::mean<int>(mat_IN);

				/// DEFINE TILE PATH FOR COORESPONDING CELL GRAYSCALE VALUE
				stringstream ss;
				ss << std::setw(3) << std::setfill('0');
				ss << avg;
				string mean = ss.str();

				string url = path_Tiles + mean + ".jpg";
				const char * file = url.c_str();
				/// GENERATE RANDOM SEED FOR 
				float fx_i = mapRange(0.01, 0.25, 0, cols, i);
				float fx_j = mapRange(0.01, 0.25, 0, rows, j);
				float seed = ((rand() % 1) + ((rand() % 10) * fx_i));

				seed = mapRange(0, 1, 0, 0.25, seed);

				if ((gen > 1) && (seed < 3.5)) {
					mat_r0 = build_02( seed, mat_IN, gen);
					mat_r0 = af::resize(mat_r0, dim_y, dim_x);
				/// CONFIRM THAT TILE EXISTS FOR CELL VALUE
				} else if (isFile(url)) {
					mat_r0 = af::loadImage(file, false);
					mat_r0 = af::resize(mat_r0, dim_y, dim_x);
					mat_r0 = af::flip(mat_r0, 0);
				} else {
					///std::cout << "//////////////   ERROR:  NO TILE FIND WITH MEAN VALUE: " << avg << endl;
					mat_r0 = af::array(dim_y, dim_x);
					//mat_r0 *= 0;
					//mat_r0 += avg;
					mat_r0 = mat_IN;
				}

				/// JOIN NEXT ROW TO CURRENT COLUMN
				if (j == 0) {
					mat_c0 = mat_r0;
				} else {
					mat_c0 = af::join(0, mat_c0, mat_r0);
				}
			}
			/// JOIN NEXT COLUMN TO MATRIX
			if (i == 0) {
				mat_XX = mat_c0;
			} else{
				//std::cout << " mat: " << i << " | " << mat_XX.dims(0) << " x "<< mat_XX.dims(1) << endl;
				//std::cout << " col: " << i << " | " << mat_c0.dims(0) << " x " << mat_c0.dims(1) << "\n" << endl;

				if (mat_c0.dims(0) == mat_XX.dims(0)) {
					mat_XX = af::join(1, mat_XX, mat_c0);
				} else {
					std::cout << "//////////////   ERROR:  COLUMN EXCEEDS MATRIX DIM(0)" << endl;
				}
			}
		}
	}
	return mat_XX;
}
af::array  ntMosaic::build_02(float seed, af::array mat_00, int gen) {
	/// FUNCTION MANAGED ASSEMBLY OF RECURSIVE SUBDIVISIONS
	//::array mat_00;	/// matrix cropped region of image matrix
	af::array mat_IN;	/// matrix containing current image from tile library
	af::array mat_XX;	/// matrix of previously joined columns
	af::array mat_c0;	/// matrix of previous cells in current column
	af::array mat_r0;	/// matrix containing tile | join to mat_0

	int cols = 2;
	int rows = 2;

	gen -= round(rand() % 10) * 0.01;

	if (gen > 0) {
		gen -= 1;

		int dim_y = floor(mat_00.dims(0) / cols);
		int dim_x = floor(mat_00.dims(1) / rows);

		for (int i = 0; i < cols; i++) {
			for (int j = 0; j < rows; j++) {

				int ind_x0 = dim_x * i;
				int ind_x1 = dim_x * i + dim_x - 1;
				int ind_y0 = dim_y * j;
				int ind_y1 = dim_y * j + dim_y - 1;
				///
				mat_IN = mat_00(af::seq(ind_y0, ind_y1), af::seq(ind_x0, ind_x1), af::span);
				int avg = af::mean<int>(mat_IN);

				/// DEFINE TILE PATH FOR COORESPONDING CELL GRAYSCALE VALUE
				stringstream ss;
				ss << std::setw(3) << std::setfill('0');
				ss << avg;
				string mean = ss.str();

				string url = path_Tiles + mean + ".jpg";
				const char * file = url.c_str();

				/// GENERATE RANDOM SEED FOR 
				seed += 0.1;

				if ((gen > 1) && (seed > 0.25)) {
					mat_r0 = build_02(seed, mat_IN, gen);
					mat_r0 = af::resize(mat_r0, dim_y, dim_x);
					/// CONFIRM THAT TILE EXISTS FOR CELL VALUE
				}
				else if (isFile(url)) {
					mat_r0 = af::loadImage(file, false);
					mat_r0 = af::resize(mat_r0, dim_y, dim_x);
					mat_r0 = af::flip(mat_r0, 0);

				}
				else {
					///std::cout << "//////////////   ERROR:  NO TILE FIND WITH MEAN VALUE: " << avg << endl;
					mat_r0 = af::array(dim_y, dim_x);
					//mat_r0 *= 0;
					//mat_r0 += avg;
					mat_r0 = mat_IN;
				}

				/// JOIN NEXT ROW TO CURRENT COLUMN
				if (j == 0) {
					mat_c0 = mat_r0;
				}
				else {
					mat_c0 = af::join(0, mat_c0, mat_r0);
				}
			}
			/// JOIN NEXT COLUMN TO MATRIX
			if (i == 0) {
				mat_XX = mat_c0;
			}
			else {
				//std::cout << " mat: " << i << " | " << mat_XX.dims(0) << " x "<< mat_XX.dims(1) << endl;
				//std::cout << " col: " << i << " | " << mat_c0.dims(0) << " x " << mat_c0.dims(1) << "\n" << endl;

				if (mat_c0.dims(0) == mat_XX.dims(0)) {
					mat_XX = af::join(1, mat_XX, mat_c0);
				}
				else {
					std::cout << "//////////////   ERROR:  COLUMN EXCEEDS MATRIX DIM(0)" << endl;
				}
			}
		}
	}
	return mat_XX;
}

/// UTILITY FUNCTIONS
void ntMosaic::print_T(clock_t time, std::string message) {

	float sec = ((float)time) / CLOCKS_PER_SEC;

	stringstream ss;
	ss << std::setw(8) << std::setfill('_');
	ss << to_string(sec);
	string t = ss.str();

	std::cout << "\n	////	" << message << ":  " << t << " SECONDS ELAPSED" << endl;
}
af::array ntMosaic::get_Matrix() {
	return matrix;
}