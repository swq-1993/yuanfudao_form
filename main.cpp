#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <cmath>
#include <unordered_map>
#include <time.h>
using namespace std;
#define k 9

// 定义行、列的范围
#define k_row_min 3
#define k_row_max 15
#define k_col_min 3
#define k_col_max 15


// 单个检测框
struct Bounding_box{
    int x;
    int y;
    int width;
    int height;
    int class_idx;
    string text = "";
};

// 行、列检测框
struct Row_box{
    int x;
    int y;
    int width;
    int height;
    int row_col;
    vector<Bounding_box> single_boxs;
};

//大框和序号的检测框 203和102
struct Big_bounding_box{
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
    vector<string> file_content;
    vector<Bounding_box> bounding_boxs;
    vector<Big_bounding_box> big_bounding_boxs;
    vector<vector<Bounding_box>> clusters_row;
    vector<vector<Bounding_box>> clusters_col;
    vector<Cluster_center> centers_row;
    vector<Cluster_center> centers_col;

    vector<vector<vector<Bounding_box>>> group_res;

    //用k-means实现聚类，来观察有几行几列
    //计算一行里面的bounding box的距离
    int dist_row(const Cluster_center a, const Bounding_box b) {
        return float(a.y - b.y) > 0 ? float(a.y - b.y) : float(a.y - b.y) * (-1.0);
    }

    //计算一列里面的bounding box的距离
    int dist_col(const Cluster_center a, const Bounding_box b) {
        return float(a.x - b.x) > 0 ? float(a.x - b.x) : float(a.x - b.x) * (-1.0);
    }

    //计算欧式距离
    int dist(const Cluster_center a, const Bounding_box b) {
        return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }


    //查找两个vector的交集
    vector<Bounding_box> intersection(vector<Bounding_box>& A, vector<Bounding_box>& B)
    {
        vector<Bounding_box> res;
        unordered_map<int, Bounding_box>part_a, part_b;
        for(auto a : A) {
            part_a.insert(make_pair(a.x, a));
        }
        for (auto b : B) {
            part_b.insert(make_pair(b.x, b));
        }

        unordered_map<int, Bounding_box> :: iterator iter = part_a.begin();
        while (iter != part_a.end()) {
            if (part_b.find(iter->first) != part_b.end()) {
                res.push_back(iter->second);
            }
            iter++;
        }

        return res;
    }
    //合并行、列的结果
    void group_clusters_res(vector<vector<vector<Bounding_box>>>& group_res, vector<vector<Bounding_box>>& clusters_row, vector<vector<Bounding_box>>& clusters_col) {
        int rows = clusters_row.size();
        int cols = clusters_col.size();
        vector<Bounding_box> tmp;
        vector<vector<Bounding_box>> tmp_row;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                //求一行与一列的bounding_box的交集
//                set_intersection(clusters_row[i].begin(), clusters_row[i].end(), clusters_col[j].begin(), clusters_col[j].end(), back_inserter(tmp));
                tmp = intersection(clusters_row[i], clusters_col[j]);
                sort(tmp.begin(), tmp.end(), comp_col_single);
                tmp_row.push_back(tmp);
                tmp.clear();
            }
            group_res.push_back(tmp_row);
            tmp_row.clear();
        }

    }

    //拼接在一个格子的印刷体汉字
    void splice_chi_char(vector<vector<vector<Bounding_box>>>& group_res)
    {
        for (int i = 0; i < group_res.size(); i++)
        {
            for (int j = 0; j < group_res[i].size(); j++)
            {
                //一个格子里面的所有bounding_box
                for (int m = 0; m < group_res[i][j].size(); m++)
                {
                    //如果发现是印刷体汉字
                    if (group_res[i][j][m].class_idx == 100)
                    {
                        for (int n = m + 1; n < group_res[i][j].size(); n++)
                        {
                            //如果第二个连续的字符也是印刷体汉字
                            if (group_res[i][j][n].class_idx == 100)
                            {
                                //合并连续的印刷体汉字
                                group_res[i][j][m].width = group_res[i][j][n].x + group_res[i][j][n].width - group_res[i][j][m].x;
                                group_res[i][j][m].y = min(group_res[i][j][m].y, group_res[i][j][n].y);
                                group_res[i][j][m].height = max(group_res[i][j][m].y + group_res[i][j][m].height, \
                                                                group_res[i][j][n].y + group_res[i][j][n].height) - group_res[i][j][m].y;
                                group_res[i][j][m].text += group_res[i][j][n].text;

                                //删除第二个印刷体汉字
                                group_res[i][j].erase(group_res[i][j].begin() + n);
                                n = m;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    //用IOU的思想对 行 进行聚类
    void cluster_row(vector<Bounding_box>& bounding_boxs, vector<vector<Bounding_box>>& clusters_row)
    {
        vector<Bounding_box> copy = bounding_boxs;
        vector<Bounding_box> tmp;
        for (int i = 0; i < copy.size(); i++)
        {
            int tmp_top = copy[i].y;
            int tmp_bottom = copy[i].y + copy[i].height;
            tmp.push_back(copy[i]);
            for (int j = 0; j < copy.size(); j++)
            {
                if (i == j){
                    continue;
                }
                int Iou_min = max(tmp_top, copy[j].y);
                int Iou_max = min(tmp_bottom, copy[j].y + copy[j].height);
                if (Iou_max >= Iou_min)
                {
                    tmp.push_back(copy[j]);
                    tmp_top = min(tmp_top, copy[j].y);
                    tmp_bottom = max(tmp_bottom, copy[j].y + copy[j].height);

                    copy.erase(copy.begin() + j);
                    j = i;
                }
            }
            clusters_row.push_back(tmp);
            tmp.clear();
        }
        sort(clusters_row.begin(), clusters_row.end(), comp_row);
    }


    //用IOU的思想对 列 进行聚类
    void cluster_col(vector<Bounding_box>& bounding_boxs, vector<vector<Bounding_box>>& clusters_col)
    {
        vector<Bounding_box> copy = bounding_boxs;
        vector<Bounding_box> tmp;
        for (int i = 0; i < copy.size(); i++)
        {
            int tmp_left = copy[i].x;
            int tmp_right = copy[i].x + copy[i].width;
            tmp.push_back(copy[i]);
            for (int j = 0; j < copy.size(); j++)
            {
                if (i == j){
                    continue;
                }
                int Iou_min = max(tmp_left, copy[j].x);
                int Iou_max = min(tmp_right, copy[j].x + copy[j].width);
                if (Iou_max >= Iou_min)
                {
                    tmp.push_back(copy[j]);
                    tmp_left = min(tmp_left, copy[j].x);
                    tmp_right = max(tmp_right, copy[j].x + copy[j].width);

                    copy.erase(copy.begin() + j);
                    j = i;
                }
            }
            clusters_col.push_back(tmp);
            tmp.clear();
        }
        sort(clusters_col.begin(), clusters_col.end(), comp_col);
    }

    //计算簇内的平方误差
    float getVar(vector<vector<Bounding_box>>& clusters, vector<Cluster_center>& centers, const int cur_k, bool row_or_col) {
        float var = 0;
        for (int i = 0; i < cur_k; i++)
        {
            vector<Bounding_box> cluster = clusters[i];
            for (int j = 0; j < cluster.size(); j++)
            {
                if (row_or_col) {
                    var += dist_col(centers[i], cluster[j]);
                }
                else {
                    var += dist_row(centers[i], cluster[j]);
                }

            }
        }
        return var;
    }

    //根据算与聚类中心的距离，分配
    int cluster_of_tuple(vector<Cluster_center>& centers, Bounding_box bounding_box, const int cur_k, bool row_or_col) {
        int dist = dist_row(centers[0], bounding_box);
        if (row_or_col) {
            dist = dist_col(centers[0], bounding_box);
        }
        int tmp;
        int label = 0;
        for (int i = 1; i < cur_k; i++) {
            tmp = dist_row(centers[i], bounding_box);
            if (row_or_col) {
                tmp = dist_col(centers[i], bounding_box);
            }
            if (tmp < dist) {
                dist = tmp;
                label = i;
            }
        }
        return label;
    }

    //计算簇的中心
    Cluster_center get_means(vector<Bounding_box>& clusters) {
        int num = clusters.size();
        float meansX= 0.0, meansY = 0.0;
        Cluster_center center;
        for (int i = 0; i < num; i++) {
            meansX += clusters[i].x;
            meansY += clusters[i].y;
        }
        center.x = meansX / num;
        center.y = meansY / num;
        return center;
    }

    //单个K的k-means算法
    float k_means(vector<Bounding_box>& bounding_boxs, const int cur_k, vector<vector<Bounding_box>>& clusters, vector<Cluster_center>& centers, bool row_or_col) {
        //vector<vector<Bounding_box>> clusters(k);
        clusters.resize(cur_k);
        centers.resize(cur_k);
        int i = 0;
        for (i = 0; i < cur_k; i++) {
            centers[i].x = float(bounding_boxs[i * bounding_boxs.size() / cur_k].x);
            centers[i].y = float(bounding_boxs[i * bounding_boxs.size() / cur_k].y);
        }
        int label = 0;
        for (i = 0; i != bounding_boxs.size(); i++) {
            label = cluster_of_tuple(centers, bounding_boxs[i], cur_k, row_or_col);
            clusters[label].push_back(bounding_boxs[i]);
        }

        for (label = 0; label < cur_k; label++) {
            cout << "第" << label + 1 << "个簇" << endl;
            vector<Bounding_box> box = clusters[label];
            for (i = 0; i < box.size(); i++) {
                cout << box[i].x << " " << box[i].y << " " << box[i].text << endl;
            }
            cout << endl;
        }

        float oldVal = -1;
        float newVal = getVar(clusters, centers, cur_k, row_or_col);
        while (abs(newVal - oldVal) >= 1) {
            for (i = 0; i < cur_k; i++) {
                centers[i] = get_means(clusters[i]);
            }
            oldVal = newVal;
            newVal = getVar(clusters, centers, cur_k, row_or_col);

            //清空每个簇
            for (i = 0; i < cur_k; i++) {
                clusters[i].clear();
            }

            for (i = 0; i != bounding_boxs.size(); i++) {
                label = cluster_of_tuple(centers, bounding_boxs[i], cur_k, row_or_col);
                clusters[label].push_back(bounding_boxs[i]);
            }

            for (label = 0; label < cur_k; label++) {
                cout << "第" << label + 1 << "个簇" << endl;
                vector<Bounding_box> box = clusters[label];
                for (i = 0; i < box.size(); i++) {
                    cout << box[i].x << " " << box[i].y << " " << box[i].text << endl;
                }
                cout << endl;
            }
        }
        return newVal;
    }

    //找最优的K值
    int find_best_k(vector<Bounding_box>& bounding_boxs, const int k_min, const int k_max, vector<vector<Bounding_box>>& clusters, vector<Cluster_center>& centers, bool row_or_col) {
        int final_k = 0;
        int cur_k = k_min;
        float final_loss = 0.0;
        vector<vector<Bounding_box>> tmp;
        vector<Cluster_center> tmp_centers;
        while (cur_k <= k_max) {
            if (cur_k == k_min) {
                final_loss = k_means(bounding_boxs, cur_k, clusters, centers, row_or_col);
                final_k = cur_k;
            }
            else {
                float loss = k_means(bounding_boxs, cur_k, tmp, tmp_centers, row_or_col);
                cout << "loss: " << loss << endl;
                if (loss < final_loss) {
                    final_k = cur_k;
                    //清空每个簇
//                    for (int i = 0; i < cur_k; i++) {
//                        clusters[i].clear();
//                    }
                    clusters = tmp;
                    centers = tmp_centers;
                }
            }
            cur_k++;
        }
        return final_k;
    }

    //group聚类行的结果
    void group_row(vector<vector<Bounding_box>>& clusters, vector<Cluster_center>& centers) {
        vector<int> cluster_bottoms(clusters.size());
        vector<int> cluster_tops(clusters.size());
        for (int i = 0; i < clusters.size(); i++) {
            int tmp_bottom = 0;
            int tmp_top = INT_MAX;
            for (int j = 0; j < clusters[i].size(); j++) {
                if (clusters[i][j].y + clusters[i][j].height > tmp_bottom) {
                    tmp_bottom = clusters[i][j].y + clusters[i][j].height;
                }
                if (clusters[i][j].y < tmp_top)
                {
                    tmp_top = clusters[i][j].y;
                }
            }
            cluster_bottoms[i] = tmp_bottom;
            cluster_tops[i] = tmp_top;
        }

        for (int i = 0; i < clusters.size(); i++) {
            for (int j = i + 1; j < clusters.size(); j++) {
                int Iou_min = max(cluster_tops[i], cluster_tops[j]);
                int Iou_max = min(cluster_bottoms[i], cluster_bottoms[j]);
                if (Iou_max >= Iou_min) {
                    clusters[i].insert(clusters[i].end(), clusters[j].begin(), clusters[j].end());
                    float new_center_x = (centers[i].x + centers[j].x) / 2;
                    float new_center_y = (centers[i].y + centers[j].y) / 2;
                    centers[i].x = new_center_x;
                    centers[i].y = new_center_y;
                    cluster_bottoms[i] = max(cluster_bottoms[i], cluster_bottoms[j]);
                    cluster_tops[i] = min(cluster_tops[i], cluster_tops[j]);

                    clusters.erase(clusters.begin() + j);
                    centers.erase(centers.begin() + j);
                    cluster_bottoms.erase(cluster_bottoms.begin() + j);
                    cluster_tops.erase(cluster_tops.begin() + j);
                    j = i;
                }
            }
        }

        sort(clusters.begin(), clusters.end(), comp_row);
    }

    //group聚类列的结果
    void group_col(vector<vector<Bounding_box>>& clusters, vector<Cluster_center>& centers)
    {
        vector<int> cluster_rights(clusters.size());
        vector<int> cluster_lefts(clusters.size());
        for (int i = 0; i < clusters.size(); i++) {
            int tmp_right = 0;
            int tmp_left = INT_MAX;
            for (int j = 0; j < clusters[i].size(); j++) {
                if (clusters[i][j].x < tmp_left) {
                    tmp_left = clusters[i][j].x;
                }
                if (clusters[i][j].x + clusters[i][j].width > tmp_right) {
                    tmp_right = clusters[i][j].x + clusters[i][j].width;
                }
            }
            cluster_rights[i] = tmp_right;
            cluster_lefts[i] = tmp_left;
        }
        for (int i = 0; i < clusters.size(); i++) {
            for (int j = i + 1; j < clusters.size(); j++) {
                int Iou_min = max(cluster_lefts[i], cluster_lefts[j]);
                int Iou_max = min(cluster_rights[i], cluster_rights[j]);
                if (Iou_max >= Iou_min) {
                    clusters[i].insert(clusters[i].end(), clusters[j].begin(), clusters[j].end());
                    float new_center_x = (centers[i].x + centers[j].x) / 2;
                    float new_center_y = (centers[i].y + centers[j].y) / 2;
                    centers[i].x = new_center_x;
                    centers[i].y = new_center_y;
                    cluster_rights[i] = max(cluster_rights[i], cluster_rights[j]);
                    cluster_lefts[i] = min(cluster_lefts[i], cluster_lefts[j]);

                    clusters.erase(clusters.begin() + j);
                    centers.erase(centers.begin() + j);
                    cluster_lefts.erase(cluster_lefts.begin() + j);
                    cluster_rights.erase(cluster_rights.begin() + j);
                    j = i;
                }
            }
        }

        sort(clusters.begin(), clusters.end(), comp_col);
    }

    //按照上和左的位置进行排序
    static bool compare(const Bounding_box a, const Bounding_box b) {
        if (a.y < b.y)
            return true;
        else if (a.y == b.y)
            return a.x < b.x;
        else
            return false;
    }

    static bool compare_col(const Bounding_box a, const Bounding_box b){
        if (a.x < b.x)
            return true;
        else if (a.x == b.x)
            return a.y < b.y;
        else
            return false;
    }

    //行结果的排序规则
    static bool comp_row(const vector<Bounding_box>& a, const vector<Bounding_box>& b) {
        return a[0].y < b[0].y;
    }

    //列结果的排序规则
    static bool comp_col(const vector<Bounding_box>& a, const vector<Bounding_box>& b){
        return a[0].x < b[0].x;
    }

    //对一个格子里面的box按从左到右排序
    static bool comp_col_single(const Bounding_box a, const Bounding_box b) {
        return a.x < b.x;
    }

    static bool comp_row_single(const Bounding_box a, const Bounding_box b) {
        return a.y < b.y;
    }

    //将拆分下来的字符串，按照内容，封装到一个bounding box结构体对象里面
    void analysis(vector<string>& file_content, vector<Bounding_box>& bounding_boxs, vector<Big_bounding_box>& big_bounding_boxs) {
        for (int i = 0; i < file_content.size(); i++) {
            Bounding_box bounding_box;
            Big_bounding_box big_bounding_box;

            vector<string> splitedLine = split_line(file_content[i], ' ');
            string class_id = split_line(splitedLine[4], ':')[1];
            if (class_id == "102" || class_id == "203") {
                big_bounding_box.x = stoi(split_line(splitedLine[0], ':')[1]);
                big_bounding_box.y = stoi(split_line(splitedLine[1], ':')[1]);
                big_bounding_box.width = stoi(split_line(splitedLine[2], ':')[1]);
                big_bounding_box.height = stoi(split_line(splitedLine[3], ':')[1]);
                big_bounding_box.class_idx = stoi(split_line(splitedLine[4], ':')[1]);
                big_bounding_boxs.push_back(big_bounding_box);
            }
            else{
                bounding_box.x = stoi(split_line(splitedLine[0], ':')[1]);
                bounding_box.y = stoi(split_line(splitedLine[1], ':')[1]);
                bounding_box.width = stoi(split_line(splitedLine[2], ':')[1]);
                bounding_box.height = stoi(split_line(splitedLine[3], ':')[1]);
                bounding_box.class_idx = stoi(split_line(splitedLine[4], ':')[1]);
                if (splitedLine.size() == 6) {
                    bounding_box.text = split_line(splitedLine[5], ':')[1];
                }
                bounding_boxs.push_back(bounding_box);
            }
        }
        sort(bounding_boxs.begin(), bounding_boxs.end(), compare);
    }

    //实现字符串的split函数
    vector<string> split_line(string single_line, char symbol) {
        int pos = 0;
        int npos = 0;
        vector<string> result;
        while ((npos = single_line.find(symbol, pos)) != -1) {
            string tmp = single_line.substr(pos, npos - pos);
            result.push_back(tmp);
            pos = npos + 1;
        }
        result.push_back(single_line.substr(pos, single_line.length() - pos));
        return result;
    }

    //从识别结果的TXT中读取数据到内存
    void ReadDataFromTxt(string filename) {
        ifstream in(filename);
        string line;
        if (in.is_open()) {
            while(getline(in, line)) {
                file_content.push_back(line);
            }
        }
        else {
            cout << "no such file! " << endl;
        }
    }

};


int main() {
    //std::cout << "Hello, World!" << std::endl;
    FormOperator formOperator;
    formOperator.ReadDataFromTxt("../test.txt");
    formOperator.analysis(formOperator.file_content, formOperator.bounding_boxs, formOperator.big_bounding_boxs);
    clock_t start, end;
    start = clock();

//    int best_k_row = formOperator.find_best_k(formOperator.bounding_boxs, k_row_min, k_row_max, formOperator.clusters_row, formOperator.centers_row,
//                                          false);
//    int best_k_col = formOperator.find_best_k(formOperator.bounding_boxs, k_col_min, k_col_max, formOperator.clusters_col, formOperator.centers_col,
//                                          true);

        formOperator.cluster_row(formOperator.bounding_boxs, formOperator.clusters_row);
        formOperator.cluster_col(formOperator.bounding_boxs, formOperator.clusters_col);

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
    cout << "解析耗时： " << (double)(end - start) / CLOCKS_PER_SEC << endl;
    cout << "解析结果：" << endl;
    for (int i = 0; i < formOperator.group_res.size(); i++)
    {
        cout << "第" << i + 1 << "行：" << endl;
        for (int j = 0; j < formOperator.group_res[i].size(); j++)
        {

            for (int m = 0; m < formOperator.group_res[i][j].size(); m++)
            {
                cout << formOperator.group_res[i][j][m].x << " " << formOperator.group_res[i][j][m].y << " " << formOperator.group_res[i][j][m].text << " ";
            }
            cout << " | ";
        }
        cout << endl;
    }
    cout << endl;

    start = clock();
    formOperator.splice_chi_char(formOperator.group_res);
    end = clock();
    cout << "合并印刷体汉字耗时： " << (double)(end - start) / CLOCKS_PER_SEC << endl;
    cout << endl << "合并印刷体汉字结果：" << endl;
    for (int i = 0; i < formOperator.group_res.size(); i++)
    {
        cout << "第" << i + 1 << "行：" << endl;
        for (int j = 0; j < formOperator.group_res[i].size(); j++)
        {

            for (int m = 0; m < formOperator.group_res[i][j].size(); m++)
            {
                cout << formOperator.group_res[i][j][m].x << " " << formOperator.group_res[i][j][m].y << " " << formOperator.group_res[i][j][m].text << " ";
            }
            cout << " | ";
        }
        cout << endl;
    }


    return 0;

}





