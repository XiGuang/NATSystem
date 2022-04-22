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
    // 添加人员进楼
    Person& AddPerson(const std::string& personal_code,
                      const Person::TestStatus test_status = Person::kNoTesting,
                      const Person::ContiguityStatus con_status = Person::kNoContiguity){
        if(personal_code.substr(0,3) != building_code_) throw std::invalid_argument("楼号不匹配");
        return people_.InsertElem(Person(personal_code,test_status,con_status));
    }
    // 添加人员进楼
    Person& AddPerson(const Person& person){
        if(person.BuildingNum() != building_code_) throw std::invalid_argument("楼号不匹配");
        return people_.InsertElem(person);
    }
    // 用人员代码搜索人员
    Person* FindPerson(const std::string& personal_code) const {
        for(int i = 0;i < people_.GetLength();++i)
            if(people_[i].PersonalCode() == personal_code)
                return &people_[i];
        return nullptr;
    }
    // 查询人员状态
    __attribute__((unused)) Person::Status QueryStatus(const std::string &personal_code){
        for(int i = 0;i < people_.GetLength();++i){
            if(people_[i].PersonalCode() == personal_code)
                return people_[i].PersonalStatus();
        }
        return {};
    }
    // 设置为阳性，也设置同楼为密接
    inline void UpdateToConfirmed(Person& person){  // 设置阳性
        if(person.BuildingNum() != building_code_) return;
        person.UpdateTestStatus(Person::kConfirmed);
        UpdateToContiguity();
    }
    // 设置为密接，同时设置同楼为次密接
    inline void UpdateToContiguity(const std::string &personal_code){   // 设置密接
        if(personal_code.substr(0,3) != building_code_) return;
        for(int i = 0; i < people_.GetLength(); ++i)
            if(people_[i].PersonalCode() == personal_code) {
                UpdateToContiguity(people_[i]);
                return;
            }
    }
    // 设置为密接，同时设置同楼为次密接
    inline void UpdateToContiguity(Person& person){
        if(person.BuildingNum() != building_code_) return;
        person.UpdateContiguityStatus(Person::kContiguity);
        UpdateToSecContiguity();
    }
    // 输出前先刷新状态
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
    // 输出所有特定检测状态的人的人员代码
    inline void ShowTestStatus(const Person::TestStatus status,std::ostream& out){
        // 若楼中没有这种状态的直接跳过
        if(status < Person::kNoTesting && !status_[status]) return;
        for(int i = 0; i < people_.GetLength(); ++i)
            if(people_[i].PersonalStatus().test_status == status)
                out << people_[i].PersonalCode() << "  ";
    }
    // 输出所有特定密接状态的人的人员代码
    inline void ShowContiguityStatus(const Person::ContiguityStatus status,std::ostream& out){
        // 若楼中没有这种状态的直接跳过
        if(status < Person::kNoContiguity && !status_[status + 5]) return;
        for(int i = 0; i < people_.GetLength(); ++i)
            if(people_[i].PersonalContiguityStatus() == status && people_[i].PersonalTestStatus() != Person::kConfirmed)
                out << people_[i].PersonalCode() << "  ";
    }
    // 楼号
    std::string BuildingCode() const {return building_code_;}

private:
    LinkList<Person> people_;
    std::string building_code_; // 楼号
    bool status_[7]{false};    /// 分别表示楼中是否有 阳性，可疑，阴性，待上传，排队中，密接，次密接 的人

    // 同楼栋的设置为次密接
    inline void UpdateToSecContiguity(){
        for(int i = 0; i < people_.GetLength(); ++i)
            people_[i].UpdateContiguityStatus(Person::kSecContiguity);
    }
    // 同楼栋的设置为密接
    inline void UpdateToContiguity(){
        for(int i = 0; i < people_.GetLength(); ++i)
            people_[i].UpdateContiguityStatus(Person::kContiguity);
    }
};

#endif //NATSYSTEM_BUILDING_H
