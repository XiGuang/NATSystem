//
// Created by 稀光 on 2022/4/13.
//

#ifndef NATSYSTEM_NATSYSTEM_H
#define NATSYSTEM_NATSYSTEM_H

#include "buildingList.h"
#include "personQueue.h"
#include "tubeList.h"

// 核酸检测系统
class NATSystem{
private:
    BuildingList building_list; // 楼链表
    PersonQueue person_queue;   // 排队检测队列
    TubeList tube_list; // 试管链表
    // 检测最前面的人
    TestTube * NATest(bool is_single);

public:
    NATSystem() = default;
    // 加入排队
    void QueueUp(const std::string& personal_code, bool is_single);
    // 检测最前面的数个人
    bool NATest(int people_num, bool is_single, std::ostream &out);
    // 登记试管信息
    bool RegisterTube(const std::string &tube_code, Person::TestStatus status = Person::kNegative);
    // 个人查询
    void ShowPersonalStatus(const std::string& personal_code,std::ostream& out);
    // 查询所有状态人员
    void ShowAllStatus(std::ostream& out);
    // 当前排队的人员的人员代码
    inline void ShowQueue(std::ostream& out){out << person_queue;}
};



#endif //NATSYSTEM_NATSYSTEM_H
