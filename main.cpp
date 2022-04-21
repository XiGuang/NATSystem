#include <iostream>
#include "buildingList.h"
#include "personQueue.h"
#include "tubeList.h"
#include "NATSystem.h"

using namespace std;

void Stop();    // 暂停命令行

int main() {
    NATSystem NAT_system;
    cout << "从文件读取数据中..." <<endl;
    try{
        ifstream in;
        in.open("queue.in", ios::in);
        int single_num(0),hybrid_num(0);
        in >> hybrid_num >> single_num;
        if(hybrid_num < 0 || hybrid_num > 10000 || single_num < 0 || single_num > 10000){
            cerr << "读入数据不在范围！" <<endl;
            in.close();
            Stop();
            return -1;
        }
        string person_code;
        for(int i = 0;i < hybrid_num;++i){
            in >> person_code;
            NAT_system.QueueUp(person_code, false);
            cout << person_code << "  ";
        }
        cout << "加入了混合管队列" << endl;
        for(int i = 0;i < single_num;++i){
            in >> person_code;
            NAT_system.QueueUp(person_code, true);
            cout << person_code << "  ";
        }
        cout << "加入了单人管队列" << endl;
        int finish_single(0),finish_hybrid(0);
        in >> finish_hybrid >> finish_single;   // 读入完成检测的人数
        in.close();
        if(finish_single > single_num || finish_single < 0 || finish_hybrid > hybrid_num || finish_hybrid < 0){
            cerr << "完成人数超范围！" <<endl;
            Stop();
            return -2;
        }
        NAT_system.NATest(finish_single, true, cout);
        NAT_system.NATest(finish_hybrid, false, cout);
//        NAT_system.ShowTubeList(cout);
        cout << "初始化完成！" << endl;
        std::system("PAUSE");
        std::system("cls");
    }catch (...){
        cerr << "读取文件失败！！" << endl;
        std::system("PAUSE");
        return -3;
    }

    while(true){
        cout << "      1. 加入核酸排队      " << endl;
        cout << "      2. 检测核酸         " << endl;
        cout << "      3. 查看排队情况      " << endl;
        cout << "      4. 登记测试管信息    " << endl;
        cout << "      5. 各类人员查询      " << endl;
        cout << "      6. 个人查询         " <<endl;
        cout << "      0. 退出            " << endl;
        cout << "请输入（0~6）：";
        char c;
        cin >> c;
        if( c == '0') break;
        cout << endl;
        switch (c) {
            case '1':{
                std::string str;
                char chr;
                while(true){
                    cout << "请选择混合测试或单人测试并输入八位人员编号 (输入 E 表示退出)" << endl
                        << "格式如：S 00000000 （S代表单人测试，H代表混合检测）：" << endl;
                    cin >> chr;
                    if(chr == 'e' || chr == 'E') break;
                    cin >> str;
                    if(chr != 'S' && chr != 's' && chr != 'h' && chr != 'H'){
                        cerr << "输入格式不对！" << endl;
                        Stop();
                        continue;
                    }
                    try{
                        NAT_system.QueueUp(str,(chr == 's' || chr == 'S'));
                        cout << str << " 加入了" << (chr == 's' || chr == 'S' ? "单人" : "混合")
                            << "管队列" << endl;
                    }catch(...){
                        cerr << "输入格式不正确或已入队，重新输入！！" << endl;
                        Stop();
                        continue;
                    }
                }
                break;
            }
            case '2':{
                while(true) {
                    char chr;
                    cout << "请选择 1.混合检测 2.单人检测 （输入E返回）" << endl;
                    cin >> chr;
                    if(chr == 'e' || chr == 'E') break;
                    if(chr != '1' && chr != '2'){
                        cerr << "输入格式不对！！" << endl;
                        Stop();
                        continue;
                    }
                    int i(0);
                    cout << "请输入检测人数：";
                    cin >> i;
                    if(!cin.good()){    // 防止输入数据错误
                        cerr << "请输入数字！！！" << endl;
                        cin.clear();//把可恢复的流都恢复
                        cin.ignore();
                        Stop();
                        continue;
                    }
                    try{ NAT_system.NATest(i, chr == '2',cout); }
                    catch (...){
                        cerr << "人数过多!!" << endl;
                        Stop();
                        continue;
                    }
                    cout << "检测完成,继续检测：" << endl;
                    Stop();
                }
                break;
            }
            case '3':
                NAT_system.ShowQueue(cout);
                Stop();
                break;
            case '4':{
                string str;
                char chr;
                while(true){
                    cout << "请输入试管编号和测试结果（输入E返回）" << endl
                         << "格式如：00000 1 (1.阳性  2.阴性  3.可疑)" << endl;
                    cin >> str;
                    if(str[0] == 'E' || str[0] == 'e') break;
                    cin >> chr;
                    if(chr != '1' && chr != '2' && chr != '3'){
                        cerr << "输入测试结果不符合要求。" << endl;
                        Stop();
                        continue;
                    }
                    if(chr == '1' &&     // 返回值是false说明tube_code有误
                            NAT_system.RegisterTube(str.substr(0, 5), Person::kConfirmed));
                    else if(chr == '2' &&
                            NAT_system.RegisterTube(str.substr(0, 5), Person::kNegative));
                    else if(chr == '3' &&
                            NAT_system.RegisterTube(str.substr(0, 5), Person::kSuspected));
                    else{
                        cerr << "未找到此试管或已经登记过！！" << endl;
                        Stop();
                        continue;
                    }
                    cout << "登记成功,继续登记：" << endl;
                }
                break;
            }
            case '5':
                NAT_system.ShowAllStatus(cout);
                Stop();
                break;
            case '6':{
                while(true){
                    string str;
                    cout << "请输入想要查询的八位人员代码（输入E返回）:" << endl;
                    cin >> str;
                    if(str.empty() || str[0] == 'E' || str[0] == 'e') break;
                    for(auto& s:str)
                        if(!isalnum(s)){
                            cerr << "不是全数字代码！！" << endl;
                            Stop();
                            continue;
                        }
                    if(str.length() != 8){
                        cerr << "不是八位数字代码！！" << endl;
                        Stop();
                        continue;
                    }
                    NAT_system.ShowPersonalStatus(str,cout);
                    cout << endl << "继续查询：" << endl;
                }
                break;
            }
            default:
                cout << "请重新输入：" << endl;
                break;
        }
        std::system("cls");
    }
    return 0;
}

void Stop(){
    std::system("PAUSE");
    cin.ignore((numeric_limits< streamsize >::max)(), '\n');
    std::system("cls");
}