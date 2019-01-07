// Created by zhuss 2018-1016

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include "question/tabular.h"
#include "question/generate_linesegs.h"
#include "mentor/CaffePartRecog_client.h"
#include <opencv2/opencv.hpp>
#include "interface/MentorBase_types.h" //Yolo Box
#include "interface/MentalMath.h" //RecogResult Definition
#include "question/tabular_impl.h" //RecogResult Definition
#include <time.h>
#include "mentor_server/score_return.h"
using namespace std;
using namespace cv;
using namespace  ::com::fenbi::research::mentor;

//#define DEBUG_DRAWBOX
void Tabular::process(){
	clock_t start, end;

	FormOperator formOperator;
	formOperator.query_id_ = query_id_;
	formOperator.src_img_ = src_img_;
	formOperator.analysis_mentor_boxes(layout_box_, basic_boxes_);
	/*
	cout << "layout_box_ :1"<< endl;
	cout << "query_id_:" << query_id_ << endl;
	cout << "basic_boxes_:" << basic_boxes_.size() << endl;
	*/
	#ifdef DEBUG_DRAWBOX
		vector<Rect> tables;
		Mat processed_mat;
		processed_mat = src_img_.clone();
		for (int i = 0; i < formOperator.bboxs.size(); i++){
			cv::Rect rect(formOperator.bboxs[i].x, formOperator.bboxs[i].y, formOperator.bboxs[i].width, formOperator.bboxs[i].height);
			tables.push_back(rect);
		}
		#ifdef DEBUG_FORM
			LOG(INFO) << query_id_ + " tables.size() =   " << tables.size();
		#endif
		for (auto &rr : tables) {
		 rectangle(processed_mat, rr, cv::Scalar(0, 0, 255));
	 }
	 imshow("image", processed_mat);
	 waitKey();
 #endif

 	formOperator.run_result();
	//cout << "final_res:" << formOperator.final_res.size() << endl;

    vector<vector<string>> recog_strings;
    vector<Rect> regions;
    for (int i = 0; i < formOperator.final_res.size(); i++){
        recog_strings.push_back(formOperator.final_res[i].splice_result);
        regions.push_back(Rect(formOperator.final_res[i].x, formOperator.final_res[i].y, formOperator.final_res[i].width, formOperator.final_res[i].height));
    }
    hwseg_.makeCompatibleToSource(regions);

    recog_results_ = CheckAllResult(1, recog_strings, regions);
}
