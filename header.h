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
#include <map>
//using namespace std;
#define k 9

// 定义行、列的范围
#define k_row_min 3
#define k_row_max 15
#define k_col_min 3
#define k_col_max 15

// 最后要的结果
struct Res{
    int x;
    int y;
    int width;
    int height;
    std::vector<std::string> splice_result;
};

// 单个检测框
struct Bbox{
    int x;
    int y;
    int width;
    int height;
    int class_idx;
    std::string text = "";
};

// 行、列检测框
struct Row_box{
    int x;
    int y;
    int width;
    int height;
    int row_col;
    std::vector<Bbox> single_boxs;
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
    int nums_size = 3; //默认是计算式是三个元素
    const int chi_length = 4 * 3;
    const int chi_length_mid = 7 * 3;
    std::vector<std::string> file_content;
    std::vector<Bbox> bboxs;
    std::vector<Big_bbox> big_bboxs;
    std::vector<std::vector<Bbox>> clusters_row;
    std::vector<std::vector<Bbox>> clusters_col;
    std::vector<Cluster_center> centers_row;
    std::vector<Cluster_center> centers_col;

    std::vector<std::vector<std::vector<std::string>>> nums_result;

    //手写体框的位置记录
    std::vector<std::vector<bool>> hand_write_index;
    std::vector<Bbox> result_boxs;

    //最后要的拼接的字符串数组，包括表格数字后面的单位
    std::vector<std::vector<std::string>> splice_res;

    std::vector<Res> final_res;

    std::vector<std::vector<std::vector<Bbox>>> group_res;

    std::map< std::pair<std::pair <std::string,std::string>, std::string>, int > stem_map;

    std::vector<std::string> rule{"加法", "减法", "乘法", "除法", "除法带余数"};

    //题干，之后可以在此扩充，设置四则运算分别为 0、1、2、3
    std::vector<std::vector<std::string>> stem{

            {"单价", "数量", "金额", "2"},
            {"数量", "单价", "总价", "2"},
            {"单价", "数量", "总价", "2"},
            {"数量", "单价", "金额", "2"},
            {"速度", "时间", "路程", "2"},
            {"时间", "路程", "路程", "2"},
            {"乘数", "乘数", "积", "2"},
            {"因数", "因数", "积", "2"},
            {"被除数", "除数", "商", "3"},
            {"被减数", "减数", "差", "1"},
            {"单产量", "数量", "总价", "2"},
            {"被减数", "减数", "差", "1"},
            {"加数", "加数", "和", "0"},

            {"原有", "卖出", "剩", "1"},
            {"原有", "卖出", "有", "1"},

            {"原有", "借走", "还剩", "1"},
            {"原有", "吃了", "还剩", "1"},
            {"原有", "借走", "剩下", "1"},
            {"原有", "借出", "还剩", "1"},
            {"原有", "借走", "现有", "1"},


            {"总数量", "卖出", "还剩", "1"},
            {"新进", "卖出", "还剩", "1"},
            {"借出", "还剩", "原有", "0"},
            {"借出", "还剩", "原来有", "0"},
            {"收入", "支出", "结余", "1"},

            {"原有", "又买来", "一共有", "0"},
            {"原有", "又买来", "现有", "0"},
            {"原有", "购进来", "一共有", "0"},
            {"原有", "吃掉", "剩", "1"},
            {"原有", "购进", "现有", "0"},
            {"购进", "卖出", "剩", "1"},
            {"卖出", "剩", "原有", "0"},

            {"男生", "女生", "一共", "0"},
            {"总质量", "卖出", "还剩", "1"},
            {"单价", "售出", "收入", "2"},
            {"单价", "售出", "总", "2"},
            {"原价", "现价", "优惠", "1"},
            {"原价", "现价", "便宜", "1"},
            {"价钱", "付出", "找回", "1"},
            {"总价", "数量", "单价", "3"},
            {"总价", "单价", "数量", "3"},
            {"售出", "单价", "总价", "2"},
            {"摆出", "拿出", "还剩", "2"},
            {"班级", "图书", "平均", "3"},
            {"一", "二", "合计", "0"},
            {"上", "下", "总", "0"}
    };

    //更加一般情况的运算法则
    std::vector<std::string> normal_rule{"连加法", "连减法", "长方体体积", "长方形周长", "正方形周长", "正方形面积",
                                "长方形面积", "三角形面积", "平行四边形面积", "梯形面积", "除法", "圆周长面积",
                                "正方体表面积体积", "圆柱体表面积体积", "既加又减运算"};

    //更加一般的情况
    std::vector<std::vector<std::string>> normal_stem{
            {"一", "二", "三", "合计", "0"},
            {"一", "二", "三", "总数", "0"},
            {"一", "二", "三", "四", "合计", "0"},
            {"金", "银", "铜", "总", "0"},
            {"原有", "花去", "花去", "还剩", "1"},
            {"原有", "卖出", "卖出", "还剩", "1"},
//            {"长", "宽", "高", "体积", "2"},
            {"长", "宽", "高", "体积", "2"},
            {"长", "宽", "周长", "3"},
            {"边长", "周长", "4"},
            {"边长", "面积", "5"},
            {"长", "宽", "面积", "6"},
            {"底", "高", "面积", "7"},
            {"上底", "下底", "高", "面积", "9"},
            {"班级", "总", "平均", "10"},
            {"半径", "直径", "周长", "面积", "11"},
            {"跳绳", "短跑", "跳远", "合计", "0"},
            {"棱长", "表面积", "体积", "12"},
            {"做了", "做了", "一共", "0"},
            {"杨树", "柳树", "松树", "总计", "0"},
            {"半径", "直径", "高", "表面积", "体积", "13"},
            {"原来", "进", "出", "剩", "14"}

    };

    std::vector<std::string> key_word{"长方形", "正方形", "三角形"};

    //单独处理正方形表面积体积问题
    void process_square();

    //单独处理圆直径周长面积题
    void process_circle();

    //重新整理输出结构体
    void arrange_res();

    //判断每一行的列是否一样，不一样的话就是非法框，不用匹配运算，直接过滤掉
    bool is_legal_form();

    //将解析一般运算
    void transform_normal_result();

    //将解析的四则运算
    void transform_result();

    //调用整个识别运算规则
    void run_result();

    //查看a是否是存在与b中
    bool part_match(std::string a, std::string b);

    //输出中间行的特别长的中文干扰
    std::vector<int> exist_long_chi_mid(std::vector<std::vector<std::vector<Bbox>>>& group_res);

    //获取非四则运算匹配之后的结果
    void get_normal_res(std::vector<std::vector<std::vector<Bbox>>>& group_res, std::vector<std::vector<std::string>>& nums, std::vector<int>& rows, std::vector<int>& cols);

    //非四则运算的匹配
    int match_normal_formula(std::vector<std::vector<std::vector<Bbox>>>& group_res, std::vector<int>& rows, std::vector<int>& cols);

    //添加前缀匹配规则
    bool string_blurry_match(std::string a, std::string b);

    //单独处理被除数、除数、商、余数这种情况
    bool is_has_remainder(std::vector<std::vector<std::vector<Bbox>>>& group_res, std::vector<int>& row_index, std::vector<int>& col_index);

    //判断第一行是否存在很长的中文字符串，这种长的中文字符串是不会出现在表格里面的，所以判定为表格外的干扰
    std::vector<std::vector<int>> exist_long_chi(std::vector<std::vector<std::vector<Bbox>>>& group_res);

    //去掉group_res其中某一个Bbox，并重新列聚类，行列分开操作
    void filter_long_chi_str_bbox(std::vector<std::vector<std::vector<Bbox>>>& group_res, std::vector<std::vector<Bbox>>& clusters_row, \
                                std::vector<std::vector<Bbox>>& clusters_col, int row, int col, int num);

    //去掉第一行的干扰信息
    void filter_long_chi(std::vector<std::vector<std::vector<Bbox>>>& group_res, std::vector<std::vector<Bbox>>& clusters_row, std::vector<std::vector<Bbox>>& clusters_col, std::vector<std::vector<int>>& coord);

    //输出干扰的bbox
    void filter_useless_bbox(std::vector<std::vector<std::vector<Bbox>>>& group_res, std::vector<Bbox>& bboxs, int row, int col, int num);

    //删除第一行的Bboxs
    void filter_useless_bbox_firstrow(std::vector<std::vector<std::vector<Bbox>>>& group_res, std::vector<Bbox>& bboxs);

    //将要运算的数字运算验证
    void compute(std::vector<std::vector<std::string>>& nums, int rule);

    //根据拿下来的res，进行匹配运算规则
    int match_formula(std::map< std::pair<std::pair <std::string,std::string>, std::string>, int >& stem_map, std::vector<std::vector<std::vector<Bbox>>>& group_res);

    //将所给的题型初始化到指定数据结构
    void init(std::map< std::pair<std::pair <std::string,std::string>, std::string>, int >& stem_map);

    //用k-means实现聚类，来观察有几行几列
    //计算一行里面的bounding box的距离
    double dist_row(const Cluster_center a, const Bbox b);

    //计算一列里面的bounding box的距离
    double dist_col(const Cluster_center a, const Bbox b);

    //计算欧式距离
    double dist(const Cluster_center a, const Bbox b);

    //查找两个std::vector的交集
    std::vector<Bbox> intersection(std::vector<Bbox>& A, std::vector<Bbox>& B);

    //合并行、列的结果
    void group_clusters_res(std::vector<std::vector<std::vector<Bbox>>>& group_res, std::vector<std::vector<Bbox>>& clusters_row, std::vector<std::vector<Bbox>>& clusters_col);

    //拼接在一个格子的印刷体汉字
    void splice_chi_char(std::vector<std::vector<std::vector<Bbox>>>& group_res);

    //用IOU的思想对 行 进行聚类
    void cluster_row(std::vector<Bbox>& bboxs, std::vector<std::vector<Bbox>>& clusters_row);

    //根据 行聚类结果对 列 进行聚类
    void cluster_col2(std::vector<std::vector<Bbox>>& clusters_row, std::vector<std::vector<std::vector<Bbox>>> group_res);

    //用IOU的思想对 列 进行聚类
    void cluster_col(std::vector<Bbox>& bboxs, std::vector<std::vector<Bbox>>& clusters_col);

    //计算簇内的平方误差
    float getVar(std::vector<std::vector<Bbox>>& clusters, std::vector<Cluster_center>& centers, const int cur_k, bool row_or_col);

    //根据算与聚类中心的距离，分配
    int cluster_of_tuple(std::vector<Cluster_center>& centers, Bbox bbox, const int cur_k, bool row_or_col);

    //计算簇的中心
    Cluster_center get_means(std::vector<Bbox>& clusters);

    //单个K的k-means算法
    float k_means(std::vector<Bbox>& bboxs, const int cur_k, std::vector<std::vector<Bbox>>& clusters, std::vector<Cluster_center>& centers, bool row_or_col);
    //找最优的K值
    int find_best_k(std::vector<Bbox>& bboxs, const int k_min, const int k_max, std::vector<std::vector<Bbox>>& clusters, std::vector<Cluster_center>& centers, bool row_or_col);

    //group聚类行的结果
    void group_row(std::vector<std::vector<Bbox>>& clusters, std::vector<Cluster_center>& centers);

    //group聚类列的结果
    void group_col(std::vector<std::vector<Bbox>>& clusters, std::vector<Cluster_center>& centers);

    //按照上和左的位置进行排序
    static bool compare(const Bbox &a, const Bbox &b);

    static bool compare_col(const Bbox a, const Bbox b);

    //行结果的排序规则
    static bool comp_row(const std::vector<Bbox>& a, const std::vector<Bbox>& b);

    //列结果的排序规则
    static bool comp_col(const std::vector<Bbox>& a, const std::vector<Bbox>& b);

    //对一个格子里面的box按从左到右排序
    static bool comp_col_single(const Bbox a, const Bbox b);

    static bool comp_row_single(const Bbox a, const Bbox b);

    //将拆分下来的字符串，按照内容，封装到一个bounding box结构体对象里面
    void analysis(std::vector<std::string>& file_content, std::vector<Bbox>& bboxs, std::vector<Big_bbox>& big_bboxs);

    //过滤掉大框外面的小框
    void filter(std::vector<Bbox>& bboxs, Big_bbox big_bbox);

    //实现字符串的split函数
    std::vector<std::string> split_line(std::string single_line, char symbol);

    //从识别结果的TXT中读取数据到内存
    void ReadDataFromTxt(std::string filename);
};

#endif //FORM_HEADER_H
