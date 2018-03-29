#include <iostream>
#include <iosfwd>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdlib.h>

using namespace std;

struct RoboLabel
{
    bool dir_;
    string global_id_;
    float x_;
    float y_;
    float z_;
    float psi_;
    float theta_;
    float phi_;
    float size_[3];
    string type_;
    float weight_;
    bool available_;
};

struct RoboLabelData
{
    string pcap_name_;
    string pcd_name_;
    int frame_id_;
    int number_box_;
    vector<RoboLabel>  label_v_;
    long total_frame_;
};

class SerilizeLabelFile
{
public:
    SerilizeLabelFile();
    ~SerilizeLabelFile(){}

    /**
     * @brief serilize the label data
     * @input file_path: input the label file folder,eg. "/home/bigcong/data/sti_train_data/170825-1708-LM120/"
     * @return true if the file folder is correct
     */
    bool serilizeData(const string file_path);

    /**
     * @brief serilize the label data
     * @input file_path: input the whole label file folder,eg. "/home/bigcong/data/sti_train_data/"
     * @return true if the file folder is correct
     */
    bool serilizeDataAll(const string &file_path);

    std::string pcd_file_;//the label pcd folder,eg. "/home/bigcong/data/sti_train_data/170825-1708-LM120/pcd"
    std::vector<std::string> pcd_file_vec_;//the label pcd folders.
    std::vector<RoboLabelData> data_;//the serilized label information
    std::vector<std::vector<RoboLabelData> > data_vec_;//the serilized label information of ros bags

private:

    bool parse_rsfile(string filename, vector<RoboLabelData> &data);

    float stringToFloat(const string& str);
    int stringToInt(const string& str);
    int stringToLong(const string& str);
    bool stringToBool(const string& str);
    string Int2Str(int &num);
    string Float2str(float &num);
    string Long2Str(long &num);
    string Bool2String(bool flag);
    void moveOn(string &content, string Symbol);
    string EraseMaohao(string str);
    string EraseDquotes(string str);
    string getBtSymbol(string cont, string sym1, string sym2);
    string getSymAndMove(string &cont, string sym1, string sym2);
    vector<string> split(const string &s, const string &seperator);
    int hex_char_value(char c);
    int hex_to_decimal(const char* szHex, int len);
};










