#include "../include/parse.h"
#include <math.h>


//template <class Type>
SerilizeLabelFile::SerilizeLabelFile() {
    data_.clear();

}

bool SerilizeLabelFile::serilizeData(const string file_path) {
    std::string label_file = file_path;
//    pcd_file_ = file_path + "/pcd";
    bool flag = parse_rsfile(label_file,data_);
    if(!flag)
        return flag;
    return true;
}



//cjf parse
bool SerilizeLabelFile::parse_rsfile(string filename, std::vector<RoboLabelData> &data)
{
    std::ifstream file(filename.c_str(), std::ios::in);
    file.is_open();
    std::string line;
    std::string head, frame_id, pcd_path, content;
    cout<<"file name = "<<filename <<endl;
    while (true)
    {
        //cout << "step 1";
        if (file.eof())
        {
            std::cout << "file is eof"<<std::endl;
            break;
        }
        //cout << "step 2";
        RoboLabelData	bd_data_load_;
        file >> head;
        file >> frame_id;
        file >> pcd_path;
        file >> content;

        vector<string> slist = split(pcd_path,"/");
        string frame_id_ = getSymAndMove(content, ":", ",");
        string box_num = getSymAndMove(content, ":", ",");
        moveOn(content, ":");
        if (box_num == "" || stringToInt(box_num) < 1)
        {
            //		qDebug() << "Ending is an Empty Line ";
            break;
        }
        bd_data_load_.pcap_name_ = getSymAndMove(head, "0", ".");// head;
        bd_data_load_.pcd_name_ = slist[slist.size()-1];
        bd_data_load_.frame_id_ = stringToInt(frame_id);
        bd_data_load_.number_box_ = stringToInt(box_num);

        //	cout << "box_num = " << box_num <<endl;
        for (int i = 0; i < stringToInt(box_num); i++)
        {
            RoboLabel label_;
            label_.dir_ = stringToBool(getSymAndMove(content, ":", ","));
            label_.global_id_ =  getBtSymbol(content, ":", ",");
            moveOn(content, "{");
            label_.x_ = stringToFloat(getSymAndMove(content, ":", ","));
            label_.y_ = stringToFloat(getSymAndMove(content, ":", ","));
            label_.z_ = stringToFloat(getSymAndMove(content, ":", ","));
            moveOn(content, "{");
            label_.phi_ = stringToFloat(getSymAndMove(content, ":", ","));
            //		cout << "phi = " << label_.phi_ << endl;
            label_.psi_ = stringToFloat(getSymAndMove(content, ":", ","));
            label_.theta_ = stringToFloat(getSymAndMove(content, ":", ","));
            moveOn(content, "[");
            label_.size_[0] = stringToFloat(getSymAndMove(content, "0", ","));
            label_.size_[1] = stringToFloat(getSymAndMove(content, "0", ","));
            label_.size_[2] = stringToFloat(getSymAndMove(content, "0", "]"));
            moveOn(content, ",");
            label_.type_ = getSymAndMove(content, ":", ",");
            label_.weight_ = stringToFloat(getSymAndMove(content, ":", ","));
            label_.available_ = stringToBool(getSymAndMove(content, ":","}" ));
            moveOn(content, ",");
            // cout <<  "weight = " <<label_.weight_;
            //  cout <<  "available_ = " <<label_.available_;
            // cout << "size_= " << label_.size_[0] << "\t" << label_.size_[1] << "\t" << label_.size_[2] << endl;
            bd_data_load_.label_v_.push_back(label_);
        }
        bd_data_load_.total_frame_ = stringToLong(getSymAndMove(content, ":", ","));//�����굱ǰ֡����
        getSymAndMove(content, ":", "}");
        data.push_back(bd_data_load_);	//��֡���ݽ���һ��push
    }
    if (data.size() == 0)
    {
        std::cout<<"data size is zero!"<<std::endl;
    }
    cout<< "size = " << data.size() <<endl;
    file.close();
    return true;
}

 float SerilizeLabelFile::stringToFloat(const string& str)
{
    istringstream iss(str);
    float num;
    iss >> num;
    return num;
}

int SerilizeLabelFile::stringToInt(const string& str)
{
    istringstream iss(str);
    int num;
    iss >> num;
    return num;
}

int SerilizeLabelFile::stringToLong(const string& str)
{
    istringstream iss(str);
    long num;
    iss >> num;
    return num;
}

bool SerilizeLabelFile::stringToBool(const string& str)
{
    if (str == "true")
    {
        return true;
    }
    else
    {
        return false;
    }
}

int SerilizeLabelFile::hex_char_value(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c >= 'a' && c <= 'f')
        return (c - 'a' + 10);
    else if (c >= 'A' && c <= 'F')
        return (c - 'A' + 10);
    return 0;
}

int SerilizeLabelFile::hex_to_decimal(const char* szHex, int len)
{
    int result = 0;
    for (int i = 0; i < len; i++)
    {
        result += (int)pow((float)16, (int)len - i - 1) * hex_char_value(szHex[i]);
    }
    return result;
}

vector<string> SerilizeLabelFile::split(const string &s, const string &seperator){
    vector<string> result;
    typedef string::size_type string_size;
    string_size i = 0;

    while (i != s.size()){
        //找到字符串中首个不等于分隔符的字母；
        int flag = 0;
        while (i != s.size() && flag == 0){
            flag = 1;
            for (string_size x = 0; x < seperator.size(); ++x)
                if (s[i] == seperator[x]){
                    ++i;
                    flag = 0;
                    break;
                }
        }

        //找到又一个分隔符，将两个分隔符之间的字符串取出；
        flag = 0;
        string_size j = i;
        while (j != s.size() && flag == 0){
            for (string_size x = 0; x < seperator.size(); ++x)
                if (s[j] == seperator[x]){
                    flag = 1;
                    break;
                }
            if (flag == 0)
                ++j;
        }
        if (i != j){
            result.push_back(s.substr(i, j - i));
            i = j;
        }
    }
    return result;
}

//前移到某个符号+1
void SerilizeLabelFile::moveOn(string &content, string Symbol)
{
    size_t move_Pos = content.find(Symbol);
    content = content.substr(move_Pos + 1);
}
string SerilizeLabelFile::EraseDquotes(string str)
{
    string temp = "";
    for (int i = 0; i < str.size(); i++)
    {
        if ('\"' != str[i])
        {
            temp += str[i];
        }
    }
    return temp;
}
string SerilizeLabelFile::getBtSymbol(string cont, string sym1, string sym2)
{
    if (sym1 == "0")
    {
        return EraseDquotes((cont.substr(0, cont.find(sym2) - cont.find(sym1) - 1)));
    }
    else if (sym2 == "0")
    {
        return EraseDquotes(cont.substr(cont.find(sym1) + 1));
    }
    else
        return EraseDquotes((cont.substr(cont.find(sym1) + 1, cont.find(sym2) - cont.find(sym1) - 1)));
}

string SerilizeLabelFile::getSymAndMove(string &cont, string sym1, string sym2)
{
    string temp_str;
    if (sym1 == "0")
    {
        temp_str = EraseDquotes((cont.substr(0, cont.find(sym2))));
        moveOn(cont, sym2);
        return temp_str;
    }
    else
    {
        temp_str = EraseDquotes((cont.substr(cont.find(sym1) + 1, cont.find(sym2) - cont.find(sym1) - 1)));
        moveOn(cont, sym2);
        return temp_str;
    }
}


//--------------------------------------------------------------

string SerilizeLabelFile::Int2Str(int &num)
{
   string str;
   char temp[256];
   sprintf(temp, "%d", num);
   str = temp;
   return  str;
}

string SerilizeLabelFile::Float2str(float &num)
{
   string str;
   char temp[256];
   sprintf(temp, "%.6f", num);
   str = temp;
   return  str;
}


string SerilizeLabelFile::Bool2String(bool flag)
{
    string temp;
    if (flag)
    {
        ;
        temp = "true";
    }
    else
    {
        temp = "false";
    }
    //cout << "temp =" << temp;
    return temp;
}
string SerilizeLabelFile::Long2Str(long &num)
{
   string str;
   char temp[256];
   sprintf(temp, "%ld", num);
   str = temp;
   return  str;
}
