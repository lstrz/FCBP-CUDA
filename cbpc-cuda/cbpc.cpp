#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <ctime>

#include "PGMCBPCCUDA.h"
#include "../staticPredictors-cuda/PredictorNCUDA.h"
#include "../staticPredictors-cuda/PredictorNWCUDA.h"
#include "../staticPredictors-cuda/PredictorGWCUDA.h"
#include "../staticPredictors-cuda/PredictorWCUDA.h"
#include "../staticPredictors-cuda/PredictorNECUDA.h"
#include "../staticPredictors-cuda/PredictorGNCUDA.h"
#include "../staticPredictors-cuda/PredictorPLCUDA.h"

string usage = " inputImage.pgm...";

void fail(string msg){
	cerr<<msg<<endl;
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
	if(argc < 2){
		usage = string("Usage:\n") + argv[0] + usage;
		fail(usage);
	}

	for(unsigned fileOffset = 0; fileOffset < argc; fileOffset += FILESET_SIZE){
		vector<PGMImage> inputImages;
		vector<PGMImage> outputImages;
		vector<PGMImageError> errorImages;
		for(int i = 0; i < FILESET_SIZE && i < argc; ++i){
			if(i + fileOffset == 0){
				++i;
			}
			string inputName = argv[i + fileOffset];
			size_t dot = inputName.find_last_of('.');
			if(dot == inputName.npos){
				dot = inputName.length();
			}
			string outputName = inputName.substr(0, dot) + "_prediction" + inputName.substr(dot);
			string errorName = inputName.substr(0, dot) + "_error" + inputName.substr(dot);

			inputImages.emplace_back(inputName.c_str());
			unsigned w = inputImages[i-1].getWidth();
			unsigned h = inputImages[i-1].getHeight();
			unsigned size = inputImages[i-1].getSize();
			unsigned maxPixel = inputImages[i-1].getPixelMax();
			outputImages.emplace_back(outputName.c_str(), w, h, maxPixel);
			errorImages.emplace_back(errorName.c_str(), w, h, maxPixel);
		}

		vector<Predictor*> predictors;
		predictors.push_back(new PredictorN);
		predictors.push_back(new PredictorNW);
		predictors.push_back(new PredictorGW);
		predictors.push_back(new PredictorW);
		predictors.push_back(new PredictorNE);
		predictors.push_back(new PredictorGN);
		predictors.push_back(new PredictorPL);

		PGMCBPCCUDA cbpc(inputImages, outputImages, errorImages, predictors);
		cbpc.predict();
	}

	return 0;
}
