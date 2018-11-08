#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <cmath>
using namespace std;
#define k 9

// 定义行、列的范围
#define k_row_min 3
#define k_row_max 20
#define k_col_min 3
#define k_col_max 20


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

    //用k-means实现聚类，来观察有几行几列
    //计算一行里面的bounding box的距离
    int dist_row(const Cluster_center a, const Bounding_box b) {
        return float(a.y - b.y) > 0 ? float(a.y - b.y) : float(a.y - b.y) * (-1.0);
    }

    //计算一列里面的bounding box的距离
    int dist_col(const Cluster_center a, const Bounding_box b) {
        return float(a.x - b.x) > 0 ? float(a.x - b.x) : float(a.x - b.x) * (-1.0);
    }

    int dist(const Cluster_center a, const Bounding_box b) {
        return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }

    //计算簇内的平方误差
    float getVar(vector<vector<Bounding_box>>& clusters, vector<Cluster_center>& centers, const int cur_k) {
        float var = 0;
        for (int i = 0; i < cur_k; i++)
        {
            vector<Bounding_box> cluster = clusters[i];
            for (int j = 0; j < cluster.size(); j++)
            {
                var += dist_row(centers[i], cluster[j]);
            }
        }
        return var;
    }

    //根据算与聚类中心的距离，分配
    int cluster_of_tuple(vector<Cluster_center>& centers, Bounding_box bounding_box, const int cur_k) {
        int dist = dist_row(centers[0], bounding_box);
        int tmp;
        int label = 0;
        for (int i = 1; i < cur_k; i++)
        {
            tmp = dist_row(centers[i], bounding_box);
            if (tmp < dist)
            {
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
        for (int i = 0; i < num; i++)
        {
            meansX += clusters[i].x;
            meansY += clusters[i].y;
        }
        center.x = meansX / num;
        center.y = meansY / num;
        return center;
    }

    float k_means(vector<Bounding_box>& bounding_boxs, const int cur_k, vector<vector<Bounding_box>>& clusters, vector<Cluster_center>& centers) {
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
            label = cluster_of_tuple(centers, bounding_boxs[i], cur_k);
            clusters[label].push_back(bounding_boxs[i]);
        }

        for (label = 0; label < cur_k; label++) {
            cout << "第" << label + 1 << "个簇" << endl;
            vector<Bounding_box> box = clusters[label];
            for (i = 0; i < box.size(); i++)
            {
                cout << box[i].x << " " << box[i].y << " " << box[i].text << endl;
            }
            cout << endl;
        }

        float oldVal = -1;
        float newVal = getVar(clusters, centers, cur_k);
        while (abs(newVal - oldVal) >= 1) {
            for (i = 0; i < cur_k; i++) {
                centers[i] = get_means(clusters[i]);
            }
            oldVal = newVal;
            newVal = getVar(clusters, centers, cur_k);

            //清空每个簇
            for (i = 0; i < cur_k; i++) {
                clusters[i].clear();
            }

            for (i = 0; i != bounding_boxs.size(); i++) {
                label = cluster_of_tuple(centers, bounding_boxs[i], cur_k);
                clusters[label].push_back(bounding_boxs[i]);
            }

            for (label = 0; label < cur_k; label++) {
                cout << "第" << label + 1 << "个簇" << endl;
                vector<Bounding_box> box = clusters[label];
                for (i = 0; i < box.size(); i++)
                {
                    cout << box[i].x << " " << box[i].y << " " << box[i].text << endl;
                }
                cout << endl;
            }

        }
        return newVal;
    }

    //找最优的K值
    int find_best_k(vector<Bounding_box>& bounding_boxs, const int k_min, const int k_max, vector<vector<Bounding_box>>& clusters, vector<Cluster_center>& centers) {
        int final_k = 0;
        int cur_k = k_min;
        float final_loss = 0.0;
        vector<vector<Bounding_box>> tmp;
        vector<Cluster_center> tmp_centers;
        while (cur_k <= k_max) {
            if (cur_k == k_min) {
                final_loss = k_means(bounding_boxs, cur_k, clusters, centers);
                final_k = cur_k;
            }
            else {
                float loss = k_means(bounding_boxs, cur_k, tmp, tmp_centers);
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

    void group_row(vector<vector<Bounding_box>>& clusters, vector<Cluster_center>& centers) {
        vector<int> cluster_bottoms(clusters.size());
        for (int i = 0; i < clusters.size(); i++) {
            int tmp_bottom = 0;
            for (int j = 0; j < clusters[i].size(); j++) {
                if (clusters[i][j].y + clusters[i][j].height > tmp_bottom) {
                    tmp_bottom = clusters[i][j].y + clusters[i][j].height;
                }
            }
            cluster_bottoms[i] = tmp_bottom;
        }

        for (int i = 0; i < clusters.size() - 1; i++)
        {
            for (int j = i + 1; j < clusters.size(); j++)
            {
                if ((centers[j].y > centers[i].y && centers[j].y < cluster_bottoms[i])
                        || (centers[i].y > centers[j].y && centers[i].y < cluster_bottoms[j]))
                {
                    clusters[i].insert(clusters[i].end(), clusters[j].begin(), clusters[j].end());
                    float new_center_x = (centers[i].x + centers[j].x) / 2;
                    float new_center_y = (centers[i].y + centers[j].y) / 2;
                    centers[i].x = new_center_x;
                    centers[i].y = new_center_y;
                    cluster_bottoms[i] = max(cluster_bottoms[i], cluster_bottoms[j]);

                    clusters.erase(clusters.begin() + j);
                    centers.erase(centers.begin() + j);
                    cluster_bottoms.erase(cluster_bottoms.begin() + j);
                }
            }
        }
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

    int best_k = formOperator.find_best_k(formOperator.bounding_boxs, k_row_min, k_row_max, formOperator.clusters_row, formOperator.centers_row);

    cout << endl << "Best K: " << best_k << endl;

    for (int label = 0; label < best_k; label++) {
        cout << "第" << label + 1 << "个簇" << endl;
        cout << "簇中心" << formOperator.centers_row[label].x << " " << formOperator.centers_row[label].y << endl;
        vector<Bounding_box> box = formOperator.clusters_row[label];
        for (int i = 0; i < box.size(); i++)
        {
            cout << box[i].x << " " << box[i].y << " " << box[i].text << endl;
        }
        cout << endl;
    }

    formOperator.group_row(formOperator.clusters_row, formOperator.centers_row);
    cout << endl << "After group row" << endl << "K: " << formOperator.centers_row.size() << endl;
    for (int label = 0; label < formOperator.clusters_row.size(); label++) {
        cout << "第" << label + 1 << "个簇" << endl;
        cout << "簇中心" << formOperator.centers_row[label].x << " " << formOperator.centers_row[label].y << endl;
        vector<Bounding_box> box = formOperator.clusters_row[label];
        for (int i = 0; i < box.size(); i++)
        {
            cout << box[i].x << " " << box[i].y << " " << box[i].text << endl;
        }
        cout << endl;
    }

//    for (int i = 0 ; i < formOperator.bounding_boxs.size(); i++) {
//        cout << formOperator.bounding_boxs[i].text << endl;
//    }

    return 0;
}