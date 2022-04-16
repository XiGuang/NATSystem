//
// Created by ϡ�� on 2022/4/12.
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
    LinkQueue<std::string> single_queue_,hybrid_queue_;    // ���˼�⣬��ϼ��

public:
    PersonQueue() = default;

    void QueueUp(Person& person, const bool is_single = true){
        if(is_single)
            single_queue_.push(person.PersonalCode());
        else
            hybrid_queue_.push(person.PersonalCode());
        person.UpdateTestStatus(Person::kQueuing);
    }

    // �����ڵ��˶���ĺʹ˺��˼��ӽ������˶������ܽ�
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
        out << std::endl << "\t���˼��\t��ϼ��" << std::endl;
        out << (person_queue.single_queue_.GetLength() == 0 ? "\t  ��\t" : "\t\t   ")
            << (person_queue.hybrid_queue_.GetLength() == 0 ? "   ��" : "") << std::endl;
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

    // ������
    friend class NATSystem;
};

#endif //NATSYSTEM_PERSONQUEUE_H
