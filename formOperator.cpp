//
// Created by sunwanqi on 2018/11/9.
//
#include "header.h"

//获取非四则运算匹配之后的结果
void FormOperator::get_normal_res(vector<vector<vector<Bbox>>>& group_res, vector<vector<string>>& nums, vector<int>& rows, vector<int>& cols){
    vector<string> tmp;
//    cout << "coordinate: " << rows.size() << " " << cols.size() << endl;
    if (cols.size() == 1){
        int cur_col = cols[0];
        for (int col = cur_col + 1; col < group_res[0].size(); col++){
            tmp.clear();
            for (int i = 0; i < rows.size(); i++){
                if (!group_res[rows[i]][col].empty() &&
                        (group_res[rows[i]][col][0].class_idx == 104 || group_res[rows[i]][col][0].class_idx == 101)){
                    if (group_res[rows[i]][col][0].text.empty()){
                        tmp.push_back("空");
                    }
                    else {
                        tmp.push_back(group_res[rows[i]][col][0].text);
                    }
                }
                else {
                    tmp.push_back(" ");
                }
            }
            nums.push_back(tmp);

        }
    }
    else if (rows.size() == 1){
        int cur_row = rows[0];
        for (int row = cur_row + 1; row < group_res.size(); row++){
            tmp.clear();

            for (int i = 0; i < cols.size(); i++){
                if (!group_res[row][cols[i]].empty() &&
                        (group_res[row][cols[i]][0].class_idx == 104 || group_res[row][cols[i]][0].class_idx == 101)){
                    if (group_res[row][cols[i]][0].text.empty()){
                        tmp.push_back("空");
                    }
                    else {
                        tmp.push_back(group_res[row][cols[i]][0].text);
                    }
                }
                else {
                    tmp.push_back(" ");
                }

            }
            nums.push_back(tmp);
        }
    }

}

//非四则运算的匹配
int FormOperator::match_normal_formula(vector<vector<vector<Bbox>>>& group_res, vector<int>& rows, vector<int>& cols){
    bool find_row = false;
    bool find_col = false;
    int row = -1;
    int col = -1;

    for (int i = 0; i < group_res.size(); i++){
        for (int j = 0; j < group_res[i].size(); j++){
            if (group_res[i][j].empty() || group_res[i][j][0].class_idx != 100){
                continue;
            }
            rows.clear();
            cols.clear();
            int start = 0;
            while (start < normal_stem.size()){
//                cout << "find: " << group_res[i][j][0].text << " " << normal_stem[start][0] << endl;
                if (string_blurry_match(normal_stem[start][0], group_res[i][j][0].text) && group_res[i][j][0].text != "长方形"){
                    row = i;
                    col = j;

                    //题干在一列，开始后面的匹配
                    rows.push_back(row);
//                    cout << "enter find_row" << endl;
                    for (int index_stem = 1; index_stem < normal_stem[start].size() - 1; index_stem++){

                        for (int index_row = 0; index_row < group_res.size(); index_row++){
                            if (index_row == row){
                                continue;
                            }
//                            cout << "pipei2: " << normal_stem[start][index_stem] << endl;
                            if (!group_res[index_row][col].empty() &&
                                    string_blurry_match(group_res[index_row][col][0].text, normal_stem[start][index_stem])){
//                                cout << "find2: " << group_res[index_row][col][0].text << " " << normal_stem[start][index_stem] << endl;
                                rows.push_back(index_row);
                                break;
                            }

                        }
                    }
                    //刚好匹配到
                    if (rows.size() == normal_stem[start].size() - 1){
                        find_row = true;
                        cols.push_back(col);
                        return start;
                    }
                    else {
                        rows.clear();
                    }

                    //题干在一行，开始后面的匹配
                    if (!find_row){
//                        cout << "enter find_col" << endl;
                        cols.push_back(col);

                        for (int index_stem = 1; index_stem < normal_stem[start].size() - 1; index_stem++){

                            for (int index_col = 0; index_col < group_res[row].size(); index_col++){
                                if (index_col == col){
                                    continue;
                                }
//                                cout << "pipei_col: " << normal_stem[start][index_stem] << endl;
                                if (!group_res[row][index_col].empty() &&
                                        string_blurry_match(group_res[row][index_col][0].text, normal_stem[start][index_stem])){
//                                    cout << "find_col2: " << group_res[row][index_col][0].text << " " << normal_stem[start][index_stem] << endl;
                                    cols.push_back(index_col);
                                    break;
                                }

                            }
                        }
                        //刚刚匹配到
                        if (cols.size() == normal_stem[start].size() - 1){
                            cout << "pipei dao" << endl;
                            find_col = true;
                            rows.push_back(row);
//                            break;
                            return start;
                        }
                        else {
                            cols.clear();
                        }
                    }

                }

                start++;
            }
        }
    }

    return -1;
}

//单独处理被除数、除数、商、余数这种情况
bool FormOperator::is_has_remainder(vector<vector<vector<Bbox>>>& group_res, vector<int>& row_index, vector<int>& col_index){
    int row = -1;
    int col = -1;
    bool find = false;
    bool find_row = false;
    bool find_col = false;
    for (int i = 0; i < group_res.size(); i++){
        for (int j = 0; j < group_res[i].size(); j++){
            if (!group_res[i][j].empty() && group_res[i][j][0].class_idx == 100){
                if (group_res[i][j][0].text == "被除数"){
                    row_index.push_back(i);
                    col_index.push_back(j);
                    row = i;
                    col = j;
                    find = true;
                    break;
                }
            }
        }
        if (find)
        {
            break;
        }
    }

    if (find){
        //按列寻找
        if (group_res.size() > row + 3){
            for (int i = row + 1; i < group_res.size(); i++){
                if (!group_res[i][col].empty() && group_res[i][col][0].text == "除数"){
                    row_index.push_back(i);
                    for (int j = i + 1; j < group_res.size(); j++){
                        if (!group_res[j][col].empty() && group_res[j][col][0].text == "商"){
                            row_index.push_back(j);
                            for (int m = j + 1; m < group_res.size(); m++){
                                if (!group_res[m][col].empty() && group_res[m][col][0].text == "余数"){
                                    row_index.push_back(m);
                                    find_row = true;
                                    return true;
//                                    break;
                                }
                            }
                        }
//                        if (find_row){
//                            break;
//                        }
                    }
                }
//                if (find_row){
//                    break;
//                }
            }
        }
        //按行寻找
        if (!find_row && group_res[row].size() > col + 3){
            for (int i = col + 1; i < group_res[row].size(); i++){
                if (!group_res[row][i].empty() && group_res[row][i][0].text == "除数"){
                    col_index.push_back(i);
                    for (int j = i + 1; j < group_res[row].size(); j++){
                        if (!group_res[row][j].empty() && group_res[row][j][0].text == "商"){
                            col_index.push_back(j);
                            for (int m = j + 1; m < group_res[row].size(); m++){
                                if (!group_res[row][m].empty() && group_res[row][m][0].text == "余数"){
                                    col_index.push_back(m);
                                    find_col = true;
                                    return true;
//                                    break;
                                }
                            }
//                            if (find_col){
//                                break;
//                            }
                        }
                    }
//                    if (find_col){
//                        break;
//                    }
                }
            }
        }
    }
    return find_row || find_col;
}

//输出中间行的特别长的中文干扰
vector<int> FormOperator::exist_long_chi_mid(vector<vector<vector<Bbox>>>& group_res){
    vector<int> coord;
    if (group_res.empty()){
        return coord;
    }
    for (int i = 0; i < group_res.size(); i++){
        for (int j = 0; j < group_res[i].size(); j++){
            for (int m = 0; m < group_res[i][j].size(); m++){
                if (group_res[i][j][m].text.length() > chi_length_mid &&
                        group_res[i][j][m].class_idx == 100){
                    cout << "判断要删除的字符串:  " << group_res[i][j][m].text << " length: " << group_res[i][j][m].text.length() << endl;
                    coord.push_back(i);
                    coord.push_back(j);
                    coord.push_back(m);
                    return coord;
                }
            }
        }
    }
    return coord;
}


//判断第一行是否存在很长的中文字符串，这种长的中文字符串是不会出现在表格里面的，所以判定为表格外的干扰
//flag 为true时，判断第一行是否有干扰，为false时，判断最后一行是否有干扰
vector<vector<int>> FormOperator::exist_long_chi(vector<vector<vector<Bbox>>>& group_res){
    vector<int> tmp;
    vector<vector<int>> coord;
    if (group_res.empty()){
        return coord;
    }
    for (int i = 0; i < group_res[0].size(); i++){
        if (!group_res[0].empty()){
            for (int j = 0; j < group_res[0][i].size(); j++){
                if (group_res[0][i][j].text.length() > chi_length && group_res[0][i][j].class_idx == 100)
                {
                    cout << "判断要删除的字符串:  " << group_res[0][i][j].text << " length:" << group_res[0][i][j].text.size() << endl;
                    tmp.push_back(0);
                    tmp.push_back(i);
                    tmp.push_back(j);
                    coord.push_back(tmp);
                    tmp.clear();
                }
            }
        }

    }

    return coord;
}

//输出干扰的bbox
void FormOperator::filter_useless_bbox(vector<vector<vector<Bbox>>>& group_res, vector<Bbox>& bboxs, int row, int col, int num){
    vector<Bbox> tmp;
    int x = group_res[row][col][num].x;
    int y = group_res[row][col][num].y;
    int width = group_res[row][col][num].width;
    int height = group_res[row][col][num].height;
    for (int i = 0; i < bboxs.size(); i++){
        int Iou_min_row = max(bboxs[i].x, x);
        int Iou_max_row = min(bboxs[i].x + bboxs[i].width, x + width);
        int Iou_min_col = max(bboxs[i].y, y);
        int Iou_max_col = min(bboxs[i].y + bboxs[i].height, y + height);
        if (Iou_max_row > Iou_min_row && Iou_max_col > Iou_min_col){
            continue;
        }
        else {
            tmp.push_back(bboxs[i]);
        }
    }
    bboxs.clear();
    bboxs = tmp;
}

//删除第一行的Bboxs
void FormOperator::filter_useless_bbox_firstrow(vector<vector<vector<Bbox>>>& group_res, vector<Bbox>& bboxs){
    int top = INT_MAX;
    int bottom = -1;
    vector<Bbox> tmp;
    for (int i = 0; i < group_res[0].size(); i++){
        for (int j = 0; j < group_res[0][i].size(); j++){
            top = min(top, group_res[0][i][j].y);
            bottom = max(bottom, group_res[0][i][j].y + group_res[0][i][j].height);
        }
    }

    for (int i = 0; i < bboxs.size(); i++){
        int Iou_min_col = max(bboxs[i].y, top);
        int Iou_max_col = min(bboxs[i].y + bboxs[i].height, bottom);

        if (Iou_max_col > Iou_min_col){
            continue;
        }
        else{
            tmp.push_back(bboxs[i]);
        }
    }
    bboxs.clear();
    bboxs = tmp;
}

//去掉group_res其中某一个Bbox，并重新列聚类，行列分开操作
void FormOperator::filter_long_chi_str_bbox(vector<vector<vector<Bbox>>>& group_res, vector<vector<Bbox>>& clusters_row, vector<vector<Bbox>>& clusters_col, int row, int col, int num){

    filter_useless_bbox(group_res, bboxs, row, col, num);

    clusters_col.clear();
    clusters_row.clear();
    group_res.clear();
    cluster_col(bboxs, clusters_col);
    cluster_row(bboxs, clusters_row);
    group_clusters_res(group_res, clusters_row, clusters_col);

}

//去掉第一行的题干干扰信息
void FormOperator::filter_long_chi(vector<vector<vector<Bbox>>>& group_res, vector<vector<Bbox>>& clusters_row, vector<vector<Bbox>>& clusters_col, vector<vector<int>>& coord){

    for (int i = 0; i < coord.size(); i++){
        filter_useless_bbox(group_res, bboxs, coord[i][0], coord[i][1], coord[i][2]);
    }
    clusters_col.clear();
    clusters_row.clear();
    group_res.clear();

    cluster_col(bboxs, clusters_col);
    cluster_row(bboxs, clusters_row);
    group_clusters_res(group_res, clusters_row, clusters_col);

}

//将要运算的数字运算验证
void FormOperator::compute(vector<vector<string>> &nums, int rule){

}

//部分匹配字串，默认a的长度小于b
bool FormOperator::part_match(string a, string b){
    const char *show;
    show = strstr(b.c_str(), a.c_str());
    if (show == NULL){
        return false;
    }
    else {
        return true;
    }
}

//添加前缀匹配规则
bool FormOperator::string_blurry_match(string a, string b){
    if (a.length() == b.length()){
        return a == b;
    }
    else if (a.length() < b.length()){
        if (part_match(a, b)){
            return true;
        }
        else {
            return false;
        }
    }
    else{
        if (part_match(b, a)){
            return true;
        }
        else {
            return false;
        }
    }
}

//根据拿下来的res，进行匹配运算规则
int FormOperator::match_formula(map< pair<pair <string,string>, string>, int >& stem_map, vector<vector<vector<Bbox>>>& group_res)
{
    int res = -1;
    bool find = false;
    int row = -1;
    int col = -1;
    vector<int> row_index;
    vector<int> col_index;
    vector<vector<string>> nums;
    bool res_reminder = is_has_remainder(group_res, row_index, col_index);
    if (res_reminder){
        res = 4;
        nums_size = 4;
        vector<string> tmp;
        find = true;
        if (row_index.size() == 4){
            int cur_col = col_index[0];
            for (int i = cur_col + 1; i < group_res[0].size(); i++){
                tmp.clear();
                for (int j = 0; j < nums_size; j++){
                    if (!group_res[row_index[j]][i].empty() &&
                            (group_res[row_index[j]][i][0].class_idx == 104 || group_res[row_index[j]][i][0].class_idx == 101)){
                        if (group_res[row_index[j]][i][0].text.empty()){
                            tmp.push_back("空");
                        }
                        else {
                            tmp.push_back(group_res[row_index[j]][i][0].text);
                        }
                    }
                    else {
                        break;
                    }
                }
                if (tmp.size() == nums_size){
                    nums.push_back(tmp);
                }
            }
        }
        else {
            for (int i = row + 1; i < group_res.size(); i++){
                tmp.clear();
                for (int j = 0; j < nums.size(); j++){
                    if (!group_res[i][col_index[j]].empty() &&
                            (group_res[i][col_index[j]][0].class_idx == 104 || group_res[i][col_index[j]][0].class_idx == 101)){
                        if (group_res[i][col_index[j]][0].text.empty()){
                            tmp.push_back("空");
                        }
                        else {
                            tmp.push_back(group_res[i][col_index[j]][0].text);
                        }
                    }
                    else{
                        break;
                    }
                }
                if (tmp.size() == nums_size){
                    nums.push_back(tmp);
                }
            }
        }
    }
    else {
        map< pair<pair <string,string>, string>, int > :: iterator iter;
        vector<int> rows;
        vector<int> cols;
        for (int i = 0; i < group_res.size(); i++) {
            for (int j = 0; j < group_res[i].size(); j++) {
                if (group_res[i][j].empty() || group_res[i][j][0].class_idx != 100){
                    continue;
                }
                iter = stem_map.begin();
                rows.clear();
                cols.clear();
                while (iter != stem_map.end()){
                    if (string_blurry_match(group_res[i][j][0].text, iter->first.first.first)){
//                        cout << "pipei" << group_res[i][j][0].text << " " << iter->first.first.first << endl;
                        row = i;
                        col = j;

                        //题干在一列，且保证之后还有两行，找到剩下两行的位置
                        rows.push_back(row);
                        if (group_res.size() > (row + 2)) {
                            for (int i = row + 1; i < group_res.size() - 1; i++) {
                                if (!group_res[i][col].empty() &&
                                    string_blurry_match(group_res[i][col][0].text, iter->first.first.second)) {
                                    rows.push_back(i);
//                                    cout << "pipei2" << group_res[i][col][0].text << " " << iter->first.first.second << endl;
                                    for (int j = i + 1; j < group_res.size(); j++) {
                                        if (!group_res[j][col].empty() &&
                                            string_blurry_match(group_res[j][col][0].text, iter->first.second)) {
//                                            cout << "pipei3" << group_res[j][col][0].text << " " << iter->first.second << endl;
                                            rows.push_back(j);
                                            break;
                                        }
                                    }
                                }
                                if (rows.size() == nums_size) {
                                    res = iter->second;
                                    break;
                                }
                            }
                            if (rows.size() == nums_size) {
                                vector<string> tmp;
                                for (int i = col + 1; i < group_res[0].size(); i++) {
                                    tmp.clear();
                                    for (int j = 0; j < nums_size; j++) {
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
                                    if (tmp.size() == nums_size) {
                                        nums.push_back(tmp);
                                    }
                                }
                            }
                        }
                        if (rows.size() != nums_size && group_res[0].size() > (col + 2)){
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
                                if (cols.size() == nums_size){
                                    res = iter->second;
                                    break;
                                }
                            }

                            if (cols.size() == nums_size){

                                vector<string> tmp;
                                for (int i = row + 1; i < group_res.size(); i++){
                                    tmp.clear();
                                    for (int j = 0; j < nums_size; j++){
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
                                    if (tmp.size() == nums_size){
                                        nums.push_back(tmp);
                                    }
                                }
                            }
                        }

                        if (rows.size() == nums_size || cols.size() == nums_size){
                            find = true;
                            break;
                        }
                    }
                    rows.clear();
                    cols.clear();
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

    }

    if (!find){
        int find_normal = match_normal_formula(group_res, row_index, col_index);
        cout << "find : " << find_normal << endl;
        if (find_normal != -1){
            get_normal_res(group_res, nums, row_index, col_index);
            res = rule.size() + stoi(normal_stem[find_normal].back());
//            cout << "res; " << res << endl;
        }

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
    for (int i = 0; i < stem.size(); i++){
        stem_map.insert(make_pair(make_pair(make_pair(stem[i][0], stem[i][1]), stem[i][2]), stoi(stem[i][3])));
    }

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
                copy[j].y < tmp_top + (tmp_bottom - tmp_top) * 0.5)
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

                //合并相邻的汉字
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

            /*cout << "compare: " << copy[i].x << " " << copy[i].text << " " << copy[j].x << " " << copy[j].text << endl;
            cout << tmp_left << " " << tmp_right << endl;
            cout << (double)(Iou_max - Iou_min);
            cout << " " << (double)(0.5 * copy[j].height) << endl;*/
            if (Iou_max >= Iou_min && copy[j].class_idx == 100){
//                cout << "lianjie: " << copy[j].text << endl;
                tmp.push_back(copy[j]);
                tmp_left = min(tmp_left, copy[j].x);
                tmp_right = max(tmp_right, copy[j].x + copy[j].width);

                flag[j] = false;
            }
            else if ((double)(Iou_max - Iou_min) >= (0 * copy[j].width) &&
                    copy[j].x <= tmp_left + (tmp_right - tmp_left) * 0.9){

//                cout << "lianjie2: " << copy[j].text << endl;
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
        return true;
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
