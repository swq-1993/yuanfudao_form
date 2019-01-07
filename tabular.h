// Created by zhuss 2018-1016

#ifndef RESEARCH_MENTOR_TABULAR_H
#define RESEARCH_MENTOR_TABULAR_H

#include <opencv2/opencv.hpp>
#include "interface/MentorBase_types.h" //YOLO BOX Definition
#include "interface/MentalMath.h" //RecogResult Definition
#include "recognization/misc.h"
#include "mentor/mentor_seg.h"

using namespace std;
using namespace cv;
using namespace  ::com::fenbi::research::mentor;


class Tabular {
public:
	Tabular(const string& query_id,
			const Mat &src_img,
			Box layout_box,
			const vector<Box> &basic_boxes,
			MentorSeg &hwseg):src_img_(src_img), hwseg_(hwseg){
		query_id_ = query_id;
		layout_box_ = layout_box;
		basic_boxes_.insert(basic_boxes_.end(), basic_boxes.begin(), basic_boxes.end());
		Mat tempMat = src_img_.clone();
		// for (auto &bb : basic_boxes) {
		// 	cv::rectangle(tempMat, Rect(bb.x, bb.y, bb.width, bb.height), cv::Scalar(0, 0, 255));
		// }
		// imshow("image", tempMat);
		// waitKey();

	}
	void process();
	vector<RecogResult> get_recog_results(){return recog_results_;}

private:
	Box layout_box_;
	string query_id_;
	const Mat &src_img_;
	MentorSeg &hwseg_;
	string text_result_;
	vector<Box> basic_boxes_;
	vector<RecogResult> recog_results_;
};

#endif
