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
    LinkList<Building> buildings_;

    Building* FindBuilding(const std::string& building_code){
        for(int i = 0;i < buildings_.GetLength();++i)
            if(buildings_[i].BuildingCode() == building_code)
                return &buildings_[i];
        return nullptr;
    }

public:
    BuildingList() = default;

    explicit BuildingList(const Person& person){
        auto building = &buildings_.InsertElem(Building(person.BuildingNum()));
        building->AddPerson(person);
    }

    BuildingList(const BuildingList&) = delete;

    inline Person& AddPerson(const std::string &personal_code,
                             const Person::TestStatus status = Person::kNoTesting,
                             const Person::ContiguityStatus status1 = Person::kNoContiguity){
        auto building = FindBuilding(personal_code.substr(0,3));
        if(building == nullptr)
            building = &buildings_.InsertElem(Building(personal_code.substr(0,3)));
        return building->AddPerson(personal_code,status,status1);
    }

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

    void UpdateToConfirmed(Person& person){
        auto building = FindBuilding(person.BuildingNum());
        if(building != nullptr)
            building->UpdateToConfirmed(person);
    }

    inline void ShowTestStatus(const Person::TestStatus status,std::ostream& out){
        for(int i = 0; i < buildings_.GetLength(); ++i)
            buildings_[i].ShowTestStatus(status,out);
    }

    inline void ShowContiguityStatus(const Person::ContiguityStatus status,std::ostream& out){
        for(int i = 0; i < buildings_.GetLength(); ++i)
            buildings_[i].ShowContiguityStatus(status,out);
    }

    inline void FlushStatuses(){
        for(int i = 0;i < buildings_.GetLength();++i)
            buildings_[i].FlushStatus();
    }

};

#endif //NATSYSTEM_BUILDINGLIST_H
