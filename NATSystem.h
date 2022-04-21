//
// Created by Ï¡¹â on 2022/4/13.
//

#ifndef NATSYSTEM_NATSYSTEM_H
#define NATSYSTEM_NATSYSTEM_H

#include "buildingList.h"
#include "personQueue.h"
#include "tubeList.h"

class NATSystem{
private:
    BuildingList building_list;
    PersonQueue person_queue;
    TubeList tube_list;

public:
    NATSystem() = default;

    void QueueUp(const std::string& personal_code, bool is_single);

    TestTube * NATest(bool is_single);

    bool NATest(int people_num, bool is_single, std::ostream &out);

    bool RegisterTube(const std::string &tube_code, Person::TestStatus status = Person::kNegative);

    void ShowPersonalStatus(const std::string& personal_code,std::ostream& out);

    void ShowAllStatus(std::ostream& out);

    inline void ShowQueue(std::ostream& out){out << person_queue;}

    inline void ShowTubeList(std::ostream& out){out << tube_list;}
};



#endif //NATSYSTEM_NATSYSTEM_H
