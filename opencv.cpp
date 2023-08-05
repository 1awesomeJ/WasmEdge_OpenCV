#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>

int main(int argc, char** argv) {

	if (argc != 4) {
        	std::cerr << "Usage: " << argv[0] << " <first_image_path> <second_image_path> <output_file_name>" << std::endl;
			return -1;
        }

	std::string output_file = argv[3];
	output_file += ".png";

	cv::Mat left_face, right_face;

	try {
	    	left_face = cv::imread(argv[1], cv::IMREAD_COLOR);
	    	right_face = cv::imread(argv[2], cv::IMREAD_COLOR);
	} catch (cv::Exception& e) {
	    	std::cerr << "An error occurred during image read. " << e.msg << std::endl;
		return -1;
	}

	if(!left_face.data || !right_face.data) {
		std::cerr << "No image data." << std::endl;
		return -1;
	}

	cv::CascadeClassifier cascade;
	if (!cascade.load("/usr/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml")) {
		std::cerr << "Failed to load face detector." << std::endl;
		return -1;
	}
	std::vector<cv::Rect> left_side, right_side;
	try {
		cascade.detectMultiScale(left_face, left_side);
		cascade.detectMultiScale(right_face, right_side);
	} catch (cv::Exception& e) {
		std::cerr << "An error occurred during face detection. " << e.msg << std::endl;
		return -1;
	}

	if (left_side.empty() || right_side.empty()) {
		std::cerr << "Face detection failed. Make sure to use images with clear visible faces." << std::endl;
		return -1;
	}
	cv::Rect left_rect = left_side[0];
	cv::Rect right_rect = right_side[0];
	
	cv::Mat temp_left_face, temp_right_face;
	
	try {
	cv::resize(left_face(left_rect), temp_left_face, cv::Size(std::max(left_rect.width, right_rect.width), std::max(left_rect.height, right_rect.height)));
	cv::resize(right_face(right_rect), temp_right_face, cv::Size(std::max(left_rect.width, right_rect.width), std::max(left_rect.height, right_rect.height)));
	} catch (cv::Exception& e) {
		std::cerr << "An error occurred during image resize. " << e.msg << std :: endl;
		return -1;
	}

	left_face = temp_left_face;
	right_face = temp_right_face;

	left_rect = cv::Rect(0, 0, left_face.cols, left_face.rows);
	right_rect = cv::Rect(0, 0, right_face.cols, right_face.rows);

	cv::Mat left, right, combined_face;
	try {
		left = left_face(left_rect).colRange(0, left_rect.width / 2);
		right = right_face(right_rect).colRange(right_rect.width / 2, right_rect.width);
		combined_face = cv::Mat(left_rect.height, left_rect.width, left_face.type());
		left.copyTo(combined_face.colRange(0, left_rect.width / 2));
		right.copyTo(combined_face.colRange(left_rect.width / 2, left_rect.width));
	} catch (cv::Exception& e) {
		std::cerr << "An error occurred during face merging. " << e.msg << std::endl;
		return -1;
	}

	double sigma = 1.0;
	double contrast_threshold = 5.0;
	double sharpening_factor = 1.0;
	
	cv::Mat blurred, sharp, grayscale;

	try {
		cv::GaussianBlur(combined_face, blurred, cv::Size(), sigma, sigma);
	} catch (cv::Exception& e) {
		std::cerr << "An error occured during Gaussian blur. " << e.msg << std::endl;
		return -1;
	}

	cv::Mat low_contrast_mask = abs(combined_face - blurred) < contrast_threshold;
	try {
		sharp = combined_face * (1 + sharpening_factor) + blurred * (-sharpening_factor);
	} catch (cv::Exception& e) {
		std::cerr << "An error occurred during sharpeining. " << e.msg << std::endl;
		return -1;
	}
	
	try {
		cv::cvtColor(sharp, grayscale, cv::COLOR_BGR2GRAY);
	} catch (cv::Exception& e) {
		std::cerr << "An error occurred during color conversion. " << e.msg << std::endl;
		return -1;
	}

	try {
		cv::imwrite(output_file, grayscale);
	} catch (cv::Exception& e) {
		std::cerr << "An error occurred during image write. " << e.msg << std::endl;
		return -1;
	}

	std::cout << "The faces have been successfully merged!" << std::endl;

	return 0;
}
