#include "nt_08_afireApp-v02.h"

void afApp02::init() {

	pathIn = nt_Utility::getPathToResources();
	pathOut = nt_Utility::getPathToOutput();
	pathExtension = "imgs\\data\\";
	fileName = "c_001";
	fileExt = ".jpg";
	url = pathIn + pathExtension + fileName + fileExt;

	const char * file = url.c_str();
	img_00 = af::loadImage(file, false);

	img_Sum = img_00 * 0;
	img_Avg = img_00 * 0;
	img_Var = img_00 * 0;

	img_X = img_00.dims(0);
	img_Y = img_00.dims(1);

	rangeBeg = 1;
	rangeEnd = 100;
	fileName = "c_";

	doHistogram = true;

	//std::cout << "\n\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\" << endl;
	//std::cout << "Image Processing Data:" << endl;

	imgAvg(fileName, rangeBeg, rangeEnd);
	imgVar(fileName, rangeBeg, rangeEnd);

	rangeBeg = 1;
	rangeEnd = 2;
	imgAvg(fileName, rangeBeg, rangeEnd); 
	imgVar(fileName, rangeBeg, rangeEnd);

	//std::cout << "\n\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\" << endl;
}

void afApp02::imgAvg(std::string fileName, int rangeBeg, int rangeEnd){

	img_Avg = img_00 * 0;
	img_Sum = img_00 * 0;
	range = (rangeEnd - rangeBeg) + 1;
	pathExtension = "imgs\\data\\";

	////////////////////////// CONSTRUCT IMAGEWISE SUMMATION MATRIX
	///////////////////////////////////////////////////////////////
	for (int i = rangeBeg; i <= rangeEnd; i++){
		stringstream ss;
		ss << setw(3) << setfill('0') << i;
		std::string fileIndex = ss.str();
		url = pathIn + pathExtension + fileName + fileIndex + fileExt;
		const char * file = url.c_str();
		af::array img_X = af::loadImage(file, false);
		img_Sum += img_X;
	}
	////////////////////////////////////  DIVIDE SUMMATION BY RANGE
	///////////////////////////////////////////////////////////////
	img_Avg = img_Sum / range;

	//////////////////////////////// SAVE IMAGE TO OUTPUT DIRECTORY
	///////////////////////////////////////////////////////////////
	pathExtension = "img\\";
	stringstream rB; 
	rB << setw(3) << setfill('0') << rangeBeg;
	stringstream rE;
	rE << setw(3) << setfill('0') << rangeEnd;
	url = pathOut + pathExtension + fileName + "Avg_" + rB.str() + "-" + rE.str() + fileExt;

	const char * fc_01 = url.c_str();
	af::saveImage(fc_01, img_Avg);

	/////////////////////////////////////////// CONSTRUCT HISTOGRAM
	///////////////////////////////////////////////////////////////
	if (doHistogram == true){
		af::array his_Avg = histogram(img_Avg, 255);
		string file = fileName + "Avg_Histogram_" + rB.str() + "-" + rE.str();
		imgHis(file, his_Avg);
	}
}
void afApp02::imgVar(std::string fileName, int rangeBeg, int rangeEnd){
	
	img_Var = img_00 * 0;
	range = (rangeEnd - rangeBeg) + 1;
	pathExtension = "imgs\\data\\";

	////////////////////////////////// CALCULATE PIXELWISE VARIANCE
	///////////////////////////////////////////////////////////////

	for (int i = rangeBeg; i <= rangeEnd; i++){
		stringstream ss;
		ss << setw(3) << setfill('0') << i;
		std::string fileIndex = ss.str();
		url = pathIn + pathExtension + fileName + fileIndex + fileExt;
		const char * file = url.c_str();
		af::array img_X = af::loadImage(file, false);
		img_Var += pow(img_X - img_Avg, 2);
	}
	img_Var /= range;

	//////////////////////////////// SAVE IMAGE TO OUTPUT DIRECTORY
	///////////////////////////////////////////////////////////////
	pathExtension = "img\\";
	stringstream rB;
	rB << setw(3) << setfill('0') << rangeBeg;
	stringstream rE;
	rE << setw(3) << setfill('0') << rangeEnd;
	url = pathOut + pathExtension + fileName + "Var_" + rB.str() + "-" + rE.str() + fileExt;

	const char * fc_01 = url.c_str();
	af::saveImage(fc_01, img_Var);

	/////////////////////////////////////////// CONSTRUCT HISTOGRAM
	///////////////////////////////////////////////////////////////
	if (doHistogram == true){
		af::array his_Var = histogram(img_Var, 255);
		string file = fileName + "Var_Histogram_" + rB.str() + "-" + rE.str();
		imgHis(file, his_Var);
	}

	af::array varMin = af::min(img_Var);
	float valMin = af::min<float>(varMin);
	af::array varMax = af::max(img_Var);
	float valMax = af::max<float>(varMax);
	af::array varAvg = af::mean(img_Var);
	float valAvg = af::mean<float>(varAvg);

	/////////////////////////////////// PRINT PROPERTIES TO CONSOLE
	///////////////////////////////////////////////////////////////
	//std::cout << "\n" << endl;
	//std::cout << fileName + "Var_" + rB.str() + "-" + rE.str() + fileExt << endl;
	//std::cout << "IMAGE SIZE:     X:" << img_X << "    Y: " << img_Y << endl;
	//std::cout << "IMAGES IN SET:  " << range << "\n" << endl;
	//std::cout << "VARIANCE MIN:   " << valMin << endl;
	//std::cout << "VARIANCE MAX:   " << valMax << endl;
	//std::cout << "VARIANCE MEAN:  " << valAvg << endl;

	///////////////////////////////// WRITE PROPERTIES TO TEXT FILE
	///////////////////////////////////////////////////////////////
	std::string	url = pathOut + pathExtension + fileName + "Var_" + rB.str() + "-" + rE.str() + ".txt";
	std::ofstream file(url);

	stringstream ss1;
	ss1 << setw(10) << setfill(' ') << valMin;
	stringstream ss2;
	ss2 << setw(10) << setfill(' ') << valMax;
	stringstream ss3;
	ss3 << setw(10) << setfill(' ') << valAvg;

	file << "//////////////////////////////////////////////////////////////\n";
	file << "/////////////////////////////////////////////// IMAGE ANALYSIS\n\n";
	file << "IMAGES SET NAME:  " << fileName << rB.str() << "-" << rE.str() << fileExt << "\n" << endl;
	file << "IMAGES IN SET:    " << range << endl;

	file << "IMAGE SIZE: X:    " << img_X << endl;
	file << "            Y:    " << img_Y << "\n" << endl;

	file << "VARIANCE MIN:     " << ss1.str() << "\n";
	file << "VARIANCE MAX:     " << ss2.str() << "\n";
	file << "VARIANCE MEAN:    " << ss3.str() << "\n";
}
void afApp02::imgHis(std::string file, af::array histogram){

	//////////////////////////////// SAVE IMAGE TO OUTPUT DIRECTORY
	///////////////////////////////////////////////////////////////
	pathExtension = "img\\";
	url = pathOut + pathExtension + file + ".jpg";

	float histMin = af::min<float>(histogram);
	float histMax = af::max<float>(histogram);

	//std::cout << "histMin:  " << histMin << endl;
	//std::cout << "histMax:  " << histMax << endl;
	//af_print(histogram.elements());
	//printf("ISU32: %d\n", histogram.type() == u32);
	histogram = histogram.as(f32);
	//printf("ISF32: %d\n", histogram.type() == f32);
	//af_print(histogram.elements());

	int K = 2;											///best graph quality if multiple of 255
	dim_t dim1 = 255 * K;								//graph width
	dim_t dim0 = 255;									//graph height
	
	//dim_type dim1 = 255 * K;							//data type discontinued with v3.0 of AF					
	//dim_type dim0 = 255;								

	af::array img_Hist(dim0, dim1, f32);
	img_Hist = 1.0;

	std::array <float, 255> outData;
	histogram.host((void*)&outData);

	for (int i = 0; i < outData.size(); i++){			//0-255 histogram values
		float val = outData[i];							//value stored in histogram at 
		val = mapRange(0, dim0, 0, histMax, val);		//value mapped to graph height
		for (int k = 0; k < K; k++){
			for (int j = 0; j < dim0; j++){				//0-graph height
				if (j < val){							//map val to pixel position  | 0 - dim0
					img_Hist(j, (K * i) + k) = 0;
				}
			}
		}
	}

	img_Hist = flip(img_Hist, 0);

	const char * fc_01 = url.c_str();
	af::saveImage(fc_01, img_Hist);
}
void afApp02::run(){

}
void afApp02::display(){

}