#include "ntImage.h"

using namespace arma;
using namespace std;

ntImage::ntImage(){}
ntImage::ntImage(int width, int height, float col):
	img_X(width), img_Y(height)
{
	img_IN = af::constant(col, height, width);
	img_2D = af::array(img_IN);

	img_W = img_2D.dims(1);
	img_H = img_2D.dims(0);
}
ntImage::ntImage(std::string url, std::string prefix):
	url_IN(url),  prefix(prefix)
{
	init();
}
ntImage::ntImage(std::string url):
	url_IN(url)
{
	init();
}
ntImage::ntImage(ntVec3* pos, std::string url):
pos(pos), url_IN(url){
	init();
}
//ntImage::ntImage(arma::mat image) {
//	img_2d = image;
//
//	dim_X = img_2d.n_cols;
//	dim_Y = img_2d.n_rows;
//
//	img_X = dim_X;
//	img_Y = dim_Y;
//}
void ntImage::init() {	
	pos->x =	0;
	pos->y =	0;
	pos->z =	0;
	load_IMG(url_IN);
}
void ntImage::load_IMG(std::string) {

	const char * file = url_IN.c_str();
	img_IN = af::loadImage(file, false);
	img_2D = af::array(img_IN);

	img_X = img_2D.dims(1);
	img_Y = img_2D.dims(0);

	img_W = img_2D.dims(1);
	img_H = img_2D.dims(0);
}

void ntImage::set_Host(){
	/// UPDATE ARMA VALUES FROM AF MATRIX
	img_2d = zeros<fmat>(img_2D.dims(0), img_2D.dims(1));
	af::array img_LOAD(img_2D.dims(0), img_2D.dims(1), img_2d.memptr());
	img_LOAD += img_2D;

	img_LOAD.host((void*)img_2d.memptr());	///  LOAD ARMA MATRIX WITH WITH IMAGE DATA

	img_W = img_2D.dims(1);
	img_H = img_2D.dims(0);

	isImgLoaded = true;
	isImgLocal = true;
}
void ntImage::set_Local() {
	/// UPDATE AF MATRIX VALUES FROM ARMA
	int dim_x = img_2d.n_cols;
	int dim_y = img_2d.n_rows;
	af::array img_AF = af::array(dim_y, dim_x, img_2d.memptr());
	img_2D = img_AF;
	isImgLocal = true;
}

void ntImage::set_Col(float col) {
	img_2D *= 0;
	img_2D += col;
}
void ntImage::set_Dim(int width, int height, bool isUniform) {
	//img_2D(af::span) = 150;
	img_W = width;
	if (height == 1) {
		height = mapRange(0, width, 0, img_X, img_Y);
	}
	img_H = height;
	if (img_H > 0) {
		dim_t dim_x = width;
		dim_t dim_y = height;
		//double scFx = 1.0 / img_T;
		//img_2D = af::resize(scFx, img_2D);
		img_2D = af::resize(img_2D, dim_y, dim_x);
		img_X = img_2D.dims(1);
		img_Y = img_2D.dims(0);
	}
	else {
		std::cout << "  ERROR:  RESIZE DIMS HAVE NEGATIVE VALUE OR EXCEED BOUNDS" << endl;
	}
	
}
void ntImage::set_Pos(ntVec3* pos) {
	this->pos = pos;
}
void ntImage::set_Pos(int x, int y) {
	pos->x = x;
	pos->y = y;
	pos->z = 0;
}
void ntImage::scale_2D(float scFx) {

}

void ntImage::sort(int dim, bool isAscending) {
	isImgLocal = false;
	img_2D = af::sort(img_2D, dim, isAscending);
}

void ntImage::reset() {
	isImgLocal = false;
	img_2D = img_IN;
}
void ntImage::add() {
	isImgLocal = false;
	img_2D =  af::maxfilt(img_2D, 5, 5);
	img_2D += img_IN;

}
void ntImage::sub() {
	isImgLocal = false;
	img_2D = img_IN - img_2D;
	img_2D = af::abs(img_2D);
}

void ntImage::contrast() {
	isImgLocal = false;
	//array changeContrast(const array &in, const float contrast)
	//{
	//	float scale = tan((contrast + 1)*Pi / 4);
	//	return (((in / 255.0f - 0.5f) * scale + 0.5f) * 255.0f);
	//}
	//array changeBrightness(const array &in, const float brightness, const float channelMax = 255.0f)
	//{
	//	float factor = brightness*channelMax;
	//	return (in + factor);
	//}
}
void ntImage::blur(int iterations)
{	
	isImgLocal = false;
	int dim = iterations;
	const af::array mask = af::gaussianKernel(dim, dim);
	af::array blurred = af::array(img_2D.dims(), img_2D.type());
	for (int i = 0; i < (int)blurred.dims(2); i++){
		blurred(af::span, af::span, i) = af::convolve(img_2D(af::span, af::span, i), mask);
	}
	img_2D = blurred;
}
void ntImage::edges() {
	isImgLocal = false;
	// 3x3 sobel weights
	static const float h_sobel[] = {
		-2.0, -1.0,  0.0,
		-1.0,  0.0,  1.0,
		 0.0,  1.0,  2.0
	};
	af::array sobel_k = af::array(3, 3, h_sobel);
	img_2D = af::abs(af::convolve(img_2D, sobel_k));
}
void ntImage::crop(int x_0, int x_1, int y_0, int y_1) {
	isImgLocal = false;
	img_2D = img_2D(af::seq(x_0, x_1), af::seq(y_0, y_1), af::span);
}
void ntImage::zoom(int x, int y, int w, int h)
{
	isImgLocal = false;
	img_IN = img_IN(af::seq(x, w - 1), af::seq(y, h - 1), af::span);
	img_IN = resize(img_IN, (unsigned)img_IN.dims(0), (unsigned)img_IN.dims(1));
}
void ntImage::invert() {
	isImgLocal = false;
	img_2D = 255 - img_2D;
}

void ntImage::tile(int cols, int rows, bool doSave) {

	int dim_x = (floor(img_2D.dims(1) / cols));
	int dim_y = (floor(img_2D.dims(0) / rows));
	af::array mat_tile;// = af::array(dim_y, dim_x, img_2D.type);

	int cnt = 0;
	int val;

	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {


			int ind_x0 = dim_x * i;
			int ind_x1 = dim_x * i + dim_x - 1;
			int ind_y0 = dim_y * j;
			int ind_y1 = dim_y * j + dim_y - 1;

			mat_tile = img_2D(af::seq(ind_y0, ind_y1), af::seq(ind_x0, ind_x1), af::span);

			float u = 50 / cols;
			float v = 50 / rows;

			int mean = af::mean<int>(mat_tile);
			cnt += 1;

			if (doSave == true) {
				stringstream ss;
				ss << std::setw(2) << std::setfill('0');
				ss << cnt;
				string count = ss.str();

				stringstream val;
				val << std::setw(3) << std::setfill('0');
				val << floor(mean);
				string avg = val.str();

				std::string tileName = "TEST_" + count + "_" + prefix + "-" + avg  +  ".jpg";
				save(path_OUT + "tiles\\", tileName, mat_tile);
			}
		}
	}
}
void ntImage::tile_CPU(int cols, int rows) {

	clock_t t = clock();
	set_Host();
	arma::fmat tile;
	int avg = 0;

	int tile_W = (floor(img_W / cols));
	int tile_H = (floor(img_H / rows));

	///
	af::array tile_SRC;	/// matrix containing current image from tile library
	int dim_y = floor(img_2D.dims(0) / rows);
	int dim_x = floor(img_2D.dims(1) / cols);
	///

	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			///
			int ind_x0 = dim_x * i;
			int ind_x1 = dim_x * i + dim_x - 1;
			int ind_y0 = dim_y * j;
			int ind_y1 = dim_y * j + dim_y - 1;
			tile_SRC = img_2D(af::seq(ind_y0, ind_y1), af::seq(ind_x0, ind_x1), af::span);
			avg = af::mean<int>(tile_SRC);
			///
			tile = zeros<fmat>(tile_H, tile_W);
			///PIXEL WISE DISTRIBUTION OF VALUES TO TILE
			for (int k = 0; k < tile_W; k++) {
				for (int l = 0; l < tile_H; l++) {

					int t_k = (tile_W * i) + k;
					int t_l = (tile_H * j) + l;

					if (t_k < img_X && t_l < img_Y) {
						if (k == 0 || l == 0 || k == tile_W-1 || l == tile_H-1) {
							img_2d(t_l, t_k) = 0;
						}
						else {
							img_2d(t_l, t_k) = avg;
						}
						tile(l, k) = img_2d(t_l, t_k);
					}
				}
			}
		}
	}
	set_Local();

	t = clock() - t;
	string time = format_SEC(t);
	std::cout << "\nPIXEL WISE PROCESSING:  " << time << "\n"  << endl;
}
void ntImage::tile_GPU_02(int cols, int rows, int sd_t) {

	clock_t t = clock();

	int avg = 0;
	int gen = 5;
	int tile_W = (floor(img_W / cols));
	int tile_H = (floor(img_H / rows));

	///
	af::array mat_XX;	/// matrix of previously joined columns
	af::array mat_c0;	/// matrix of previous cells in current column
	af::array mat_r0;	/// matrix containing tile | join to mat_0
	af::array tile_SRC;	/// matrix containing current image from tile library
	int dim_y = floor(img_2D.dims(0) / rows);
	int dim_x = floor(img_2D.dims(1) / cols);
	///

	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			///
			int ind_x0 = dim_x * i;
			int ind_x1 = dim_x * i + dim_x - 1;
			int ind_y0 = dim_y * j;
			int ind_y1 = dim_y * j + dim_y - 1;
			tile_SRC = img_2D(af::seq(ind_y0, ind_y1), af::seq(ind_x0, ind_x1), af::span);

			///std::cout << "SRC:  " << tile_SRC.dims(0) << " | " << tile_SRC.dims(1) << endl;

			///
			mat_r0 = tile_SD(tile_SRC, sd_t, gen);

			/// JOIN NEXT ROW TO CURRENT COLUMN
			if (j == 0) {
				mat_c0 = mat_r0;
				/// std::cout << i << " | " << j << endl;
			}
			else {
				///std::cout << "col: "  << i << " | row: " << j << endl;
				mat_c0 = af::join(0, mat_c0, mat_r0);
			}
		}
		/// JOIN NEXT COLUMN TO MATRIX
		if (i == 0) {
			mat_XX = mat_c0;
		}
		else {
			if (mat_c0.dims(0) == mat_XX.dims(0)) {
				mat_XX = af::join(1, mat_XX, mat_c0);
			} else if(mat_c0.dims(0) > mat_XX.dims(0)) {
				std::cout << "//////////////   ERROR:  COLUMN EXCEEDS MATRIX DIM(0): " << mat_c0.dims(0) << " : " << mat_XX.dims(0) << endl;
			}
			else if (mat_c0.dims(0) < mat_XX.dims(0)) {
				std::cout << "//////////////   ERROR:  MATRIX DIM(0) EXCEEDS COLUMN: " << mat_XX.dims(0) << " : " << mat_c0.dims(0) << endl;
			}
		}
	}
	img_2D = mat_XX;

	t = clock() - t;
	string time = format_SEC(t);
	std::cout << "\nPIXEL WISE PROCESSING:  " << time << "\n" << endl;
}

af::array ntImage::tile_SD(af::array matrix, int sd_t, int gen_Max) {
	arma::fmat tile;
	int avg = af::mean<int>(matrix);

	int dim_y = matrix.dims(0);
	int dim_x = matrix.dims(1);
	int dy = floor(dim_y * 0.5);
	int dx = floor(dim_x * 0.5);

	int avg_min = 255;
	int avg_max = 0;


	if (dim_x >= 10) {
		af::array tile_0_0 = matrix(af::seq(0, dy - 1), af::seq(0, dx - 1), af::span);
		af::array tile_0_1 = matrix(af::seq(0, dy - 1), af::seq(dx, dim_x - 1), af::span);
		af::array tile_1_0 = matrix(af::seq(dy, dim_y - 1), af::seq(0, dx - 1), af::span);
		af::array tile_1_1 = matrix(af::seq(dy, dim_y - 1), af::seq(dx, dim_x - 1), af::span);

		int avg_0_0 = af::mean<int>(tile_0_0);
		int avg_0_1 = af::mean<int>(tile_0_1);
		int avg_1_0 = af::mean<int>(tile_1_0);
		int avg_1_1 = af::mean<int>(tile_1_1);

		int avgs[4] = { af::mean<int>(tile_0_0), af::mean<int>(tile_0_1), af::mean<int>(tile_1_0), af::mean<int>(tile_1_1) };

		for (int i = 0; i < 4; i++) {
			if (avgs[i] < avg_min) { avg_min = avgs[i]; }
			if (avgs[i] > avg_max) { avg_max = avgs[i]; }
		}

		int dev = avg_max - avg_min;

		bool cond_00 = (avg_0_0 - avg < sd_t && avg_0_0 - avg > -sd_t);
		bool cond_01 = (avg_0_1 - avg < sd_t && avg_0_1 - avg > -sd_t);
		bool cond_02 = (avg_1_0 - avg < sd_t && avg_1_0 - avg > -sd_t);
		bool cond_03 = (avg_1_1 - avg < sd_t && avg_1_1 - avg > -sd_t);
		bool cond_04 = (dev > sd_t);

		tile = zeros<fmat>(dim_y, dim_x);

		matrix *= 0;
		gen_Max = gen_Max - 1;
		//cond_00 && cond_01 && cond_02 && cond_03 && 
		if (cond_04 && gen_Max > 0) {
			tile_0_0 = tile_SD(tile_0_0, sd_t, gen_Max);
			tile_0_1 = tile_SD(tile_0_1, sd_t, gen_Max);
			tile_1_0 = tile_SD(tile_1_0, sd_t, gen_Max);
			tile_1_1 = tile_SD(tile_1_1, sd_t, gen_Max);

			af::array c0 = af::join(0, tile_0_0, tile_1_0);
			af::array c1 = af::join(0, tile_0_1, tile_1_1);
			matrix += af::join(1, c0, c1);
			/// std::cout <<"mat:  " << matrix.dims(0) << " | " << matrix.dims(1) << " gen: " << gen_Max << endl;
		}
		else {
			matrix += avg;
		}
	}
	else {
		matrix *= 0;
		matrix += avg;
	}

	matrix.row(0) *= 0;
	matrix.col(0) *= 0;

	return matrix;
}
void ntImage::tile_GPU(int cols, int rows) {
	clock_t t = clock();

	int avg = 0;
	int tile_W = (floor(img_W / cols));
	int tile_H = (floor(img_H / rows));

	///
	af::array mat_XX;	/// matrix of previously joined columns
	af::array mat_c0;	/// matrix of previous cells in current column
	af::array mat_r0;	/// matrix containing tile | join to mat_0
	af::array tile_SRC;	/// matrix containing current image from tile library
	int dim_y = floor(img_2D.dims(0) / rows);
	int dim_x = floor(img_2D.dims(1) / cols);
	///

	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			///
			int ind_x0 = dim_x * i;
			int ind_x1 = dim_x * i + dim_x - 1;
			int ind_y0 = dim_y * j;
			int ind_y1 = dim_y * j + dim_y - 1;
			tile_SRC = img_2D(af::seq(ind_y0, ind_y1), af::seq(ind_x0, ind_x1), af::span);
			avg = af::mean<float>(tile_SRC);
			///
			mat_r0 = af::constant(avg, tile_H, tile_W, f32);

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
			if (mat_c0.dims(0) == mat_XX.dims(0)) {
				mat_XX = af::join(1, mat_XX, mat_c0);
			}
			else {
				std::cout << "//////////////   ERROR:  COLUMN EXCEEDS MATRIX DIM(0)" << endl;
			}
		}
	}
	img_2D = mat_XX;
	t = clock() - t;
	string time = format_SEC(t);
	std::cout << "\nMATRIX FUNCTION PROCESSING:  " << time << "\n" << endl;
}

void ntImage::tile(int cols, int rows, std::string path) {
	path_OUT = path;
	tile(cols, rows, true);
}

void ntImage::save(string path, string fileName, af::array matrix) {
	this->fileName = fileName;
	path = path + fileName;
	const char * file = path.c_str();

	af::array img_OUT = af::array(matrix);
	af::saveImage(file, img_OUT);
}

void ntImage::save(string path, string fileName, arma::fmat matrix) {
	this->fileName = fileName;
	path = path + fileName;
	const char * file = path.c_str();

	int dim_x = matrix.n_cols;
	int dim_y = matrix.n_rows;

	af::array img_OUT = af::array(dim_y,dim_x);
	img_OUT *= 0;

	for (int i = 0; i < dim_y; i++) {
		for (int j = 0; j < dim_x; j++) {

			img_OUT(i, j) = matrix(i, j);

			if (matrix(i, j) > 255) {
				img_OUT(i, j) = 255;
			}
			if (matrix(i, j) < 0) {
				img_OUT(i, j) = 0;
			}
		}
	}

	img_OUT = af::flip(img_OUT, 0);
	af::saveImage(file, img_OUT);
}
void ntImage::save(string path, string fileName) {
	save(path, fileName, img_2D);
}
void ntImage::save(string url) {
	save(url, "", img_2D);
}

int ntImage::dims(int dim) {
	int val = NULL;
	if (dim == 0) {
			val = img_Y;
	} else if (val == 1){
			val = img_X;
	}
	else {
		std::cout << " ERROR: DIM PROVIDED IS NOT A VALID VALUE" << endl;
	}
	return val;
}
double ntImage::mean() {
	double min = af::mean<double>(img_2D);
	return min;
}
double ntImage::min_val() {
	double avg = af::min<double>(img_2D);
	return avg;
}
double ntImage::max_val() {
	double max = af::max<double>(img_2D);
	return max;
}

int ntImage::getPixel(double u, double v) {
	if (isImgLocal == false) {
		set_Host();
	}
	
	int x;
	int y;
	int val;

	if (u < 0 || u > 1 || v < 0 || v > 1) {
		std::cout << "// ERROR:  UV PARAMETERS MUST BE WITHIN RANGE 0-1";
	} else {
		x = round(mapRange(0, img_X, 0, 1, u));
		y = round(mapRange(0, img_Y, 0, 1, v));

		val = img_2d(x, y);
		return val;
	}
}
arma::fmat* ntImage::getMatrix() {
	if (isImgLocal == false) {
		set_Host();
	}
	return &img_2d;
}
void ntImage::writePixels(std::vector<int> values, std::string url) {

	///////////////////////////////////////////////////// SET TIME STAP
	struct tm timeData;
	time_t timeStamp;

	timeStamp = time(0);
	localtime_s(&timeData, &timeStamp);
	string Y = to_string(timeData.tm_year + 1900);
	string M = to_string(timeData.tm_mon + 1);
	string D = to_string(timeData.tm_mday);
	string Hr = to_string(timeData.tm_hour);
	string Mn = to_string(timeData.tm_min);
	string Sc = to_string(timeData.tm_sec);
	string date = "DATE: " + Y + ":" + M + ":" + D + " | " + "TIME: " + Hr + ":" + Mn + ":" + Sc + "\n";


	///////////////////////////////////////////////////// OPEN FILE STREAM
	ofstream file(url);

	file << "//  HKS | LINE\n";
	file << "//  C3 VISUALIZING TOKYO - RESEARCH WORKSHOP\n";
	file << "//  RELEASE " << date << "\n";
	file << "//  [ PIXEL VALUES 0-255 ]\n";
	file << "\n";
	for (int i = 0; i < values.size(); i++) {
		file << "  PIX:  " << to_string(values.at(i)) << "\n";
	}
	file << "\n";
	file << "//  END FILE\n";
	file.close();

	///////////////////////////////////////////////////// SET URL
	std::cout << "\n IMAGE PIXEL DATA SAVED TO TXT FILE \n" << endl;
}

void ntImage::run() {
	if (isImgLocal == false) {
		set_Host();
	}
}
void ntImage::display() {
	if (isImgLocal == false) {
		set_Host();
	}

	int n_cols = img_2d.n_cols;
	int n_rows = img_2d.n_rows;
	int u_cols = img_T;
	int v_rows = img_T;

	float col;
	glPointSize(1);
	glBegin(GL_POINTS);

	for (int u = 0; u < u_cols; u++) {
		for (int v = 0; v < v_rows; v++) {
			for (int i = 0; i < n_cols; i++) {
				for (int j = 0; j < n_rows; j++) {
					col = img_2d( j, i);
					col = mapRange(0, 1, 0, 255, col);

					if (u != 0 || v != 0) {
						col *= 0.75;
					}

					int x = (i + pos->x) + (u * n_cols);
					int y = (pos->y + n_rows - j) + (v*n_rows);

					glColor4f(col, col, col, alpha);
					glVertex3f(x, y, 0);

				}
			}
		}
	}
	glEnd();
}