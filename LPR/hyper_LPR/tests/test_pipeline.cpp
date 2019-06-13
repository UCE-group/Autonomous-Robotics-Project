/*
@ : HyperLPR SDK
*/
#include "../include/Pipeline.h"
using namespace pr;

void TEST_PIPELINE() {

	pr::PipelinePR prc("../lpr/model/cascade.xml",
		"../lpr/model/HorizonalFinemapping.prototxt", "../lpr/model/HorizonalFinemapping.caffemodel",
		"../lpr/model/Segmentation.prototxt", "../lpr/model/Segmentation.caffemodel",
		"../lpr/model/CharacterRecognization.prototxt", "../lpr/model/CharacterRecognization.caffemodel"
	);

	cv::Mat image = cv::imread("C:\\Users\\10027\\source\\repos\\Prj-Win\\lpr\\tests\\test.png", 1);

	std::vector<pr::PlateInfo> res = prc.RunPiplineAsImage(image);
	float conf = 0;
	for (auto st : res) {
		if (st.confidence > 0.1) {
			std::cout << st.getPlateName() << " " << st.confidence << std::endl;
			conf += st.confidence;
		}
	}
	std::cout << conf << std::endl;
}
int main()
{


	TEST_PIPELINE();


	return 0;


}