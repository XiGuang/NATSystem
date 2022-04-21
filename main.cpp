#include <iostream>
#include "buildingList.h"
#include "personQueue.h"
#include "tubeList.h"
#include "NATSystem.h"

using namespace std;

void Stop();    // ��ͣ������

int main() {
    NATSystem NAT_system;
    cout << "���ļ���ȡ������..." <<endl;
    try{
        ifstream in;
        in.open("queue.in", ios::in);
        int single_num(0),hybrid_num(0);
        in >> hybrid_num >> single_num;
        if(hybrid_num < 0 || hybrid_num > 10000 || single_num < 0 || single_num > 10000){
            cerr << "�������ݲ��ڷ�Χ��" <<endl;
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
        cout << "�����˻�Ϲܶ���" << endl;
        for(int i = 0;i < single_num;++i){
            in >> person_code;
            NAT_system.QueueUp(person_code, true);
            cout << person_code << "  ";
        }
        cout << "�����˵��˹ܶ���" << endl;
        int finish_single(0),finish_hybrid(0);
        in >> finish_hybrid >> finish_single;   // ������ɼ�������
        in.close();
        if(finish_single > single_num || finish_single < 0 || finish_hybrid > hybrid_num || finish_hybrid < 0){
            cerr << "�����������Χ��" <<endl;
            Stop();
            return -2;
        }
        NAT_system.NATest(finish_single, true, cout);
        NAT_system.NATest(finish_hybrid, false, cout);
//        NAT_system.ShowTubeList(cout);
        cout << "��ʼ����ɣ�" << endl;
        std::system("PAUSE");
        std::system("cls");
    }catch (...){
        cerr << "��ȡ�ļ�ʧ�ܣ���" << endl;
        std::system("PAUSE");
        return -3;
    }

    while(true){
        cout << "      1. ��������Ŷ�      " << endl;
        cout << "      2. ������         " << endl;
        cout << "      3. �鿴�Ŷ����      " << endl;
        cout << "      4. �Ǽǲ��Թ���Ϣ    " << endl;
        cout << "      5. ������Ա��ѯ      " << endl;
        cout << "      6. ���˲�ѯ         " <<endl;
        cout << "      0. �˳�            " << endl;
        cout << "�����루0~6����";
        char c;
        cin >> c;
        if( c == '0') break;
        cout << endl;
        switch (c) {
            case '1':{
                std::string str;
                char chr;
                while(true){
                    cout << "��ѡ���ϲ��Ի��˲��Բ������λ��Ա��� (���� E ��ʾ�˳�)" << endl
                        << "��ʽ�磺S 00000000 ��S�����˲��ԣ�H�����ϼ�⣩��" << endl;
                    cin >> chr;
                    if(chr == 'e' || chr == 'E') break;
                    cin >> str;
                    if(chr != 'S' && chr != 's' && chr != 'h' && chr != 'H'){
                        cerr << "�����ʽ���ԣ�" << endl;
                        Stop();
                        continue;
                    }
                    try{
                        NAT_system.QueueUp(str,(chr == 's' || chr == 'S'));
                        cout << str << " ������" << (chr == 's' || chr == 'S' ? "����" : "���")
                            << "�ܶ���" << endl;
                    }catch(...){
                        cerr << "�����ʽ����ȷ������ӣ��������룡��" << endl;
                        Stop();
                        continue;
                    }
                }
                break;
            }
            case '2':{
                while(true) {
                    char chr;
                    cout << "��ѡ�� 1.��ϼ�� 2.���˼�� ������E���أ�" << endl;
                    cin >> chr;
                    if(chr == 'e' || chr == 'E') break;
                    if(chr != '1' && chr != '2'){
                        cerr << "�����ʽ���ԣ���" << endl;
                        Stop();
                        continue;
                    }
                    int i(0);
                    cout << "��������������";
                    cin >> i;
                    if(!cin.good()){    // ��ֹ�������ݴ���
                        cerr << "���������֣�����" << endl;
                        cin.clear();//�ѿɻָ��������ָ�
                        cin.ignore();
                        Stop();
                        continue;
                    }
                    try{ NAT_system.NATest(i, chr == '2',cout); }
                    catch (...){
                        cerr << "��������!!" << endl;
                        Stop();
                        continue;
                    }
                    cout << "������,������⣺" << endl;
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
                    cout << "�������Թܱ�źͲ��Խ��������E���أ�" << endl
                         << "��ʽ�磺00000 1 (1.����  2.����  3.����)" << endl;
                    cin >> str;
                    if(str[0] == 'E' || str[0] == 'e') break;
                    cin >> chr;
                    if(chr != '1' && chr != '2' && chr != '3'){
                        cerr << "������Խ��������Ҫ��" << endl;
                        Stop();
                        continue;
                    }
                    if(chr == '1' &&     // ����ֵ��false˵��tube_code����
                            NAT_system.RegisterTube(str.substr(0, 5), Person::kConfirmed));
                    else if(chr == '2' &&
                            NAT_system.RegisterTube(str.substr(0, 5), Person::kNegative));
                    else if(chr == '3' &&
                            NAT_system.RegisterTube(str.substr(0, 5), Person::kSuspected));
                    else{
                        cerr << "δ�ҵ����Թܻ��Ѿ��Ǽǹ�����" << endl;
                        Stop();
                        continue;
                    }
                    cout << "�Ǽǳɹ�,�����Ǽǣ�" << endl;
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
                    cout << "��������Ҫ��ѯ�İ�λ��Ա���루����E���أ�:" << endl;
                    cin >> str;
                    if(str.empty() || str[0] == 'E' || str[0] == 'e') break;
                    for(auto& s:str)
                        if(!isalnum(s)){
                            cerr << "����ȫ���ִ��룡��" << endl;
                            Stop();
                            continue;
                        }
                    if(str.length() != 8){
                        cerr << "���ǰ�λ���ִ��룡��" << endl;
                        Stop();
                        continue;
                    }
                    NAT_system.ShowPersonalStatus(str,cout);
                    cout << endl << "������ѯ��" << endl;
                }
                break;
            }
            default:
                cout << "���������룺" << endl;
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