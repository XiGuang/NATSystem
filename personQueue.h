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
        if(is_single)
            single_queue_.push(person.PersonalCode());
        else
            hybrid_queue_.push(person.PersonalCode());
        person.UpdateTestStatus(Person::kQueuing);
    }

    // 现在在单人队里的和此后单人检测加进来的人都会是密接
    void SetConfirmed(BuildingList& person_list){
        for(int i = 0;i < single_queue_.GetLength();++i){
            std::string personal_code;
            single_queue_.pop(personal_code);
            person_list.UpdateToContiguity(personal_code);
            single_queue_.push(personal_code);
        }
    }

    int LengthOfSingleQueue(){return single_queue_.GetLength();}

    int LengthOfHybridQueue(){return hybrid_queue_.GetLength();}

    friend std::ostream &operator<<(std::ostream& out,PersonQueue& person_queue){
        out << std::endl << "\t单人检测\t混合检测" << std::endl;
        out << (person_queue.single_queue_.GetLength() == 0 ? "\t  无\t" : "\t\t   ")
            << (person_queue.hybrid_queue_.GetLength() == 0 ? "   无" : "") << std::endl;
        for(int i = 0;i < person_queue.single_queue_.GetLength() || i < person_queue.hybrid_queue_.GetLength();++i){
            std::string single_code,hybrid_code;
            if(i < person_queue.single_queue_.GetLength())
                person_queue.single_queue_.pop(single_code);
            if(i < person_queue.hybrid_queue_.GetLength())
                person_queue.hybrid_queue_.pop(hybrid_code);
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
