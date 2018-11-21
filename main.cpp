#include <time.h>
#include "header.h"

int main() {

//    string box_info_name = argv[1];

    string box_info_clearname = "164a6a51c2ec7b7.txt";

    FormOperator formOperator;
    string file_path = "../box_info_normal_1121/" + box_info_clearname;
    cout << file_path << endl;
    formOperator.ReadDataFromTxt(file_path);
//    formOperator.ReadDataFromTxt("../test.txt");

    if (formOperator.file_content.empty()){
        cout << "未从识别文档中解析出结果；" << endl;
        exit(1);
    }
    formOperator.analysis(formOperator.file_content, formOperator.bboxs, formOperator.big_bboxs);
    cout << formOperator.bboxs.size() << endl;
    formOperator.filter(formOperator.bboxs, formOperator.big_bboxs[0]);
    cout << "after filter: " << formOperator.bboxs.size() << endl;
    for (int i = 0; i < formOperator.bboxs.size(); ++i) {
        cout << formOperator.bboxs[i].text << " ";
    }
    cout << endl;
    clock_t start, end;
    start = clock();

//    int best_k_row = formOperator.find_best_k(formOperator.bounding_boxs, k_row_min, k_row_max, formOperator.clusters_row, formOperator.centers_row,
//                                          false);
//    int best_k_col = formOperator.find_best_k(formOperator.bounding_boxs, k_col_min, k_col_max, formOperator.clusters_col, formOperator.centers_col,
//                                          true);


        formOperator.cluster_row(formOperator.bboxs, formOperator.clusters_row);
        /*cout << "行内容： " << endl;
        for (int i = 0; i < formOperator.clusters_row.size(); i++){
            for (int j = 0; j < formOperator.clusters_row[i].size(); j++){
                cout << formOperator.clusters_row[i][j].text << " ";
            }
            cout << endl;
        }
        cout << endl;*/
        formOperator.cluster_col(formOperator.bboxs, formOperator.clusters_col);
        /*cout << "列内容：" << endl;
        for (int i = 0; i < formOperator.clusters_col.size(); i++){
            for (int j = 0; j < formOperator.clusters_col[i].size(); j++){
                cout << formOperator.clusters_col[i][j].text << " ";
            }
            cout << endl;
        }
        cout << endl;*/

    end = clock();
    cout << "耗时: " << (double)(end - start) / CLOCKS_PER_SEC << endl;
    /*
    cout << endl << "Best K: " << best_k_col << endl;
    for (int label = 0; label < best_k_col; label++) {
        cout << "第" << label + 1 << "个簇" << endl;
        cout << "簇中心" << formOperator.centers_col[label].x << " " << formOperator.centers_col[label].y << endl;
        vector<Bounding_box> box = formOperator.clusters_col[label];
        for (int i = 0; i < box.size(); i++) {
            cout << box[i].x << " " << box[i].y << " " << box[i].text << endl;
        }
        cout << endl;
    }

    vector<int> cluster_rights;
    vector<int> cluster_lefts;

    formOperator.group_col(formOperator.clusters_col, formOperator.centers_col);
    cout << endl << "After group col" << endl << "K: " << formOperator.centers_col.size() << endl;
    for (int label = 0; label < formOperator.clusters_col.size(); label++) {
        cout << "第" << label + 1 << "个簇" << endl;
        cout << "簇中心" << formOperator.centers_col[label].x << " " << formOperator.centers_col[label].y << endl;
        vector<Bounding_box> box = formOperator.clusters_col[label];
        for (int i = 0; i < box.size(); i++) {
            cout << box[i].x << " " << box[i].y << " " << box[i].text << endl;
        }
        cout << endl;
    }

//    cout << endl << "Best K: " << best_k_row << endl;
//
//    for (int label = 0; label < best_k_row; label++) {
//        cout << "第" << label + 1 << "个簇" << endl;
//        cout << "簇中心" << formOperator.centers_row[label].x << " " << formOperator.centers_row[label].y << endl;
//        vector<Bounding_box> box = formOperator.clusters_row[label];
//        for (int i = 0; i < box.size(); i++)
//        {
//            cout << box[i].x << " " << box[i].y << " " << box[i].text << endl;
//        }
//        cout << endl;
//    }
//
    formOperator.group_row(formOperator.clusters_row, formOperator.centers_row);
//    cout << endl << "After group row" << endl << "K: " << formOperator.centers_row.size() << endl;
//    for (int label = 0; label < formOperator.clusters_row.size(); label++) {
//        cout << "第" << label + 1 << "个簇" << endl;
//        cout << "簇中心" << formOperator.centers_row[label].x << " " << formOperator.centers_row[label].y << endl;
//        vector<Bounding_box> box = formOperator.clusters_row[label];
//        for (int i = 0; i < box.size(); i++)
//        {
//            cout << box[i].x << " " << box[i].y << " " << box[i].text << endl;
//        }
//        cout << endl;
//    }

     */
    start = clock();
    formOperator.group_clusters_res(formOperator.group_res, formOperator.clusters_row, formOperator.clusters_col);
    end = clock();
    cout << "行列： "<< formOperator.group_res.size() << " " << formOperator.group_res[0].size() << endl;
    cout << "解析耗时： " << (double)(end - start) / CLOCKS_PER_SEC << endl;
    cout << "解析结果：" << endl;
    for (int i = 0; i < formOperator.group_res.size(); i++)
    {
        cout << "第" << i + 1 << "行：" << endl;
        for (int j = 0; j < formOperator.group_res[i].size(); j++)
        {

            for (int m = 0; m < formOperator.group_res[i][j].size(); m++)
            {
//                cout << formOperator.group_res[i][j][m].x << " " << formOperator.group_res[i][j][m].y << " " << formOperator.group_res[i][j][m].text << " ";
                cout << formOperator.group_res[i][j][m].text << " ";
            }
            cout << " | ";
        }
        cout << endl;
    }
    cout << endl;

    formOperator.splice_chi_char(formOperator.group_res);
    cout << "合并印刷体汉字结果：" << endl;
    for (int i = 0; i < formOperator.group_res.size(); i++)
    {
        cout << "第" << i + 1 << "行：" << endl;
        for (int j = 0; j < formOperator.group_res[i].size(); j++)
        {

            for (int m = 0; m < formOperator.group_res[i][j].size(); m++)
            {
//                cout << formOperator.group_res[i][j][m].x << " " << formOperator.group_res[i][j][m].y << " " << formOperator.group_res[i][j][m].text << " ";
                cout << formOperator.group_res[i][j][m].text << " ";
            }
            cout << " | ";
        }
        cout << endl;
    }


    //第一行存在较长的中文题目
    if (formOperator.exist_long_chi(formOperator.group_res, true)){
        cout << endl << "before filter: " << endl;
        for (int i = 0; i < formOperator.group_res.size(); i++)
        {
            cout << "第" << i + 1 << "行：" << endl;
            for (int j = 0; j < formOperator.group_res[i].size(); j++)
            {

                for (int m = 0; m < formOperator.group_res[i][j].size(); m++)
                {
//                cout << formOperator.group_res[i][j][m].x << " " << formOperator.group_res[i][j][m].y << " " << formOperator.group_res[i][j][m].text << " ";
                    cout << formOperator.group_res[i][j][m].text << " ";
                }
                cout << " | ";
            }
            cout << endl;
        }
        formOperator.filter_long_chi(formOperator.group_res, formOperator.clusters_row, formOperator.clusters_col);
        formOperator.splice_chi_char(formOperator.group_res);
        cout << "after filter: " << endl;
        for (int i = 0; i < formOperator.group_res.size(); i++)
        {
            cout << "第" << i + 1 << "行：" << endl;
            for (int j = 0; j < formOperator.group_res[i].size(); j++)
            {

                for (int m = 0; m < formOperator.group_res[i][j].size(); m++)
                {
//                cout << formOperator.group_res[i][j][m].x << " " << formOperator.group_res[i][j][m].y << " " << formOperator.group_res[i][j][m].text << " ";
                    cout << formOperator.group_res[i][j][m].text << " ";
                }
                cout << " | ";
            }
            cout << endl;
        }

    }

    //处理中间行中某一行存在较长的中文
    vector<int> coord = formOperator.exist_long_chi_mid(formOperator.group_res);
    while (coord.size() == 3){
        cout << endl << "before mid filter: " << endl;
        for (int i = 0; i < formOperator.group_res.size(); i++)
        {
            cout << "第" << i + 1 << "行：" << endl;
            for (int j = 0; j < formOperator.group_res[i].size(); j++)
            {

                for (int m = 0; m < formOperator.group_res[i][j].size(); m++)
                {
//                cout << formOperator.group_res[i][j][m].x << " " << formOperator.group_res[i][j][m].y << " " << formOperator.group_res[i][j][m].text << " ";
                    cout << formOperator.group_res[i][j][m].text << " ";
                }
                cout << " | ";
            }
            cout << endl;
        }
        formOperator.filter_long_chi_str_bbox(formOperator.group_res, formOperator.clusters_row, formOperator.clusters_col,
                                         coord[0], coord[1], coord[2]);
        formOperator.splice_chi_char(formOperator.group_res);
        cout << "after mid filter: " << endl;
        for (int i = 0; i < formOperator.group_res.size(); i++)
        {
            cout << "第" << i + 1 << "行：" << endl;
            for (int j = 0; j < formOperator.group_res[i].size(); j++)
            {

                for (int m = 0; m < formOperator.group_res[i][j].size(); m++)
                {
//                cout << formOperator.group_res[i][j][m].x << " " << formOperator.group_res[i][j][m].y << " " << formOperator.group_res[i][j][m].text << " ";
                    cout << formOperator.group_res[i][j][m].text << " ";
                }
                cout << " | ";
            }
            cout << endl;
        }
        coord.clear();
        coord = formOperator.exist_long_chi_mid(formOperator.group_res);

    }

    formOperator.init(formOperator.stem_map);

    start = clock();
    formOperator.formula = formOperator.match_formula(formOperator.stem_map, formOperator.group_res);
    end = clock();
    cout << "运算匹配时间： " << (double)(end - start) / CLOCKS_PER_SEC << endl;
    cout << "题目运算方式： ";

        if (formOperator.formula == -1){
            cout << "未知运算" << endl;
        }
        else if (formOperator.formula >= formOperator.rule.size()){
            cout << formOperator.normal_rule[formOperator.formula - formOperator.rule.size()] << endl;
        }
        else {
            cout << formOperator.rule[formOperator.formula] << endl;
        }

    return 0;

}

