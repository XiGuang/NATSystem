//
// Created by 稀光 on 2022/4/16.
//

#ifndef NATSYSTEM_BUILDING_H
#define NATSYSTEM_BUILDING_H

#include <utility>

#include "person.h"
#include "LinkList/LinkList.h"

class Building{
public:
    Building() = default;

    explicit Building(std::string  building_code):building_code_(std::move(building_code)){}

    Person& AddPerson(const std::string& personal_code,
                      const Person::TestStatus test_status = Person::kNoTesting,
                      const Person::ContiguityStatus con_status = Person::kNoContiguity){
        if(personal_code.substr(0,3) != building_code_) throw std::invalid_argument("楼号不匹配");
        return people_.InsertElem(Person(personal_code,test_status,con_status));
    }

    const Person* FindPerson(const std::string& personal_code) const {
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

    inline void UpdateToContiguity(const std::string &personal_code){   // 设置密接
        for(int i = 0; i < people_.GetLength(); ++i)
            if(people_[i].PersonalCode() == personal_code) {
                UpdateToContiguity(people_[i]);
                return;
            }
    }

    inline void UpdateToContiguity(Person& person){
        person.UpdateContiguityStatus(Person::kContiguity);
        UpdateToSecContiguity();
    }

    // 输出前必须先刷新
    void FlushStatus(){
        for(auto& s:status_)
            s = false;
        for(int i = 0;i < people_.GetLength();++i){
            if(people_[i].PersonalTestStatus() < Person::kNoTesting)
                status_[people_[i].PersonalTestStatus()] = true;
            if(people_[i].PersonalContiguityStatus() < Person::kNoContiguity)
                status_[people_[i].PersonalContiguityStatus() + 5] = true;
        }
    }

    inline void ShowTestStatus(const Person::TestStatus status,std::ostream& out){
        // 若楼中没有这种状态的直接跳过
        if(status < Person::kNoTesting && !status_[status]) return;
        for(int i = 0; i < people_.GetLength(); ++i)
            if(people_[i].PersonalStatus().test_status == status)
                out << people_[i].PersonalCode() << "  ";
    }

    inline void ShowContiguityStatus(const Person::ContiguityStatus status,std::ostream& out){
        // 若楼中没有这种状态的直接跳过
        if(status < Person::kNoContiguity && !status_[status + 5]) return;
        for(int i = 0; i < people_.GetLength(); ++i)
            if(people_[i].PersonalStatus().contiguity_status == status)
                out << people_[i].PersonalCode() << "  ";
    }

private:
    LinkList<Person> people_;
    std::string building_code_;
    bool status_[7]{false};    /// 分别表示楼中是否有 阳性，可疑，阴性，待上传，排队中，密接，次密接 的人

    // 同楼栋的设置为次密接
    inline void UpdateToSecContiguity(){
        for(int i = 0; i < people_.GetLength(); ++i)
            people_[i].UpdateContiguityStatus(Person::kSecContiguity);
    }
};

#endif //NATSYSTEM_BUILDING_H
