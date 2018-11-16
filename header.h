//
// Created by sunwanqi on 2018/11/9.
//

#ifndef FORM_HEADER_H
#define FORM_HEADER_H


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <cmath>
#include <unordered_map>
#include <map>
using namespace std;
#define k 9

// 定义行、列的范围
#define k_row_min 3
#define k_row_max 15
#define k_col_min 3
#define k_col_max 15


// 单个检测框
struct Bbox{
    int x;
    int y;
    int width;
    int height;
    int class_idx;
    string text = "空";
};

// 行、列检测框
struct Row_box{
    int x;
    int y;
    int width;
    int height;
    int row_col;
    vector<Bbox> single_boxs;
};

//大框和序号的检测框 200、201、202、203、102
struct Big_bbox{
    int x;
    int y;
    int width;
    int height;
    int class_idx;
};

// 聚类中心
struct Cluster_center{
    float x;
    float y;
};


// 定义对表格识别结果的处理类
class FormOperator{
private:

public:
    int formula = -1;
    const int chi_length = 4 * 3;
    vector<string> file_content;
    vector<Bbox> bboxs;
    vector<Big_bbox> big_bboxs;
    vector<vector<Bbox>> clusters_row;
    vector<vector<Bbox>> clusters_col;
    vector<Cluster_center> centers_row;
    vector<Cluster_center> centers_col;


    vector<vector<vector<Bbox>>> group_res;

    map< pair<pair <string,string>, string>, int > stem_map;
    vector<string> rule{"加法", "减法", "乘法", "除法"};

    //题干，之后可以在此扩充
    vector<vector<string>> stem{

            {"单价", "数量", "金额", "2"},
            {"数量", "单价", "金额", "2"},
            {"单价", "数量", "总价", "2"},
            {"数量", "单价", "总价", "2"},
            {"速度", "时间", "路程", "2"},
            {"时间", "路程", "路程", "2"},
            {"乘数", "乘数", "积", "2"},
            {"因数", "因数", "积", "2"},
            {"被除数", "除数", "商", "3"},
            {"加数", "加数", "和", "0"},
            {"被减数", "减数", "差", "1"},
            {"单产量", "数量", "总价", "2"}

    };

    //判断第一行是否存在很长的中文字符串，这种长的中文字符串是不会出现在表格里面的，所以判定为表格外的干扰
    bool exist_long_chi(vector<vector<vector<Bbox>>>& group_res);

    //去掉第一行的干扰信息
    void filter_long_chi_str(vector<vector<vector<Bbox>>>& group_res, vector<vector<Bbox>>& clusters_row, vector<vector<Bbox>>& clusters_col);

    //将要运算的数字运算验证
    void compute(vector<vector<string>>& nums, int rule);

    //根据拿下来的res，进行匹配运算规则
    int match_formula(map< pair<pair <string,string>, string>, int >& stem_map, vector<vector<vector<Bbox>>>& group_res);

    //将所给的题型初始化到指定数据结构
    void init(map< pair<pair <string,string>, string>, int >& stem_map);

    //用k-means实现聚类，来观察有几行几列
    //计算一行里面的bounding box的距离
    double dist_row(const Cluster_center a, const Bbox b);

    //计算一列里面的bounding box的距离
    double dist_col(const Cluster_center a, const Bbox b);

    //计算欧式距离
    double dist(const Cluster_center a, const Bbox b);

    //查找两个vector的交集
    vector<Bbox> intersection(vector<Bbox>& A, vector<Bbox>& B);

    //合并行、列的结果
    void group_clusters_res(vector<vector<vector<Bbox>>>& group_res, vector<vector<Bbox>>& clusters_row, vector<vector<Bbox>>& clusters_col);

    //拼接在一个格子的印刷体汉字
    void splice_chi_char(vector<vector<vector<Bbox>>>& group_res);

    //用IOU的思想对 行 进行聚类
    void cluster_row(vector<Bbox>& bboxs, vector<vector<Bbox>>& clusters_row);

    //根据 行聚类结果对 列 进行聚类
    void cluster_col2(vector<vector<Bbox>>& clusters_row, vector<vector<vector<Bbox>>> group_res);

    //用IOU的思想对 列 进行聚类
    void cluster_col(vector<Bbox>& bboxs, vector<vector<Bbox>>& clusters_col);

    //计算簇内的平方误差
    float getVar(vector<vector<Bbox>>& clusters, vector<Cluster_center>& centers, const int cur_k, bool row_or_col);

    //根据算与聚类中心的距离，分配
    int cluster_of_tuple(vector<Cluster_center>& centers, Bbox bbox, const int cur_k, bool row_or_col);

    //计算簇的中心
    Cluster_center get_means(vector<Bbox>& clusters);

    //单个K的k-means算法
    float k_means(vector<Bbox>& bboxs, const int cur_k, vector<vector<Bbox>>& clusters, vector<Cluster_center>& centers, bool row_or_col);
    //找最优的K值
    int find_best_k(vector<Bbox>& bboxs, const int k_min, const int k_max, vector<vector<Bbox>>& clusters, vector<Cluster_center>& centers, bool row_or_col);

    //group聚类行的结果
    void group_row(vector<vector<Bbox>>& clusters, vector<Cluster_center>& centers);

    //group聚类列的结果
    void group_col(vector<vector<Bbox>>& clusters, vector<Cluster_center>& centers);

    //按照上和左的位置进行排序
    static bool compare(const Bbox a, const Bbox b);

    static bool compare_col(const Bbox a, const Bbox b);

    //行结果的排序规则
    static bool comp_row(const vector<Bbox>& a, const vector<Bbox>& b);

    //列结果的排序规则
    static bool comp_col(const vector<Bbox>& a, const vector<Bbox>& b);

    //对一个格子里面的box按从左到右排序
    static bool comp_col_single(const Bbox a, const Bbox b);

    static bool comp_row_single(const Bbox a, const Bbox b);

    //将拆分下来的字符串，按照内容，封装到一个bounding box结构体对象里面
    void analysis(vector<string>& file_content, vector<Bbox>& bboxs, vector<Big_bbox>& big_bboxs);

    //过滤掉大框外面的小框
    void filter(vector<Bbox>& bboxs, Big_bbox big_bbox);

    //实现字符串的split函数
    vector<string> split_line(string single_line, char symbol);

    //从识别结果的TXT中读取数据到内存
    void ReadDataFromTxt(string filename);
};

#endif //FORM_HEADER_H
