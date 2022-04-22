//
// Created by 稀光 on 2022/4/12.
//

#ifndef NATSYSTEM_PERSONQUEUE_H
#define NATSYSTEM_PERSONQUEUE_H

#include <queue>
#include "person.h"
#include "Queue/LinkQueue.h"
#include "buildingList.h"

class TubeList;

class NATSystem;

class PersonQueue{
private:
    LinkQueue<std::string> single_queue_,hybrid_queue_;    // 单人检测，混合检测

public:
    PersonQueue() = default;

    void QueueUp(Person& person, const bool is_single = true){
        if(is_single)   // 人员代码加入对应队列
            single_queue_.push(person.PersonalCode());
        else
            hybrid_queue_.push(person.PersonalCode());
        person.UpdateTestStatus(Person::kQueuing);  // 更改状态为排队中
    }

    // 现在在单人队里的人都是密接
    void SetContiguity(BuildingList& building_list){
        for(int i = 0;i < single_queue_.size(); ++i){
            std::string personal_code;
            single_queue_.pop(personal_code);
            building_list.UpdateToContiguity(personal_code);
            single_queue_.push(personal_code);
        }
    }
    // 单人检测队列长度
    int LengthOfSingleQueue(){return single_queue_.size();}
    // 混合检测队列长度
    int LengthOfHybridQueue(){return hybrid_queue_.size();}

    friend std::ostream &operator<<(std::ostream& out,PersonQueue& person_queue){
        out << std::endl << "\t单人检测\t混合检测" << std::endl;
        out << (person_queue.single_queue_.empty() ? "\t  无\t" : "\t\t   ") // 队列空，输出无
            << (person_queue.hybrid_queue_.empty() ? "   无" : "") << std::endl;
        // 让队伍纵向排列
        for(int i = 0;i < person_queue.single_queue_.size() || i < person_queue.hybrid_queue_.size(); ++i){
            std::string single_code,hybrid_code;
            if(i < person_queue.single_queue_.size()) { // 不超过size时出队，防止重复输出
                person_queue.single_queue_.pop(single_code);
            }
            if(i < person_queue.hybrid_queue_.size()){
                person_queue.hybrid_queue_.pop(hybrid_code);
            }
            out << '\t' << (single_code.empty() ? "         " : single_code)
                << '\t' << (hybrid_code.empty() ? "" : hybrid_code) << std::endl;
            if(!single_code.empty())
                person_queue.single_queue_.push(single_code);
            if(!hybrid_code.empty())
                person_queue.hybrid_queue_.push(hybrid_code);
        }
        return out;
    }

    // 核酸检测
    friend class NATSystem;
};

#endif //NATSYSTEM_PERSONQUEUE_H
