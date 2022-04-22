//
// Created by ϡ�� on 2022/4/13.
//

#ifndef NATSYSTEM_NATSYSTEM_H
#define NATSYSTEM_NATSYSTEM_H

#include "buildingList.h"
#include "personQueue.h"
#include "tubeList.h"

// ������ϵͳ
class NATSystem{
private:
    BuildingList building_list; // ¥����
    PersonQueue person_queue;   // �ŶӼ�����
    TubeList tube_list; // �Թ�����
    // �����ǰ�����
    TestTube * NATest(bool is_single);

public:
    NATSystem() = default;
    // �����Ŷ�
    void QueueUp(const std::string& personal_code, bool is_single);
    // �����ǰ���������
    bool NATest(int people_num, bool is_single, std::ostream &out);
    // �Ǽ��Թ���Ϣ
    bool RegisterTube(const std::string &tube_code, Person::TestStatus status = Person::kNegative);
    // ���˲�ѯ
    void ShowPersonalStatus(const std::string& personal_code,std::ostream& out);
    // ��ѯ����״̬��Ա
    void ShowAllStatus(std::ostream& out);
    // ��ǰ�Ŷӵ���Ա����Ա����
    inline void ShowQueue(std::ostream& out){out << person_queue;}
};



#endif //NATSYSTEM_NATSYSTEM_H
