#include <iostream>
#include "personList.h"
#include "personQueue.h"
#include "tubeList.h"
#include "NATSystem.h"

using namespace std;

void Stop();    // ��ͣ������

int main() {
    NATSystem NAT_system;
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
                        cout << "�����ʽ���ԣ�" << endl;
                        Stop();
                        continue;
                    }
                    try{
                        NAT_system.QueueUp(str,(chr == 's' || chr == 'S'));
                        cout << " �Ŷӳɹ����������룺" << endl;
                    }catch(...){
                        cout << "�����ʽ����ȷ������ӣ��������룡��" << endl;
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
                        cout << "�����ʽ���ԣ���" << endl;
                        Stop();
                        continue;
                    }
                    int i(0);
                    cout << "��������������";
                    cin >> i;
                    if(!cin.good()){    // ��ֹ�������ݴ���
                        cout << "���������֣�����" << endl;
                        cin.clear();//�ѿɻָ��������ָ�
                        cin.ignore();
                        Stop();
                        continue;
                    }
                    if(!NAT_system.NATest(i,chr == '2')){
                        cout << "��������!!" << endl;
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
                    cin >> chr >> chr;
                    if(chr != '1' && chr != '2' && chr != '3'){
                        cout << "������Խ��������Ҫ��" << endl;
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
                        cout << "δ�ҵ����Թܻ��Ѿ��Ǽǹ�����" << endl;
                        Stop();
                        continue;
                    }
                    cout << "�Ǽǳɹ�,�����Ǽǣ�" << endl;
                }
                break;
            }
            case '5':
                NAT_system.ShowAllStatus(cout);
                break;
            case '6':{
                while(true){
                    string str;
                    cout << "��������Ҫ��ѯ�İ�λ��Ա���루����E���أ�:" << endl;
                    cin >> str;
                    if(str.empty() || str[0] == 'E') break;
                    for(auto& s:str)
                        if(!isalnum(s)){
                            cout << "����ȫ���ִ��룡��" << endl;
                            Stop();
                            continue;
                        }
                    if(str.length() != 8){
                        cout << "���ǰ�λ���ִ��룡��" << endl;
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
    }
    return 0;
}

void Stop(){
    std::system("PAUSE");
    std::system("cls");
}