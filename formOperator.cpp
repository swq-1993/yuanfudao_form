//
// Created by sunwanqi on 2018/11/9.
//
#include "header.h"
using std::vector;
using std::endl;
using std::cout;
using std::string;

//单独处理正方体表面积体积问题
void FormOperator::process_square() {
    int formula_normal = formula - rule.size();
    vector<string> tmp;
    for (int i = 0; i < nums_result.size(); i++){
//      表面积是手写体
        if (hand_write_index[i][1]){
            tmp.push_back("6");
            tmp.push_back("*");
            tmp.push_back("(");
            for (int m = 0; m < nums_result[i][0].size(); m++){
                tmp.push_back(nums_result[i][0][m]);
            }
            tmp.push_back("*");
            for (int m = 0; m < nums_result[i][0].size(); m++){
                tmp.push_back(nums_result[i][0][m]);
            }
            tmp.push_back(")");
            tmp.push_back("=");
            for (int m = 0; m < nums_result[i][1].size(); m++){
                tmp.push_back(nums_result[i][1][m]);
            }
            splice_res.push_back(tmp);
            tmp.clear();
        }
        if (hand_write_index[i][2]){
            for (int num = 0; num < 3; num++){
                for (int m = 0; m < nums_result[i][0].size(); m++){
                    tmp.push_back(nums_result[i][0][m]);
                }
                tmp.push_back("*");
            }
            tmp.pop_back();
            tmp.push_back("=");
            for (int m = 0; m < nums_result[i][2].size(); m++){
                tmp.push_back(nums_result[i][2][m]);
            }
            splice_res.push_back(tmp);
            tmp.clear();
        }
    }
}

//单独处理圆直径周长面积题
void FormOperator::process_circle() {
    int formula_normal = formula - rule.size();
    vector<string> tmp;
    for (int i = 0; i < nums_result.size(); i++) {
        //半径是印刷体
        if (!hand_write_index[i][0]) {
            if (hand_write_index[i][1]) {
                tmp.push_back("2");
                tmp.push_back("*");
                for (int m = 0; m < nums_result[i][0].size(); m++) {
                    tmp.push_back(nums_result[i][0][m]);
                }
                tmp.push_back("=");
                for (int m = 0; m < nums_result[i][1].size(); m++) {
                    tmp.push_back(nums_result[i][1][m]);
                }
                splice_res.push_back(tmp);
                tmp.clear();
            }
            if (hand_write_index[i][2]) {
                tmp.push_back("2");
                tmp.push_back("*");
                tmp.push_back("pi");
                tmp.push_back("*");
                for (int m = 0; m < nums_result[i][0].size(); m++) {
                    tmp.push_back(nums_result[i][0][m]);
                }
                tmp.push_back("=");
                for (int m = 0; m < nums_result[i][2].size(); m++) {
                    tmp.push_back(nums_result[i][2][m]);
                }
                splice_res.push_back(tmp);
                tmp.clear();
            }
            if (hand_write_index[i][3]) {
                tmp.push_back("pi");
                tmp.push_back("*");
                for (int m = 0; m < nums_result[i][0].size(); m++) {
                    tmp.push_back(nums_result[i][0][m]);
                }
                tmp.push_back("*");
                for (int m = 0; m < nums_result[i][0].size(); m++) {
                    tmp.push_back(nums_result[i][0][m]);
                }
                tmp.push_back("=");
                for (int m = 0; m < nums_result[i][3].size(); m++) {
                    tmp.push_back(nums_result[i][3][m]);
                }
                splice_res.push_back(tmp);
                tmp.clear();
            }
        }
            //直径是印刷体
        else if (!hand_write_index[i][1] && hand_write_index[i][0]) {
            if (hand_write_index[i][0]) {
                tmp.push_back("0.5");
                tmp.push_back("*");
                for (int m = 0; m < nums_result[i][1].size(); m++) {
                    tmp.push_back(nums_result[i][1][m]);
                }
                tmp.push_back("=");
                for (int m = 0; m < nums_result[i][0].size(); m++) {
                    tmp.push_back(nums_result[i][0][m]);
                }
                splice_res.push_back(tmp);
                tmp.clear();
            }
            if (hand_write_index[i][2]) {
                tmp.push_back("pi");
                tmp.push_back("*");
                for (int m = 0; m < nums_result[i][1].size(); m++) {
                    tmp.push_back(nums_result[i][1][m]);
                }
                tmp.push_back("=");
                for (int m = 0; m < nums_result[i][2].size(); m++) {
                    tmp.push_back(nums_result[i][2][m]);
                }
                splice_res.push_back(tmp);
                tmp.clear();
            }
            if (hand_write_index[i][3]) {
                tmp.push_back("0.25");
                tmp.push_back("*");
                tmp.push_back("pi");
                tmp.push_back("*");
                for (int m = 0; m < nums_result[i][1].size(); m++) {
                    tmp.push_back(nums_result[i][1][m]);
                }
                tmp.push_back("*");
                for (int m = 0; m < nums_result[i][1].size(); m++) {
                    tmp.push_back(nums_result[i][1][m]);
                }
                tmp.push_back("=");
                for (int m = 0; m < nums_result[i][3].size(); m++) {
                    tmp.push_back(nums_result[i][3][m]);
                }
                splice_res.push_back(tmp);
                tmp.clear();
            }
        } else {
            cout << "给出的已知数值不是半径或者直径" << endl;
        }
        splice_res.push_back(tmp);
        tmp.clear();
    }
}

//重新整理输出结构体
void FormOperator::arrange_res() {
    Res tmp_res;
    for (int i = 0; i < result_boxs.size(); i++){
        tmp_res.x = result_boxs[i].x;
        tmp_res.y = result_boxs[i].y;
        tmp_res.width = result_boxs[i].width;
        tmp_res.height = result_boxs[i].height;
        tmp_res.splice_result = splice_res[i];
        if (!tmp_res.splice_result.empty()){
            final_res.push_back(tmp_res);
        }
    }
}

//将解析拼接非四则运算
void FormOperator::transform_normal_result() {
    vector<string> tmp;
    tmp.clear();
    int formula_normal = formula - rule.size();
    for (int i = 0; i < nums_result.size(); i++){
        //连加、连减、连乘(包括长方形面积)的case
        if (formula_normal == 0 || formula_normal == 1 || formula_normal == 2 || formula_normal == 6){
            for (int j = 0; j < nums_result[i].size(); j++){
                if (j != 0 && j != nums_result[i].size() - 1){
                    if (formula_normal == 0){
                        tmp.push_back("+");
                    }
                    else if (formula_normal == 1){
                        tmp.push_back("-");
                    }
                    else {
                        tmp.push_back("*");
                    }
                }
                else if (j == nums_result[i].size() - 1){
                    tmp.push_back("=");
                }
                for (int m = 0; m < nums_result[i][j].size(); m++){
                    tmp.push_back(nums_result[i][j][m]);
                }
            }
        }
        //长方形周长的case
        else if (formula_normal == 3){
            for (int j = 0; j < nums_result[i].size(); j++){
                if (j == 0){
                    tmp.push_back("2");
                    tmp.push_back("*");
                    tmp.push_back("(");
                }
                else if (j == 1){
                    tmp.push_back("+");
                }
                else {
                    tmp.push_back(")");
                    tmp.push_back("=");
                }
                for (int m = 0; m < nums_result[i][j].size(); m++){
                    tmp.push_back(nums_result[i][j][m]);
                }
            }
        }
        //正方形周长
        else if (formula_normal == 4){
            for (int j = 0; j < nums_result[i].size(); j++){
                if (j == 0){
                    tmp.push_back("4");
                    tmp.push_back("*");
                }
                else {
                    tmp.push_back("=");
                }
                for (int m = 0; m < nums_result[i][j].size(); m++){
                    tmp.push_back(nums_result[i][j][m]);
                }
            }
        }
        //正方形面积
        else if (formula_normal == 5){
            for (int j = 0; j < nums_result[i].size(); j++){
                if (j == 1){
                    tmp.push_back("*");
                }
                else if (j == nums_result[i].size() - 1){
                    tmp.push_back("=");
                }
                for (int m = 0; m < nums_result[i][j].size(); m++){
                    tmp.push_back(nums_result[i][j][m]);
                }
            }
        }
        //三角形面积
        else if (formula_normal == 7){
            for (int j = 0; j < nums_result[i].size(); j++){
                if (j == 0){
                    tmp.push_back("0.5");
                    tmp.push_back("*");
                }
                else if (j == 1){
                    tmp.push_back("*");
                }
                else {
                    tmp.push_back("=");
                }
                for (int m = 0; m < nums_result[i][j].size(); m++){
                    tmp.push_back(nums_result[i][j][m]);
                }
            }
        }
        //梯形面积
        else if (formula_normal == 9){
            for (int j = 0; j < nums_result[i].size(); j++){
                if (j == 0){
                    tmp.push_back("0.5");
                    tmp.push_back("*");
                    tmp.push_back("(");
                }
                else if (j == 1){
                    tmp.push_back("+");
                }
                else if (j == 2){
                    tmp.push_back(")");
                    tmp.push_back("*");
                }
                else {
                    tmp.push_back("=");
                }
                for (int m = 0; m < nums_result[i][j].size(); m++){
                    tmp.push_back(nums_result[i][j][m]);
                }
            }
        }
        //既加又减运算
        else if (formula_normal == 14){
            for (int j = 0; j < nums_result[i].size(); j++){
                if (j == 1){
                    tmp.push_back("+");
                }
                else if (j == 2){
                    tmp.push_back("-");
                }
                else if (j == nums_result[i].size() - 1){
                    tmp.push_back("=");
                }
                for (int m = 0; m < nums_result[i][j].size(); m++){
                    tmp.push_back(nums_result[i][j][m]);
                }
            }
        }
//      以下情况会出现多个手写体
//      圆的直径、周长、面积
        else if (formula_normal == 11){
            process_circle();
        }
        //正方形表面积体积
        else if (formula_normal == 12){
            process_square();
        }

        //圆柱体表面积体积
        else if (formula_normal == 13){
            cout << "就一两个case，暂时没有处理" << endl;
        }

        splice_res.push_back(tmp);
        tmp.clear();
    }
    /*for (int i = 0; i < splice_res.size(); i++){
        for (int j = 0; j < splice_res[i].size(); j++){
            cout << splice_res[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;*/
    arrange_res();
}

//判断每一行的列是否一样，不一样的话就是非法框，不用匹配运算，直接过滤掉
bool FormOperator::is_legal_form() {
    bool res = true;
    if (group_res.empty()){
        return false;
    }
    int cols = group_res[0].size();
    for (int i = 1; i < group_res.size(); i++){
        if (cols != group_res[i].size()){
            return false;
        }
    }
    return res;
}

//将解析拼接四则运算
void FormOperator::transform_result(){
    std::vector<std::string> law{"+", "-", "*", "/"};
    if (formula > 3 || formula < 0){
        result_boxs.clear();
        std::cout << "算式不是正常四则运算" << std::endl;
        return;
    }
    if (nums_result.size() != result_boxs.size()){
        std::cout << "算式中有未识别完整的式子" << std::endl;
        result_boxs.clear();
        return;
    }

    std::vector<std::string> tmp;
    tmp.clear();
    for (int i = 0; i < nums_result.size(); i++){
        for (int j = 0; j < nums_result[i].size(); j++){
            if (j == 1){
                tmp.push_back(law[formula]);
            }
            else if (j == 2){
                tmp.push_back("=");
            }
            for (int m = 0; m < nums_result[i][j].size(); m++){
                tmp.push_back(nums_result[i][j][m]);
            }
        }
        splice_res.push_back(tmp);
        tmp.clear();
    }

    arrange_res();
}

//调用整个识别运算规则
void FormOperator::run_result()
{
    cluster_row(bboxs, clusters_row);
    cluster_col(bboxs, clusters_col);
    group_clusters_res(group_res, clusters_row, clusters_col);
    splice_chi_char(group_res);
    std::vector<std::vector<int>> coord_first = exist_long_chi(group_res);
    if (!coord_first.empty()){
        filter_long_chi(group_res, clusters_row, clusters_col, coord_first);
        splice_chi_char(group_res);
    }

    std::vector<int> coord = exist_long_chi_mid(group_res);
    while (coord.size() == 3){
        filter_long_chi_str_bbox(group_res, clusters_row, clusters_col, coord[0], coord[1], coord[2]);
        splice_chi_char(group_res);
        coord.clear();
        coord = exist_long_chi_mid(group_res);
    }

    init(stem_map);

    for (int i = 0; i < group_res.size(); i++)
    {
        std::cout << "第" << i + 1 << "行：" << std::endl;
        for (int j = 0; j < group_res[i].size(); j++)
        {

            for (int m = 0; m < group_res[i][j].size(); m++)
            {
//                cout << formOperator.group_res[i][j][m].x << " " << formOperator.group_res[i][j][m].y << " " << formOperator.group_res[i][j][m].text << " ";
                std::cout << group_res[i][j][m].text << " ";
            }
            std::cout << "|";
        }
        std::cout << std::endl;
    }

    if (!is_legal_form()){
        std::cout << "非法表格" << std::endl;
        return;
    }


    formula = match_formula(stem_map, group_res);
    if (formula == -1){
        std::cout << "未知运算 " << std::endl;
    }
    else if (formula >= rule.size()){
        std::cout << normal_rule[formula - rule.size()] << std::endl;

        /*for (int i = 0; i < nums_result.size(); i++){
            for (int j = 0; j < nums_result[i].size(); j++){
                for (int m =  0; m < nums_result[i][j].size(); m++){
                    cout << nums_result[i][j][m] << " ";
                }
            }
            cout << endl;
        }
        cout << endl;*/
        transform_normal_result();
    }
    else {
        std::cout << rule[formula] << std::endl;
        transform_result();
    }
}

//获取非四则运算匹配之后的结果
void FormOperator::get_normal_res(std::vector<std::vector<std::vector<Bbox>>>& group_res, std::vector<std::vector<std::string>>& nums, std::vector<int>& rows, std::vector<int>& cols){
    std::vector<std::string> tmp;
    vector<vector<string>> tmp_2d;
    tmp.clear();
    tmp_2d.clear();
    vector<bool> tmp_hand_write;
//    cout << "coordinate: " << rows.size() << " " << cols.size() << endl;
    if (cols.size() == 1){
        int cur_col = cols[0];
        for (int col = cur_col + 1; col < group_res[0].size(); col++){
            int tmp_size = 0;
            for (int i = 0; i < rows.size(); i++){
                if (!group_res[rows[i]][col].empty() &&
                        (group_res[rows[i]][col][0].class_idx == 104 || group_res[rows[i]][col][0].class_idx == 101)){
                    if (group_res[rows[i]][col][0].class_idx == 101){
                        result_boxs.push_back(group_res[rows[i]][col][0]);
                        tmp_hand_write.push_back(true);
                    }
                    else{
                        tmp_hand_write.push_back(false);
                    }
                    if (group_res[rows[i]][col][0].text.empty()){
                        tmp.push_back("$BNK$");
                    }
                    else {
                        for (int index = 0; index < group_res[rows[i]][col].size(); index++) {
                            tmp.push_back(group_res[rows[i]][col][index].text);
                        }
                    }
                    tmp_2d.push_back(tmp);
                    tmp_size++;
                    tmp.clear();
                }
                else {
                    break;
                }
            }
            if (tmp_size == rows.size()){
                nums_result.push_back(tmp_2d);
//                如果一个式子里面没有手写体框，就添加最后一个框为判别框
                if (nums_result.size() == result_boxs.size() + 1){
                    result_boxs.push_back(group_res[rows.back()][col][0]);
                    tmp_hand_write.push_back(false);
                }
                tmp_2d.clear();
                hand_write_index.push_back(tmp_hand_write);
                tmp_hand_write.clear();
            }
        }
    }
    else if (rows.size() == 1){
        int cur_row = rows[0];
        for (int row = cur_row + 1; row < group_res.size(); row++){
            int tmp_size = 0;
            for (int i = 0; i < cols.size(); i++){
                if (!group_res[row][cols[i]].empty() &&
                        (group_res[row][cols[i]][0].class_idx == 104 || group_res[row][cols[i]][0].class_idx == 101)){
                    if (group_res[row][cols[i]][0].class_idx == 101){
                        result_boxs.push_back(group_res[row][cols[i]][0]);
                        tmp_hand_write.push_back(true);
                    }
                    else{
                        tmp_hand_write.push_back(false);
                    }
                    if (group_res[row][cols[i]][0].text.empty()){
                        tmp.push_back("$BNK$");
                    }
                    else {
                        for (int index = 0; index < group_res[row][cols[i]].size(); index++){
                            tmp.push_back(group_res[row][cols[i]][index].text);
                        }
                    }
                    tmp_2d.push_back(tmp);
                    tmp_size++;
                    tmp.clear();
                }
                else {
                    break;
                }

            }
            if (tmp_size == cols.size()){
                nums_result.push_back(tmp_2d);
//                如果一个式子里面没有手写体框，就添加最后一个框为判别框
                if (nums_result.size() == result_boxs.size() + 1){
                    result_boxs.push_back(group_res[row][cols.back()][0]);
                    tmp_hand_write.push_back(false);
                }
                tmp_2d.clear();
                hand_write_index.push_back(tmp_hand_write);
                tmp_hand_write.clear();

            }

        }
    }

}

//非四则运算的匹配
int FormOperator::match_normal_formula(std::vector<std::vector<std::vector<Bbox>>>& group_res, std::vector<int>& rows, std::vector<int>& cols){
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
                            std::cout << "matched normal computation" << std::endl;
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
bool FormOperator::is_has_remainder(std::vector<std::vector<std::vector<Bbox>>>& group_res, std::vector<int>& row_index, std::vector<int>& col_index){
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
std::vector<int> FormOperator::exist_long_chi_mid(std::vector<std::vector<std::vector<Bbox>>>& group_res){
    std::vector<int> coord;
    if (group_res.empty()){
        return coord;
    }
    for (int i = 0; i < group_res.size(); i++){
        for (int j = 0; j < group_res[i].size(); j++){
            for (int m = 0; m < group_res[i][j].size(); m++){
                if (group_res[i][j][m].text.length() > chi_length_mid &&
                        group_res[i][j][m].class_idx == 100){
                    std::cout << "判断要删除的字符串:  " << group_res[i][j][m].text << " length: " << group_res[i][j][m].text.length() << std::endl;
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
std::vector<std::vector<int>> FormOperator::exist_long_chi(std::vector<std::vector<std::vector<Bbox>>>& group_res){
    std::vector<int> tmp;
    std::vector<std::vector<int>> coord;
    if (group_res.empty()){
        return coord;
    }
    for (int i = 0; i < group_res[0].size(); i++){
        //if (!group_res[0].empty()){
            for (int j = 0; j < group_res[0][i].size(); j++){
                if (group_res[0][i][j].text.length() > chi_length && group_res[0][i][j].class_idx == 100)
                {
                    std::cout << "判断要删除的字符串:  " << group_res[0][i][j].text << " length:" << group_res[0][i][j].text.size() << std::endl;
                    tmp.push_back(0);
                    tmp.push_back(i);
                    tmp.push_back(j);
                    coord.push_back(tmp);
                    tmp.clear();
                }
            }
        //}

    }

    return coord;
}

//输出干扰的bbox
void FormOperator::filter_useless_bbox(std::vector<std::vector<std::vector<Bbox>>>& group_res, std::vector<Bbox>& bboxs, int row, int col, int num){
    std::vector<Bbox> tmp;
    int x = group_res[row][col][num].x;
    int y = group_res[row][col][num].y;
    int width = group_res[row][col][num].width;
    int height = group_res[row][col][num].height;
    for (int i = 0; i < bboxs.size(); i++){
        int Iou_min_row = std::max(bboxs[i].x, x);
        int Iou_max_row = std::min(bboxs[i].x + bboxs[i].width, x + width);
        int Iou_min_col = std::max(bboxs[i].y, y);
        int Iou_max_col = std::min(bboxs[i].y + bboxs[i].height, y + height);
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
void FormOperator::filter_useless_bbox_firstrow(std::vector<std::vector<std::vector<Bbox>>>& group_res, std::vector<Bbox>& bboxs){
    int top = INT_MAX;
    int bottom = -1;
    std::vector<Bbox> tmp;
    for (int i = 0; i < group_res[0].size(); i++){
        for (int j = 0; j < group_res[0][i].size(); j++){
            top = std::min(top, group_res[0][i][j].y);
            bottom = std::max(bottom, group_res[0][i][j].y + group_res[0][i][j].height);
        }
    }

    for (int i = 0; i < bboxs.size(); i++){
        int Iou_min_col = std::max(bboxs[i].y, top);
        int Iou_max_col = std::min(bboxs[i].y + bboxs[i].height, bottom);

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
void FormOperator::filter_long_chi_str_bbox(std::vector<std::vector<std::vector<Bbox>>>& group_res, std::vector<std::vector<Bbox>>& clusters_row, std::vector<std::vector<Bbox>>& clusters_col, int row, int col, int num){

    filter_useless_bbox(group_res, bboxs, row, col, num);

    clusters_col.clear();
    clusters_row.clear();
    group_res.clear();
    cluster_col(bboxs, clusters_col);
    cluster_row(bboxs, clusters_row);
    group_clusters_res(group_res, clusters_row, clusters_col);

}

//去掉第一行的题干干扰信息
void FormOperator::filter_long_chi(std::vector<std::vector<std::vector<Bbox>>>& group_res, std::vector<std::vector<Bbox>>& clusters_row, std::vector<std::vector<Bbox>>& clusters_col, std::vector<std::vector<int>>& coord){

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
void FormOperator::compute(std::vector<std::vector<std::string>> &nums, int rule){

}

//部分匹配字串，默认a的长度小于b
bool FormOperator::part_match(std::string a, std::string b){
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
bool FormOperator::string_blurry_match(std::string a, std::string b){
    if (a == "" || b == ""){
        return false;
    }
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
int FormOperator::match_formula(std::map< std::pair<std::pair <std::string,std::string>, std::string>, int >& stem_map, std::vector<std::vector<std::vector<Bbox>>>& group_res)
{
    int res = -1;
    bool find = false;
    int row = -1;
    int col = -1;

    std::vector<int> row_index;
    std::vector<int> col_index;
    std::vector<std::vector<std::string>> nums;
    bool res_reminder = is_has_remainder(group_res, row_index, col_index);
    if (res_reminder){
        res = 4;
        nums_size = 4;
        std::vector<std::string> tmp;
        std::vector<std::vector<std::string>> tmp_2d;
        find = true;
        int tmp_size = 0;
        if (row_index.size() == 4){
            int cur_col = col_index[0];
            for (int i = cur_col + 1; i < group_res[0].size(); i++){
                tmp_size = 0;
                for (int j = 0; j < nums_size; j++){
                    if (!group_res[row_index[j]][i].empty() &&
                            (group_res[row_index[j]][i][0].class_idx == 104 || group_res[row_index[j]][i][0].class_idx == 101)){
                        if (group_res[row_index[j]][i][0].text.empty()){
                            tmp.push_back("$BNK$");
                        }
                        else {
                            for (int index = 0; index < group_res[row_index[j]][i].size(); index++){
                                tmp.push_back(group_res[row_index[j]][i][index].text);
                            }
                        }
                        tmp_2d.push_back(tmp);
                        tmp_size++;
                        tmp.clear();
                    }
                    else {
                        break;
                    }

                }
                if (tmp_size == nums_size){
                    nums_result.push_back(tmp_2d);
                    tmp_2d.clear();
                }
            }
        }
        else {
            for (int i = row + 1; i < group_res.size(); i++){
                tmp_size = 0;
                for (int j = 0; j < nums_size; j++){
                    if (!group_res[i][col_index[j]].empty() &&
                            (group_res[i][col_index[j]][0].class_idx == 104 || group_res[i][col_index[j]][0].class_idx == 101)){

                        if (group_res[i][col_index[j]][0].text.empty()){
                            tmp.push_back("$BNK$");
                        }
                        else {
                            for (int index = 0; index < group_res[i][col_index[j]].size(); index++){
                                tmp.push_back(group_res[i][col_index[j]][index].text);
                            }
                        }
                        tmp_2d.push_back(tmp);
                        tmp_size++;
                        tmp.clear();
                    }
                    else{
                        break;
                    }
                }
                if (tmp_size == nums_size){
                    nums_result.push_back(tmp_2d);
                }
            }
        }
    }
    else {
        int tmp_size = 0;
        std::map< std::pair<std::pair <std::string,std::string>, std::string>, int > :: iterator iter;
        std::vector<int> rows;
        std::vector<int> cols;
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
                                std::vector<std::string> tmp;
                                std::vector<std::vector<std::string>> tmp_2d;
                                for (int i = col + 1; i < group_res[0].size(); i++) {
                                    tmp_size = 0;
                                    for (int j = 0; j < nums_size; j++) {
                                        if (!group_res[rows[j]][i].empty() &&
                                            (group_res[rows[j]][i][0].class_idx == 104 || group_res[rows[j]][i][0].class_idx == 101)) {
//                            cout << "题目坐标： "<< rows[j] << " " << i << endl;

                                            if (group_res[rows[j]][i][0].class_idx == 101){
                                                result_boxs.push_back(group_res[rows[j]][i][0]);
                                            }
                                            if (group_res[rows[j]][i][0].text.empty()) {
                                                tmp.push_back("$BNK$");
                                            } else {
                                                for (int index = 0; index < group_res[rows[j]][i].size(); index++)
                                                tmp.push_back(group_res[rows[j]][i][index].text);
                                            }
                                            tmp_2d.push_back(tmp);
                                            tmp_size++;
                                            tmp.clear();

                                        } else {
                                            break;
                                        }
                                    }
                                    if (tmp_size == nums_size) {
                                        nums_result.push_back(tmp_2d);
//                                        如果一个式子里面没有手写体框，就添加最后一个框为判别框
                                        if (nums_result.size() == result_boxs.size() + 1){
                                            result_boxs.push_back(group_res[rows[nums_size - 1]][i][0]);
                                        }
                                        tmp_2d.clear();
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

                                std::vector<std::string> tmp;
                                std::vector<std::vector<std::string>> tmp_2d;
                                for (int i = row + 1; i < group_res.size(); i++){
                                    tmp_size = 0;
                                    for (int j = 0; j < nums_size; j++){
                                        if (!group_res[i][cols[j]].empty() && (group_res[i][cols[j]][0].class_idx == 104 || group_res[i][cols[j]][0].class_idx == 101)){
//                            cout << "题目坐标： "<< i << " " << cols[j] << endl;

                                            if (group_res[i][cols[j]][0].class_idx == 101){
                                                result_boxs.push_back(group_res[i][cols[j]][0]);
                                            }
                                            if (group_res[i][cols[j]][0].text.empty()){
                                                tmp.push_back("$BNK$");
                                            }
                                            else{
                                                for (int index = 0; index < group_res[i][cols[j]].size(); index++) {
                                                    tmp.push_back(group_res[i][cols[j]][index].text);
                                                }
                                            }
                                            tmp_2d.push_back(tmp);
                                            tmp_size++;
                                            tmp.clear();
                                        }
                                        else{
                                            break;
                                        }
                                    }
                                    if (tmp_size == nums_size){
                                        nums_result.push_back(tmp_2d);
//                                        如果一个式子里面没有手写体框，就添加最后一个框为判别框
                                        if (nums_result.size() == result_boxs.size() + 1){
                                            result_boxs.push_back(group_res[i][cols[nums_size - 1]][0]);
                                        }
                                        tmp_2d.clear();
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
//        std::cout << "find : " << find_normal << std::endl;
        if (find_normal != -1){
            get_normal_res(group_res, nums, row_index, col_index);
            res = rule.size() + stoi(normal_stem[find_normal].back());
//            cout << "res; " << res << endl;
        }

    }

    /*if (res != -1){
        for (int i = 0; i < nums_result.size(); i++){
            for (int j = 0; j < nums_result[i].size(); j++){
                cout << nums_result[i][j] << " ";
            }
            cout << endl;
        }
    }*/
    return res;
}

//将所给的题型初始化到指定数据结构
void FormOperator::init(std::map< std::pair<std::pair <std::string,std::string>, std::string>, int >& stem_map) {
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

//查找两个std::vector的交集
std::vector<Bbox> FormOperator::intersection(std::vector<Bbox>& A, std::vector<Bbox>& B)
{
    std::vector<Bbox> res;
    std::map<std::pair<int, int>, Bbox>part_a, part_b;
    for(auto a : A) {
        part_a.insert(std::make_pair(std::make_pair(a.x, a.y), a));
    }
    for (auto b : B) {
        part_b.insert(std::make_pair(std::make_pair(b.x, b.y), b));
    }

    std::map<std::pair<int, int>, Bbox> :: iterator iter = part_a.begin();
    while (iter != part_a.end()) {
        if (part_b.find(std::make_pair(iter->first.first, iter->first.second)) != part_b.end()){
            res.push_back(iter->second);
        }
        iter++;
    }

    return res;
}

//合并行、列的结果
void FormOperator::group_clusters_res(std::vector<std::vector<std::vector<Bbox>>>& group_res, std::vector<std::vector<Bbox>>& clusters_row, std::vector<std::vector<Bbox>>& clusters_col) {
    int rows = clusters_row.size();
    int cols = clusters_col.size();
    std::vector<Bbox> tmp;
    std::vector<std::vector<Bbox>> tmp_row;
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
void FormOperator::splice_chi_char(std::vector<std::vector<std::vector<Bbox>>>& group_res)
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
                        int iou_min = std::max(group_res[i][j][m].y, group_res[i][j][n].y);
                        int iou_max = std::min(group_res[i][j][m].y + group_res[i][j][m].height, \
                                            group_res[i][j][n].y + group_res[i][j][n].height);
                        if (group_res[i][j][n].class_idx == 100 && (iou_max - iou_min) > 0)
                        {
                            //合并连续的印刷体汉字
                            group_res[i][j][m].width = group_res[i][j][n].x + group_res[i][j][n].width - group_res[i][j][m].x;
                            group_res[i][j][m].y = std::min(group_res[i][j][m].y, group_res[i][j][n].y);
                            group_res[i][j][m].height = std::max(group_res[i][j][m].y + group_res[i][j][m].height, \
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
void FormOperator::cluster_row(std::vector<Bbox>& bboxs, std::vector<std::vector<Bbox>>& clusters_row)
{
    std::vector<Bbox> copy = bboxs;
    sort(bboxs.begin(), bboxs.end(), compare);
    std::vector<bool> flag(copy.size(), true);
    std::vector<Bbox> tmp;
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
            int Iou_min = std::max(tmp_top, copy[j].y);
            int Iou_max = std::min(tmp_bottom, copy[j].y + copy[j].height);
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
                tmp_top = std::min(tmp_top, copy[j].y);
                tmp_bottom = std::max(tmp_bottom, copy[j].y + copy[j].height);

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

void FormOperator::cluster_col2(std::vector<std::vector<Bbox>>& clusters_row, std::vector<std::vector<std::vector<Bbox>>> group_res){

    std::vector<Bbox> tmp;
    std::vector<std::vector<bool>> flag;
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
void FormOperator::cluster_col(std::vector<Bbox>& bboxs, std::vector<std::vector<Bbox>>& clusters_col)
{
    std::vector<Bbox> copy = bboxs;
    sort(copy.begin(), copy.end(), compare_col);
    std::vector<bool> flag(copy.size(), true);
    std::vector<Bbox> tmp;
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
            int Iou_min = std::max(tmp_left, copy[j].x);
            int Iou_max = std::min(tmp_right, copy[j].x + copy[j].width);

            /*cout << "compare: " << copy[i].x << " " << copy[i].text << " " << copy[j].x << " " << copy[j].text << endl;
            cout << tmp_left << " " << tmp_right << endl;
            cout << (double)(Iou_max - Iou_min);
            cout << " " << (double)(0.5 * copy[j].height) << endl;*/
            if (Iou_max >= Iou_min && copy[j].class_idx == 100){
//                cout << "lianjie: " << copy[j].text << endl;
                tmp.push_back(copy[j]);
                tmp_left = std::min(tmp_left, copy[j].x);
                tmp_right = std::max(tmp_right, copy[j].x + copy[j].width);

                flag[j] = false;
            }
            else if ((double)(Iou_max - Iou_min) >= (0 * copy[j].width) &&
                    copy[j].x <= tmp_left + (tmp_right - tmp_left) * 0.9){

//                cout << "lianjie2: " << copy[j].text << endl;
                tmp.push_back(copy[j]);
                tmp_left = std::min(tmp_left, copy[j].x);
                tmp_right = std::max(tmp_right, copy[j].x + copy[j].width);

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
float FormOperator::getVar(std::vector<std::vector<Bbox>>& clusters, std::vector<Cluster_center>& centers, const int cur_k, bool row_or_col) {
    float var = 0;
    for (int i = 0; i < cur_k; i++)
    {
        std::vector<Bbox> cluster = clusters[i];
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
int FormOperator::cluster_of_tuple(std::vector<Cluster_center>& centers, Bbox bbox, const int cur_k, bool row_or_col) {
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
Cluster_center FormOperator::get_means(std::vector<Bbox>& clusters) {
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
float FormOperator::k_means(std::vector<Bbox>& bboxs, const int cur_k, std::vector<std::vector<Bbox>>& clusters, std::vector<Cluster_center>& centers, bool row_or_col) {
    //std::vector<std::vector<Bbox>> clusters(k);
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
        std::cout << "第" << label + 1 << "个簇" << std::endl;
        std::vector<Bbox> box = clusters[label];
        for (i = 0; i < box.size(); i++) {
            std::cout << box[i].x << " " << box[i].y << " " << box[i].text << std::endl;
        }
        std::cout << std::endl;
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
            std::cout << "第" << label + 1 << "个簇" << std::endl;
            std::vector<Bbox> box = clusters[label];
            for (i = 0; i < box.size(); i++) {
                std::cout << box[i].x << " " << box[i].y << " " << box[i].text << std::endl;
            }
            std::cout << std::endl;
        }
    }
    return newVal;
}

//找最优的K值
int FormOperator::find_best_k(std::vector<Bbox>& bboxs, const int k_min, const int k_max, std::vector<std::vector<Bbox>>& clusters, std::vector<Cluster_center>& centers, bool row_or_col) {
    int final_k = 0;
    int cur_k = k_min;
    float final_loss = 0.0;
    std::vector<std::vector<Bbox>> tmp;
    std::vector<Cluster_center> tmp_centers;
    while (cur_k <= k_max) {
        if (cur_k == k_min) {
            final_loss = k_means(bboxs, cur_k, clusters, centers, row_or_col);
            final_k = cur_k;
        }
        else {
            float loss = k_means(bboxs, cur_k, tmp, tmp_centers, row_or_col);
            std::cout << "loss: " << loss << std::endl;
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
void FormOperator::group_row(std::vector<std::vector<Bbox>>& clusters, std::vector<Cluster_center>& centers) {
    std::vector<int> cluster_bottoms(clusters.size());
    std::vector<int> cluster_tops(clusters.size());
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
            int Iou_min = std::max(cluster_tops[i], cluster_tops[j]);
            int Iou_max = std::min(cluster_bottoms[i], cluster_bottoms[j]);
            if (Iou_max >= Iou_min) {
                clusters[i].insert(clusters[i].end(), clusters[j].begin(), clusters[j].end());
                float new_center_x = (centers[i].x + centers[j].x) / 2;
                float new_center_y = (centers[i].y + centers[j].y) / 2;
                centers[i].x = new_center_x;
                centers[i].y = new_center_y;
                cluster_bottoms[i] = std::max(cluster_bottoms[i], cluster_bottoms[j]);
                cluster_tops[i] = std::min(cluster_tops[i], cluster_tops[j]);

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
void FormOperator::group_col(std::vector<std::vector<Bbox>>& clusters, std::vector<Cluster_center>& centers)
{
    std::vector<int> cluster_rights(clusters.size());
    std::vector<int> cluster_lefts(clusters.size());
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
            int Iou_min = std::max(cluster_lefts[i], cluster_lefts[j]);
            int Iou_max = std::min(cluster_rights[i], cluster_rights[j]);
            if (Iou_max >= Iou_min) {
                clusters[i].insert(clusters[i].end(), clusters[j].begin(), clusters[j].end());
                float new_center_x = (centers[i].x + centers[j].x) / 2;
                float new_center_y = (centers[i].y + centers[j].y) / 2;
                centers[i].x = new_center_x;
                centers[i].y = new_center_y;
                cluster_rights[i] = std::max(cluster_rights[i], cluster_rights[j]);
                cluster_lefts[i] = std::min(cluster_lefts[i], cluster_lefts[j]);

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
bool FormOperator::compare(const Bbox &a, const Bbox &b) {
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
bool FormOperator::comp_row(const std::vector<Bbox>& a, const std::vector<Bbox>& b) {
    return a[0].y < b[0].y;
}

//列结果的排序规则
bool FormOperator::comp_col(const std::vector<Bbox>& a, const std::vector<Bbox>& b){
    return a[0].x < b[0].x;
}

//对一个格子里面的box按从左到右排序
bool FormOperator::comp_col_single(const Bbox a, const Bbox b) {
//    if a.y < b.y
    int iou_min = std::max(a.y, b.y);
    int iou_max = std::min(a.y + a.height, b.y + b.height);
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
void FormOperator::analysis(std::vector<std::string>& file_content, std::vector<Bbox>& bboxs, std::vector<Big_bbox>& big_bboxs) {
    for (int i = 0; i < file_content.size(); i++) {
        Bbox bbox;
        Big_bbox big_bbox;
        std::vector<std::string> splitedLine = split_line(file_content[i], ' ');
        if (splitedLine.size() >= 5){
            std::string class_id = split_line(splitedLine[4], ':')[1];
            if (class_id == "203" || class_id == "200" || class_id == "201" || class_id == "202" || class_id == "204") {
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
void FormOperator::filter(std::vector<Bbox>& bboxs, Big_bbox big_bbox){
    std::vector<Bbox> tmp;
    for (int i = 0; i < bboxs.size(); i++){
        int Iou_min_row = std::max(bboxs[i].x, big_bbox.x);
        int Iou_max_row = std::min(bboxs[i].x + bboxs[i].width, big_bbox.x + big_bbox.width);

        int Iou_min_col = std::max(bboxs[i].y, big_bbox.y);
        int Iou_max_col = std::min(bboxs[i].y + bboxs[i].height, big_bbox.y + big_bbox.height);

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
std::vector<std::string> FormOperator::split_line(std::string single_line, char symbol) {
    int pos = 0;
    int npos = 0;
    std::vector<std::string> result;
    while ((npos = single_line.find(symbol, pos)) != -1) {
        std::string tmp = single_line.substr(pos, npos - pos);
        result.push_back(tmp);
        pos = npos + 1;
    }
    result.push_back(single_line.substr(pos, single_line.length() - pos));
    return result;
}

//从识别结果的TXT中读取数据到内存
void FormOperator::ReadDataFromTxt(std::string filename) {
    std::ifstream in(filename);
    std::string line;
    if (in.is_open()) {
        while(getline(in, line)) {
            file_content.push_back(line);
        }
    }
    else {
        std::cout << "no such file! " << std::endl;
    }
}
