//
// Created by 稀光 on 2022/4/12.
//

#ifndef NATSYSTEM_BUILDINGLIST_H
#define NATSYSTEM_BUILDINGLIST_H

#include "person.h"
#include "LinkList/LinkList.h"
#include "building.h"

// 人员信息表
class BuildingList{
private:
    LinkList<Building> buildings_;  // 楼链表
    // 根据楼号找楼
    Building* FindBuilding(const std::string& building_code){
        for(int i = 0;i < buildings_.GetLength();++i)
            if(buildings_[i].BuildingCode() == building_code)
                return &buildings_[i];
        return nullptr;
    }

public:
    BuildingList() = default;

    BuildingList(const BuildingList&) = delete;
    // 添加人到对应的楼链表
    inline Person& AddPerson(const std::string &personal_code,
                             const Person::TestStatus status = Person::kNoTesting,
                             const Person::ContiguityStatus status1 = Person::kNoContiguity){
        auto building = FindBuilding(personal_code.substr(0,3));    // 查找楼
        if(building == nullptr) // 若楼不存在，则插入楼
            building = &buildings_.InsertElem(Building(personal_code.substr(0,3)));
        return building->AddPerson(personal_code,status,status1);   // 把人添加到楼里
    }
    // 根据人员代码寻找人
    Person *FindPerson(const std::string &personal_code){
        auto building = FindBuilding(personal_code.substr(0, 3));
        if (building != nullptr)
            return building->FindPerson(personal_code);
        return nullptr;
    }

    /// 设置密接，同时设置次密接
    inline void UpdateToContiguity(const std::string &personal_code){
        auto building = FindBuilding(personal_code.substr(0,3));
        if(building != nullptr)
            building->UpdateToContiguity(personal_code);
    }

    /// 设置密接，同时设置次密接
    inline void UpdateToContiguity(Person& person){
        auto building = FindBuilding(person.BuildingNum());
        if(building != nullptr)
            building->UpdateToContiguity(person);
    }
    /// 设置阳性，同时设置同楼为密接
    void UpdateToConfirmed(Person& person){
        auto building = FindBuilding(person.BuildingNum());
        if(building != nullptr)
            building->UpdateToConfirmed(person);
    }
    // 展示所有特定检测状态的人员代码
    inline void ShowTestStatus(const Person::TestStatus status,std::ostream& out){
        for(int i = 0; i < buildings_.GetLength(); ++i)
            buildings_[i].ShowTestStatus(status,out);
    }
    // 展示所有特定密接状态的人员代码
    inline void ShowContiguityStatus(const Person::ContiguityStatus status,std::ostream& out){
        for(int i = 0; i < buildings_.GetLength(); ++i)
            buildings_[i].ShowContiguityStatus(status,out);
    }
    // 刷新楼的状态
    inline void FlushStatuses(){
        for(int i = 0;i < buildings_.GetLength();++i)
            buildings_[i].FlushStatus();
    }

};

#endif //NATSYSTEM_BUILDINGLIST_H
