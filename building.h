//
// Created by 稀光 on 2022/4/16.
//

#ifndef NATSYSTEM_BUILDING_H
#define NATSYSTEM_BUILDING_H

#include "person.h"
#include "LinkList/LinkList.h"

class Building{
public:
    Building() = default;

    Building(const std::string& building_code):building_code_(building_code){}

    Person& AddPerson(const std::string& personal_code,
                      const Person::TestStatus test_status = Person::kNoTesting,
                      const Person::ContiguityStatus con_status = Person::kNoContiguity){
        if(personal_code.substr(0,3) != building_code_) throw std::invalid_argument("楼号不匹配");
        auto &person = people_.InsertElem(Person(personal_code,test_status,con_status));
        ChangeStatus(person);
        return person;
    }

    Person* FindPerson(const std::string& personal_code){
        for(int i = 0;i < people_.GetLength();++i)
            if(people_[i].PersonalCode() == personal_code)
                return &people_[i];
        return nullptr;
    }

    Person::Status QueryStatus(const std::string &personal_code){
        for(int i = 0;i < people_.GetLength();++i){
            if(people_[i].PersonalCode() == personal_code)
                return people_[i].PersonalStatus();
        }
        return {};
    }

    // 同楼栋的设置为密接或次密接
    inline void UpdateBuildingStatus(const Person::ContiguityStatus status = Person::kContiguity){
        for(int i = 0; i < people_.GetLength(); ++i) {
            ChangeContiguityStatus(status,people_[i].PersonalContiguityStatus());
            people_[i].UpdateContiguityStatus(status);
        }
    }

private:
    LinkList<Person> people_;
    std::string building_code_;
    int status_[7]{0};    /// 分别表示楼中是否有 阳性，可疑，阴性，待上传，排队中，密接，次密接 的人

    void ChangeStatus(const Person& person){
        if(person.PersonalTestStatus() < Person::kNoTesting)
            ++status_[person.PersonalStatus().test_status];
        if(person.PersonalContiguityStatus() < Person::kNoContiguity)
            ++status_[person.PersonalStatus().contiguity_status + 5];
    }

    void ChangeTestStatus(const Person::TestStatus now_status,const Person::TestStatus old_status){
        if(now_status < old_status){
            if(now_status < Person::kNoTesting)
                ++status_[now_status];
            if(old_status < Person::kNoTesting)
                --status_[old_status];
        }
    }

    void ChangeContiguityStatus(const Person::ContiguityStatus now_status,const Person::ContiguityStatus old_status){
        if(now_status < old_status){
            if(now_status < Person::kNoContiguity)
                ++status_[now_status + 5];
            if(old_status < Person::kNoContiguity)
                --status_[old_status + 5];
        }
    }

};

#endif //NATSYSTEM_BUILDING_H
