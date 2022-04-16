//
// Created by 稀光 on 2022/4/12.
//

#ifndef NATSYSTEM_PERSONLIST_H
#define NATSYSTEM_PERSONLIST_H

#include "person.h"
#include "LinkList/LinkList.h"

// 人员信息表
class PersonList{
private:
    LinkList<Person> people_;

public:
    PersonList() = default;

    explicit PersonList(const Person& person){people_.InsertElem(person);}

    PersonList(const PersonList&) = delete;

    inline Person& AddPerson(const std::string &personal_code,
                             const Person::TestStatus status = Person::kNoTesting,
                             const Person::ContiguityStatus status1 = Person::kNoContiguity){
        return people_.InsertElem(Person(personal_code, status, status1)); // 返回插入的元素
    }

    Person *FindPerson(const std::string &personal_code){
        for(int i = 0; i < people_.GetLength(); ++i)
            if(people_[i].PersonalCode() == personal_code)
                return &people_[i];
        return nullptr;
    }

    Person::Status QueryStatus(const std::string &personal_code){
        for(int i = 0; i < people_.GetLength(); ++i){
            if(people_[i].PersonalCode() == personal_code)
                return people_[i].PersonalStatus();
        }
        return {};
    }

    bool UpdateTestStatus(const std::string &personal_code,const Person::TestStatus test_status){
        for(int i = 0; i < people_.GetLength(); ++i)
            if(people_[i].PersonalCode() == personal_code) {
                people_[i].UpdateTestStatus(test_status);
                return true;
            }
        return false;
    }

    bool UpdateContiguityStatus(const std::string &personal_code,const Person::ContiguityStatus contiguity_status){
        for(int i = 0; i < people_.GetLength(); ++i)
            if(people_[i].PersonalCode() == personal_code) {
                people_[i].UpdateContiguityStatus(contiguity_status);
                return true;
            }
        return false;
    }

    inline void UpdateToContiguity(const std::string &personal_code){   // 设置密接
        for(int i = 0; i < people_.GetLength(); ++i)
            if(people_[i].PersonalCode() == personal_code) {
                UpdateToContiguity(people_[i]);
                return;
            }
    }

    inline void UpdateToContiguity(Person& person){
        person.UpdateContiguityStatus(Person::kContiguity);
        UpdateBuildingStatus(person.BuildingNum(),Person::kSecContiguity);
    }

    // 同楼栋的设置为密接或次密接
    inline void UpdateBuildingStatus(const std::string &building_num,const Person::ContiguityStatus status = Person::kContiguity){
        for(int i = 0; i < people_.GetLength(); ++i)
            if(people_[i].BuildingNum() == building_num)
                people_[i].UpdateContiguityStatus(status);
    }

    inline void ShowTestStatus(const Person::TestStatus status,std::ostream& out){
        for(int i = 0; i < people_.GetLength(); ++i)
            if(people_[i].PersonalStatus().test_status == status)
                out << people_[i].PersonalCode() << "  ";
    }

    inline void ShowContiguityStatus(const Person::ContiguityStatus status,std::ostream& out){
        for(int i = 0; i < people_.GetLength(); ++i)
            if(people_[i].PersonalStatus().contiguity_status == status)
                out << people_[i].PersonalCode() << "  ";
    }

    // 遍历people的函数
    void Traverse(void (*func)(const Person&)){
        people_.Traverse(func);
    }

};

#endif //NATSYSTEM_PERSONLIST_H
