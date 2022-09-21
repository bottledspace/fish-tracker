#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <llsvm.h>

const char *title = "fishcls";

auto get_solved_model(const std::string &filename, int size) {
	std::ifstream is(filename);
	std::string line;

	double rho = 0.0f;
	int n = 0;
	while (getline(is, line)) {
		std::stringstream ls(line);
		std::string cmd;
		ls >> cmd;
		
		if (cmd == "total_sv")
			ls >> n;
		else if (cmd == "rho")
			ls >> rho;
		else if (cmd == "SV")
			break;
	}

	std::vector<double> alphas(n);
	cv::Mat_<double> M(n, size);
	for (int i = 0; i < n; i++) {
		getline(is, line);
		
		std::stringstream ls(line);
		int j; char sep;
	
		ls >> alphas[i];
	    while (ls >> j >> sep >> M.at<double>(i,j))
			;
	}
	
	std::vector<double> res(size+1, 0);
	for (int i = 0; i < n; i++)
	for (int j = 0; j < size; j++)
		res[j] += alphas[i] * M.at<double>(i,j);
	res.back() = -rho;
	return res;
}

void test_svm(cv::HOGDescriptor &hog, cv::VideoCapture &cap, libsvm::svm &classify) {
	std::vector<cv::Rect> rois;
	std::vector<double> weights;
	std::vector<float> desc;
	cv::Mat image, gray;
	
	cv::namedWindow(title, cv::WINDOW_NORMAL);	
	for (;;) {
		cap >> image;
		
		hog.detectMultiScale(image, rois, weights);
		for (int i = 0; i < weights.size(); i++) {
			float certainty = 200*weights[i]*weights[i];
			rectangle(image, rois[i], cv::Scalar(certainty,0,0));
			
			cv::cvtColor(image(rois[i]), gray, cv::COLOR_BGR2GRAY);
			cv::resize(gray, gray, hog.winSize);
			hog.compute(gray, desc, cv::Size(8,8), cv::Size(0,0));
			
			std::vector<std::pair<int, double>> V;
			for (int i = 0; i < desc.size(); i++)
				V.push_back({i, desc[i]});
			int guess = classify.predict(V);
			const char *names[] = {
				"Dascyllus reticulatus",
				"Plectroglyphidodon dickii ",
				"Chromis chrysura",
				"Amphiprion clarkii",
				"Chaetodon lunulatus",
				"Chaetodon trifascialis",
				"Myripristis kuntee",
				"Acanthurus nigrofuscus",
				"Hemigymnus fasciatus",
				"Neoniphon sammara",
				"Abudefduf vaigiensis",
				"Canthigaster valentini",
				"Pomacentrus moluccensis",
				"Zebrasoma scopas",
				"Hemigymnus melapterus",
				"Lutjanus fulvus",
				"Scolopsis bilineata",
				"Scaridae",
				"Pempheris vanicolensis",
				"Zanclus cornutus",
				"Neoglyphidodon nigroris",
				"Balistapus undulatus",
				"Siganus fuscescens",
			};
			cv::putText(image, names[guess-1], 0.5*(rois[i].br()+rois[i].tl()), 
				cv::FONT_HERSHEY_SIMPLEX, 0.3, cv::Scalar(255,0,0));
		}
		
		cv::imshow(title, image);
		cv::waitKey(1);
	}
}

auto str2size(const std::string &str) {
	std::stringstream is(str);
	int x, y; char sep;
	is >> x >> sep >> y;
	return cv::Size(x, y);
}

int main(int argc, char **argv) {
	if (argc != 5) {
		std::cerr << "Usage: fishcls WxH <detector> <classifier> <video>" << std::endl;
		return EXIT_FAILURE;
	}
	cv::HOGDescriptor hog;
	hog.winSize = str2size(argv[1]);
	hog.setSVMDetector(get_solved_model(argv[2], hog.getDescriptorSize()));
	
	libsvm::svm classify{argv[3]};
	
	cv::VideoCapture cap;
	cap.open(argv[4]);
	test_svm(hog, cap, classify);
}