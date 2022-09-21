#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

std::random_device rd;

auto str2pair(const std::string &str) {
	std::stringstream is(str);
	int x, y; char sep;
	is >> x >> sep >> y;
	return std::make_tuple(x, y);
}

int main(int argc, char **argv) {
	if (argc != 5) {
		std::cerr
			<<  "Usage: mkhogs WxH MODE <category> <directory>\n"
				"MODE\n"
				" r - Randomized window in images\n"
				" s - Resize images\n"
			<< std::endl;
		return EXIT_FAILURE;
	}
	auto [w, h] = str2pair(argv[1]);
	int mode = argv[2][0];
	int category = atoi(argv[3]);
	std::vector<cv::String> listing;
	cv::glob(argv[4], listing);
	
	cv::HOGDescriptor hog;
	hog.winSize = cv::Size(w,h);
	
	std::vector<float> desc;
	for (const auto &filename : listing) {
		cv::Mat image = cv::imread(filename);
		if (image.empty())
			continue; // Ignore empty files
		
		if (mode == 's')
			cv::resize(image, image, cv::Size(w,h));
		
		cv::Mat gray;
		cv::Rect roi = cv::Rect((image.cols-w)/2, (image.rows-h)/2, w, h);
		if (mode == 'r') {
			int x = std::uniform_int_distribution<>(0,image.cols-w)(rd);
			int y = std::uniform_int_distribution<>(0,image.rows-h)(rd);
			roi = cv::Rect(x, y, w, h);
		}
		cv::cvtColor(image(roi), gray, cv::COLOR_BGR2GRAY);
		hog.compute(gray, desc, cv::Size(8,8), cv::Size(0,0));
		
		std::cout << category << " ";
		for (int i = 0; i < desc.size(); i++)
			std::cout << i << ":" << desc[i] << " ";
		std::cout << std::endl;
	}
}