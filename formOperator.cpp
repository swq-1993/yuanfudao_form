//
// Created by sunwanqi on 2018/11/9.
//
#include "header.h"


//判断第一行是否存在很长的中文字符串，这种长的中文字符串是不会出现在表格里面的，所以判定为表格外的干扰
bool FormOperator::exist_long_chi(vector<vector<vector<Bbox>>>& group_res){
    bool res = false;
    if (group_res.empty()){
        return false;
    }
    for (int i = 0; i < group_res[0].size(); i++){
        if (!group_res[0].empty()){
            for (int j = 0; j < group_res[0][i].size(); j++){
                if (group_res[0][i][j].text.length() >= chi_length && group_res[0][i][j].class_idx == 100)
                {
                    cout << "判断要删除的字符串:" << group_res[0][i][j].text << " length:" << group_res[0][i][j].text.size() << endl;
                    return true;
                }
            }
        }

    }
    return res;
}

//去掉第一行的干扰信息
void FormOperator::filter_long_chi_str(vector<vector<vector<Bbox>>>& group_res, vector<vector<Bbox>>& clusters_row, vector<vector<Bbox>>& clusters_col){
    //删除第一行识别结果
    clusters_row.erase(clusters_row.begin());

    vector<Bbox> tmp;
    for (int i = 0; i < clusters_row.size(); i++){
        for (int j = 0; j < clusters_row[i].size(); j++){
            tmp.push_back(clusters_row[i][j]);
        }
    }
    clusters_col.clear();
    group_res.clear();

    cluster_col(tmp, clusters_col);
    group_clusters_res(group_res, clusters_row, clusters_col);

}

//将要运算的数字运算验证
void FormOperator::compute(vector<vector<string>> &nums, int rule){

}

bool string_blurry_match(string a, string b){
    if (a.length() == b.length()){
        if (a == "金额"){
            return a == b || b == "总价";
        }
        else if (a == "总价"){
            return a == b || b == "金额";
        }
        return a == b;
    }
    else if (a.length() < b.length()){
        for (int i = 0; i < a.length(); i++){
            if (a[i] != b[i]){
                return false;
            }
        }
        return true;
    }
    else{
        for (int i = 0; i < b.length(); i++){
            if (a[i] != b[i]){
                return false;
            }
        }
        return true;
    }
}

//根据拿下来的res，进行匹配运算规则
int FormOperator::match_formula(map< pair<pair <string,string>, string>, int >& stem_map, vector<vector<vector<Bbox>>>& group_res)
{
    int res = -1;
    bool find = false;
    int row = -1;
    int col = -1;
    map< pair<pair <string,string>, string>, int > :: iterator iter;
    vector<int> rows;
    vector<int> cols;
    for (int i = 0; i < group_res.size(); i++) {
        for (int j = 0; j < group_res[i].size(); j++) {
            if (group_res[i][j].size() == 0 || group_res[i][j][0].class_idx != 100){
                continue;
            }
            iter = stem_map.begin();
            while (iter != stem_map.end()){
                if (string_blurry_match(group_res[i][j][0].text, iter->first.first.first)){
                    row = i;
                    col = j;
                    find = true;
                    break;
                }
                iter++;
            }
            if (find){
                break;
            }

        }
        if (find){
            break;
        }
    }

    vector<vector<string>> nums;
    if (find){
        //题干在一列，且保证之后还有两行，找到剩下两行的位置
//        if (group_res.size() > (row + 1) && iter->first.first.second == group_res[row + 1][col][0].text){
//            if (group_res.size() > (row + 2) && iter->first.second == group_res[row + 2][col][0].text){

//        vector<int> rows{row};
        rows.push_back(row);
        if (group_res.size() > (row + 2)) {
            for (int i = row + 1; i < group_res.size() - 1; i++) {
                if (!group_res[i][col].empty() &&
                    string_blurry_match(group_res[i][col][0].text, iter->first.first.second)) {
                    rows.push_back(i);
                    for (int j = i + 1; j < group_res.size(); j++) {
                        if (!group_res[j][col].empty() &&
                            string_blurry_match(group_res[j][col][0].text, iter->first.second)) {
                            rows.push_back(j);
                            break;
                        }
                    }
                }
                if (rows.size() == 3) {
                    res = iter->second;
                    break;
                }
            }
            if (rows.size() == 3) {
                vector<string> tmp;
                for (int i = col + 1; i < group_res[0].size(); i++) {
                    tmp.clear();
                    for (int j = 0; j < 3; j++) {
                        if (!group_res[rows[j]][i].empty() &&
                            (group_res[rows[j]][i][0].class_idx == 104 || group_res[rows[j]][i][0].class_idx == 101)) {
//                            cout << "题目坐标： "<< rows[j] << " " << i << endl;
                            if (group_res[rows[j]][i][0].text.empty()) {
                                tmp.push_back("空");
                            } else {
                                tmp.push_back(group_res[rows[j]][i][0].text);
                            }
                        } else {
                            break;
                        }
                    }
                    if (tmp.size() == 3) {
                        nums.push_back(tmp);
                    }
                }
            }
        }
        if (rows.size() != 3 && group_res[0].size() > (col + 2)){
//            vector<int> cols{col};
            cols.push_back(col);
            for (int i = col + 1; i < group_res[0].size() - 1; i++){
//                cout << "pipei: " << group_res[row][i][0].text << " " << iter->first.first.second << endl;
                if (!group_res[row][i].empty() && string_blurry_match(group_res[row][i][0].text, iter->first.first.second)){
                    cols.push_back(i);
                    for (int j = i + 1; j < group_res[0].size(); j++){
//                        cout << "pipei2: " << group_res[row][j][0].text << " " << iter->first.second << endl;
                        if (!group_res[row][j].empty() && string_blurry_match(group_res[row][j][0].text, iter->first.second)){
                            cols.push_back(j);
                            break;
                        }
                    }
                }
                if (cols.size() == 3){
                    res = iter->second;
                    break;
                }
            }

            if (cols.size() == 3){

                vector<string> tmp;
                for (int i = row + 1; i < group_res.size(); i++){
                    tmp.clear();
                    for (int j = 0; j < 3; j++){
                        if (!group_res[i][cols[j]].empty() && (group_res[i][cols[j]][0].class_idx == 104 || group_res[i][cols[j]][0].class_idx == 101)){
//                            cout << "题目坐标： "<< i << " " << cols[j] << endl;
                            if (group_res[i][cols[j]][0].text.empty()){
                                tmp.push_back("空");
                            }
                            else{
                                tmp.push_back(group_res[i][cols[j]][0].text);
                            }
                        }
                        else{
                            break;
                        }
                    }
                    if (tmp.size() == 3){
                        nums.push_back(tmp);
                    }
                }
            }
        }

        /*if (group_res.size() > (row + 1) && string_blurry_match(iter->first.first.second, group_res[row + 1][col][0].text)){
            if (group_res.size() > (row + 2) && string_blurry_match(iter->first.second, group_res[row + 2][col][0].text)){
                vector<vector<string>> nums;
                vector<string> tmp;
                for (int i = col + 1; i < group_res[0].size(); i++){
                    tmp.clear();
                    if (group_res[row][i][0].class_idx == 104){
                        for (int j = row; j < row + 3; j++){
                            tmp.push_back(group_res[j][i][0].text);
                        }
                        nums.push_back(tmp);
                    }
                }
                res = iter->second;
            }
        }*/

        //题干在一行，且保证之后还有两列，找到剩下两列的位置
        /*else if (group_res[0].size() > (col + 1) && iter->first.first.second == group_res[row][col + 1][0].text){
            if (group_res[0].size() > (col + 2) && iter->first.second == group_res[row][col + 1][0].text){
                vector<vector<string>> nums;
                vector<string> tmp;
                for (int i = row + 1; i < group_res.size(); i++){
                    tmp.clear();
                    if (group_res[i][col][0].class_idx == 104){
                        for (int j = col; j < col + 3; j++){
                            tmp.push_back(group_res[i][j][0].text);
                        }
                        nums.push_back(tmp);
                    }
                }

                res = iter->second;
            }
        }*/
    }
    if (res != -1){
        for (int i = 0; i < nums.size(); i++){
            for (int j = 0; j < nums[i].size(); j++){
                cout << nums[i][j] << " ";
            }
            cout << endl;
        }
    }
    return res;
}

//将所给的题型初始化到指定数据结构
void FormOperator::init(map< pair<pair <string,string>, string>, int >& stem_map) {
//    map< pair<pair <string,string>, string>, int > mymap ;// declare map of pairs
//    mymap[ make_pair(make_pair("单价","数量"), "总价") ] = 2 ; // map[key] =value
//    mymap.insert(make_pair(make_pair(make_pair("单价","数量"), "路程"), 3));
//    map< pair<pair <string,string>, string>, int >::iterator iter = mymap.begin();
//    cout << iter->first.first.first << " " << iter->first.first.second << " " << iter->first.second << " " << iter->second << endl;
//    iter++;
//    cout << iter->first.first.first << " " << iter->first.first.second << " " << iter->first.second << " " << iter->second << endl;

    for (int i = 0; i < stem.size(); i++){
        //stem_map[make_pair(make_pair(stem[i][0], stem[i][1]), stem[i][2])] = stoi(stem[i][3]);
                stem_map.insert(make_pair(make_pair(make_pair(stem[i][0], stem[i][1]), stem[i][2]), stoi(stem[i][3])));
    }

//    map< pair<pair <string,string>, string>, int > :: iterator iter = stem_map.begin();
//    while (iter != stem_map.end())
//    {
//        cout << iter->first.first.first << " " << iter->first.first.second << " " << iter->first.second << " " << iter->second << endl;
//        iter++;
//    }
}

//计算一行里面的bounding box的距离
double FormOperator::dist_row(const Cluster_center a, const Bbox b) {
    return double(a.y - b.y) > 0 ? double(a.y - b.y) : double(a.y - b.y) * (-1.0);
}

//计算一列里面的bounding box的距离
double FormOperator::dist_col(const Cluster_center a, const Bbox b) {
    return double(a.x - b.x) > 0 ? double(a.x - b.x) : double(a.x - b.x) * (-1.0);
}

//计算欧式距离
double FormOperator::dist(const Cluster_center a, const Bbox b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

//查找两个vector的交集
vector<Bbox> FormOperator::intersection(vector<Bbox>& A, vector<Bbox>& B)
{
    vector<Bbox> res;
    map<pair<int, int>, Bbox>part_a, part_b;
    for(auto a : A) {
        part_a.insert(make_pair(make_pair(a.x, a.y), a));
    }
    for (auto b : B) {
        part_b.insert(make_pair(make_pair(b.x, b.y), b));
    }

    map<pair<int, int>, Bbox> :: iterator iter = part_a.begin();
    while (iter != part_a.end()) {
        /*if (part_b.find(iter->first) != part_b.end()) {
            res.push_back(iter->second);
        }*/
        if (part_b.find(make_pair(iter->first.first, iter->first.second)) != part_b.end()){
            res.push_back(iter->second);
        }
        iter++;
    }

    return res;
}
//合并行、列的结果
void FormOperator::group_clusters_res(vector<vector<vector<Bbox>>>& group_res, vector<vector<Bbox>>& clusters_row, vector<vector<Bbox>>& clusters_col) {
    int rows = clusters_row.size();
    int cols = clusters_col.size();
    vector<Bbox> tmp;
    vector<vector<Bbox>> tmp_row;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            //求一行与一列的bbox的交集
//                set_intersection(clusters_row[i].begin(), clusters_row[i].end(), clusters_col[j].begin(), clusters_col[j].end(), back_inserter(tmp));
            tmp = intersection(clusters_row[i], clusters_col[j]);
            sort(tmp.begin(), tmp.end(), comp_col_single);
           /* cout << "合并结果： " << i << " " << j << endl;
            for (int index = 0; index < tmp.size(); index++){
                cout << tmp[index].text << " ";
            }
            cout << endl;*/
            tmp_row.push_back(tmp);
            tmp.clear();
        }
        group_res.push_back(tmp_row);
        tmp_row.clear();
    }

}

//拼接在一个格子的印刷体汉字
void FormOperator::splice_chi_char(vector<vector<vector<Bbox>>>& group_res)
{
    for (int i = 0; i < group_res.size(); i++)
    {
        for (int j = 0; j < group_res[i].size(); j++)
        {
            //一个格子里面的所有bbox
            for (int m = 0; m < group_res[i][j].size(); m++)
            {
                //如果发现是印刷体汉字
                if (group_res[i][j][m].class_idx == 100)
                {
                    for (int n = m + 1; n < group_res[i][j].size(); n++)
                    {
//                        如果第二个连续的字符也是印刷体汉字，而且是在同一行
                        int iou_min = max(group_res[i][j][m].y, group_res[i][j][n].y);
                        int iou_max = min(group_res[i][j][m].y + group_res[i][j][m].height, \
                                            group_res[i][j][n].y + group_res[i][j][n].height);
                        if (group_res[i][j][n].class_idx == 100 && (iou_max - iou_min) > 0)
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
void FormOperator::cluster_row(vector<Bbox>& bboxs, vector<vector<Bbox>>& clusters_row)
{
    vector<Bbox> copy = bboxs;
    sort(bboxs.begin(), bboxs.end(), compare);
    vector<bool> flag(copy.size(), true);
    vector<Bbox> tmp;
    for (int i = 0; i < copy.size(); i++)
    {
        if (!flag[i]){
            continue;
        }
        int tmp_top = copy[i].y;
        int tmp_bottom = copy[i].y + copy[i].height;
        tmp.push_back(copy[i]);
        flag[i] = false;
        for (int j = 0; j < copy.size(); j++)
        {
            if (!flag[j]){
                continue;
            }
            int Iou_min = max(tmp_top, copy[j].y);
            int Iou_max = min(tmp_bottom, copy[j].y + copy[j].height);
            /*cout << "compare: " << copy[i].y << " " << copy[i].text << " " << copy[j].y << " " << copy[j].text << endl;
            cout << tmp_top << " " << tmp_bottom << endl;
            cout << (double)(Iou_max - Iou_min);
            cout << " " << (double)(0.5 * copy[j].height) << endl;*/
//            if (Iou_max >= Iou_min)
            if ((double)(Iou_max - Iou_min) >= (double)(0.25 * copy[j].height) &&
                copy[j].y < tmp_top + (tmp_bottom - tmp_top) * 0.75)
            {

//                cout << "合并" << endl;
                tmp.push_back(copy[j]);
                tmp_top = min(tmp_top, copy[j].y);
                tmp_bottom = max(tmp_bottom, copy[j].y + copy[j].height);

//                copy.erase(copy.begin() + j);
                flag[j] = false;
            }
        }
        sort(tmp.begin(), tmp.end(), [](const Bbox &a, const Bbox &b){return a.x < b.x;});
        clusters_row.push_back(tmp);
        tmp.clear();
    }
    sort(clusters_row.begin(), clusters_row.end(), comp_row);
}

void FormOperator::cluster_col2(vector<vector<Bbox>>& clusters_row, vector<vector<vector<Bbox>>> group_res){

    vector<Bbox> tmp;
    vector<vector<bool>> flag;
    for (int i = 0; i < clusters_row.size(); i++){
        for (int j = 0; j < clusters_row[i].size(); j++){
            for (int m = j + 1; m < clusters_row[i].size(); m++){

                //合并相邻
                if ((clusters_row[i][m].x - clusters_row[i][j].x) < clusters_row[i][m].width &&
                    clusters_row[i][m].class_idx == 100 &&
                        clusters_row[i][j].class_idx == 100){

                }
            }
        }
    }

}

//用IOU的思想对 列 进行聚类
void FormOperator::cluster_col(vector<Bbox>& bboxs, vector<vector<Bbox>>& clusters_col)
{
    vector<Bbox> copy = bboxs;
    sort(copy.begin(), copy.end(), compare_col);
    vector<bool> flag(copy.size(), true);
    vector<Bbox> tmp;
    for (int i = 0; i < copy.size(); i++)
    {
        if (!flag[i]){
            continue;
        }
        int tmp_left = copy[i].x;
        int tmp_right = copy[i].x + copy[i].width;
        tmp.push_back(copy[i]);
        flag[i] = false;
        for (int j = 0; j < copy.size(); j++)
        {
            if (!flag[j]){
                continue;
            }
            int Iou_min = max(tmp_left, copy[j].x);
            int Iou_max = min(tmp_right, copy[j].x + copy[j].width);

           /* cout << "compare: " << copy[i].y << " " << copy[i].text << " " << copy[j].y << " " << copy[j].text << endl;
            cout << tmp_left << " " << tmp_right << endl;
            cout << (double)(Iou_max - Iou_min);
//            cout << " " << (double)(0.5 * copy[j].height) << endl;*/
            if (Iou_max >= Iou_min && copy[j].class_idx == 100){
                tmp.push_back(copy[j]);
                tmp_left = min(tmp_left, copy[j].x);
                tmp_right = max(tmp_right, copy[j].x + copy[j].width);

                flag[j] = false;
            }
            else if ((double)(Iou_max - Iou_min) >= (double)(0.25 * copy[j].width) &&
                    copy[j].x <= tmp_left + (tmp_right - tmp_left) * 0.75){
                tmp.push_back(copy[j]);
                tmp_left = min(tmp_left, copy[j].x);
                tmp_right = max(tmp_right, copy[j].x + copy[j].width);

                flag[j] = false;
            }
//            if ((double)(Iou_max - Iou_min) >= (double)(0.5 * copy[j].width) &&
//                    copy[j].x <= tmp_left + (tmp_right - tmp_left) * 0.5)
        /*if ((double)(Iou_max - Iou_min) >= (double)(0.5 * copy[j].width))
            {
                if ((tmp_left <= copy[j].x && copy[j].x <= (tmp_left + tmp_right) * 0.5) ||
                        (tmp_left > copy[j].x && copy[j].x + copy[j].width <= (tmp_left + tmp_right) * 0.5)){
                    tmp.push_back(copy[j]);
                    tmp_left = min(tmp_left, copy[j].x);
                    tmp_right = max(tmp_right, copy[j].x + copy[j].width);

                    copy.erase(copy.begin() + j);
                    j = i;
                }
            }*/
        }
        sort(tmp.begin(), tmp.end(), [](const Bbox &a, const Bbox &b){return a.y < b.y;});
        clusters_col.push_back(tmp);
        tmp.clear();
    }
    sort(clusters_col.begin(), clusters_col.end(), comp_col);
}

//计算簇内的平方误差
float FormOperator::getVar(vector<vector<Bbox>>& clusters, vector<Cluster_center>& centers, const int cur_k, bool row_or_col) {
    float var = 0;
    for (int i = 0; i < cur_k; i++)
    {
        vector<Bbox> cluster = clusters[i];
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
int FormOperator::cluster_of_tuple(vector<Cluster_center>& centers, Bbox bbox, const int cur_k, bool row_or_col) {
    int dist = dist_row(centers[0], bbox);
    if (row_or_col) {
        dist = dist_col(centers[0], bbox);
    }
    int tmp;
    int label = 0;
    for (int i = 1; i < cur_k; i++) {
        tmp = dist_row(centers[i], bbox);
        if (row_or_col) {
            tmp = dist_col(centers[i], bbox);
        }
        if (tmp < dist) {
            dist = tmp;
            label = i;
        }
    }
    return label;
}

//计算簇的中心
Cluster_center FormOperator::get_means(vector<Bbox>& clusters) {
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
float FormOperator::k_means(vector<Bbox>& bboxs, const int cur_k, vector<vector<Bbox>>& clusters, vector<Cluster_center>& centers, bool row_or_col) {
    //vector<vector<Bbox>> clusters(k);
    clusters.resize(cur_k);
    centers.resize(cur_k);
    int i = 0;
    for (i = 0; i < cur_k; i++) {
        centers[i].x = float(bboxs[i * bboxs.size() / cur_k].x);
        centers[i].y = float(bboxs[i * bboxs.size() / cur_k].y);
    }
    int label = 0;
    for (i = 0; i != bboxs.size(); i++) {
        label = cluster_of_tuple(centers, bboxs[i], cur_k, row_or_col);
        clusters[label].push_back(bboxs[i]);
    }

    for (label = 0; label < cur_k; label++) {
        cout << "第" << label + 1 << "个簇" << endl;
        vector<Bbox> box = clusters[label];
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

        for (i = 0; i != bboxs.size(); i++) {
            label = cluster_of_tuple(centers, bboxs[i], cur_k, row_or_col);
            clusters[label].push_back(bboxs[i]);
        }

        for (label = 0; label < cur_k; label++) {
            cout << "第" << label + 1 << "个簇" << endl;
            vector<Bbox> box = clusters[label];
            for (i = 0; i < box.size(); i++) {
                cout << box[i].x << " " << box[i].y << " " << box[i].text << endl;
            }
            cout << endl;
        }
    }
    return newVal;
}

//找最优的K值
int FormOperator::find_best_k(vector<Bbox>& bboxs, const int k_min, const int k_max, vector<vector<Bbox>>& clusters, vector<Cluster_center>& centers, bool row_or_col) {
    int final_k = 0;
    int cur_k = k_min;
    float final_loss = 0.0;
    vector<vector<Bbox>> tmp;
    vector<Cluster_center> tmp_centers;
    while (cur_k <= k_max) {
        if (cur_k == k_min) {
            final_loss = k_means(bboxs, cur_k, clusters, centers, row_or_col);
            final_k = cur_k;
        }
        else {
            float loss = k_means(bboxs, cur_k, tmp, tmp_centers, row_or_col);
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
void FormOperator::group_row(vector<vector<Bbox>>& clusters, vector<Cluster_center>& centers) {
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
void FormOperator::group_col(vector<vector<Bbox>>& clusters, vector<Cluster_center>& centers)
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
bool FormOperator::compare(const Bbox a, const Bbox b) {
    if (a.y < b.y)
        return a.x < b.x;
    else if (a.y == b.y)
        return a.x < b.x;
    else
        return false;
}

bool FormOperator::compare_col(const Bbox a, const Bbox b){
    if (a.x < b.x)
        return true;
    else if (a.x == b.x)
        return a.y < b.y;
    else
        return false;
}

//行结果的排序规则
bool FormOperator::comp_row(const vector<Bbox>& a, const vector<Bbox>& b) {
    return a[0].y < b[0].y;
}

//列结果的排序规则
bool FormOperator::comp_col(const vector<Bbox>& a, const vector<Bbox>& b){
    return a[0].x < b[0].x;
}

//对一个格子里面的box按从左到右排序
bool FormOperator::comp_col_single(const Bbox a, const Bbox b) {
//    if a.y < b.y
    int iou_min = max(a.y, b.y);
    int iou_max = min(a.y + a.height, b.y + b.height);
    if (iou_max > iou_min){
        return a.x < b.x;
    }
    else{
        return a.y < b.y;
    }
//    return a.x < b.x;
}

bool FormOperator::comp_row_single(const Bbox a, const Bbox b) {
//    int iou_min = max(a.y, b.y);
//    int iou_max = min(a.y + a.height, b.y + b.height);
//    if (iou_max > iou_min){
//        return a.x < b.x;
//    }
//    else{
//        return a.y < b.y;
//    }
    return a.y < b.y;
}

//将拆分下来的字符串，按照内容，封装到一个bounding box结构体对象里面
void FormOperator::analysis(vector<string>& file_content, vector<Bbox>& bboxs, vector<Big_bbox>& big_bboxs) {
    for (int i = 0; i < file_content.size(); i++) {
        Bbox bbox;
        Big_bbox big_bbox;
        vector<string> splitedLine = split_line(file_content[i], ' ');
        if (splitedLine.size() >= 5){
            string class_id = split_line(splitedLine[4], ':')[1];
            if (class_id == "203" || class_id == "200" || class_id == "201" || class_id == "202") {
                big_bbox.x = stoi(split_line(splitedLine[0], ':')[1]);
                big_bbox.y = stoi(split_line(splitedLine[1], ':')[1]);
                big_bbox.width = stoi(split_line(splitedLine[2], ':')[1]);
                big_bbox.height = stoi(split_line(splitedLine[3], ':')[1]);
                big_bbox.class_idx = stoi(split_line(splitedLine[4], ':')[1]);
                big_bboxs.push_back(big_bbox);
            }
            else if(class_id == "100" || class_id == "101" || class_id == "103" || class_id == "104"){
                bbox.x = stoi(split_line(splitedLine[0], ':')[1]);
                bbox.y = stoi(split_line(splitedLine[1], ':')[1]);
                bbox.width = stoi(split_line(splitedLine[2], ':')[1]);
                bbox.height = stoi(split_line(splitedLine[3], ':')[1]);
                bbox.class_idx = stoi(split_line(splitedLine[4], ':')[1]);
                if (splitedLine.size() == 6) {
                    bbox.text = split_line(splitedLine[5], ':')[1];
                }
                bboxs.push_back(bbox);
            }

            /*if (class_id == "100" || class_id == "101" || class_id == "103" || class_id == "104") {
                bbox.x = stoi(split_line(splitedLine[0], ':')[1]);
                bbox.y = stoi(split_line(splitedLine[1], ':')[1]);
                bbox.width = stoi(split_line(splitedLine[2], ':')[1]);
                bbox.height = stoi(split_line(splitedLine[3], ':')[1]);
                bbox.class_idx = stoi(split_line(splitedLine[4], ':')[1]);
                if (splitedLine.size() == 6) {
                    bbox.text = split_line(splitedLine[5], ':')[1];
                }
                bboxs.push_back(bbox);
            }
            else{
                big_bbox.x = stoi(split_line(splitedLine[0], ':')[1]);
                big_bbox.y = stoi(split_line(splitedLine[1], ':')[1]);
                big_bbox.width = stoi(split_line(splitedLine[2], ':')[1]);
                big_bbox.height = stoi(split_line(splitedLine[3], ':')[1]);
                big_bbox.class_idx = stoi(split_line(splitedLine[4], ':')[1]);
                big_bboxs.push_back(big_bbox);
            }*/
        }

    }
    sort(bboxs.begin(), bboxs.end(), compare);
    sort(big_bboxs.begin(), big_bboxs.end(), [](const Big_bbox &a, const Big_bbox &b){return a.y < b.y;});
}

//过滤掉大框外面的小框
void FormOperator::filter(vector<Bbox>& bboxs, Big_bbox big_bbox){
    vector<Bbox> tmp;
    for (int i = 0; i < bboxs.size(); i++){
        int Iou_min_row = max(bboxs[i].x, big_bbox.x);
        int Iou_max_row = min(bboxs[i].x + bboxs[i].width, big_bbox.x + big_bbox.width);

        int Iou_min_col = max(bboxs[i].y, big_bbox.y);
        int Iou_max_col = min(bboxs[i].y + bboxs[i].height, big_bbox.y + big_bbox.height);

//        if (Iou_max_row >= Iou_min_row && Iou_max_col >= Iou_min_col){
        //增加过滤条件
        if ((Iou_max_row - Iou_min_row >= bboxs[i].width * 0.75) && (Iou_max_col - Iou_min_col >= bboxs[i].height * 0.75)){
            tmp.push_back(bboxs[i]);
        }
    }
    bboxs.clear();
    bboxs = tmp;
//    sort(erase_index.begin(), erase_index.end(), greater<int>());
//    for (int i = 0; i < erase_index.size(); i++){
//        bboxs.erase(bboxs.begin() + i);
//    }
}

//实现字符串的split函数
vector<string> FormOperator::split_line(string single_line, char symbol) {
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
void FormOperator::ReadDataFromTxt(string filename) {
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
