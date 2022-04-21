//
// Created by 稀光 on 2022/4/13.
//

#include "NATSystem.h"

void NATSystem::QueueUp(const string &personal_code, bool is_single) {
    if(building_list.FindPerson(personal_code))
        throw std::invalid_argument("人员码已排队过！");
    person_queue.QueueUp(building_list.AddPerson(personal_code), is_single);
}

bool NATSystem::RegisterTube(const std::string &tube_code, Person::TestStatus status){
    auto test_tube = tube_list.FindTube(tube_code);
    if(test_tube == nullptr) return false; // 找不到试管
    if(!test_tube->IsSingle() && test_tube->PersonNum() != 10)  return false;   // 不满十人不能检测
    if(test_tube->IsRegistered()) return false; // 若试管已经登记过则阻止
    if(status != Person::kNegative && status != Person::kConfirmed && status != Person::kSuspected)
        throw std::range_error("输入检测类型错误！");
    if(status == Person::kNegative)
        for(int i = 0;i < test_tube->PersonNum();++i)
            test_tube->Individuals()[i]->UpdateTestStatus(status);
    else if(!test_tube->IsSingle() || status == Person::kSuspected){ // 混检管无论阳性还是可疑，都标记人员为可疑
        for(int i = 0;i < test_tube->PersonNum();++i)
            test_tube->Individuals()[i]->UpdateTestStatus(Person::kSuspected);
    }else{  // 单人阳性的情况
        building_list.UpdateToConfirmed(*test_tube->Individuals()[0]);  // 设置阳性,同时同楼为密接
        for(int i = -10;true;++i){  // 寻找前面十个人和后面的所有人设置为密接
            if(i == 0) continue;
            int tube_num(std::strtol(test_tube->TubeCode().c_str(), nullptr,10) + i);
            if(tube_num >= 0){
                auto contiguity_tube = tube_list.FindTube(std::to_string(tube_num));
                if(contiguity_tube){   // 确保不是空指针
                    building_list.UpdateToContiguity(*contiguity_tube->Individuals()[0]);
                }else
                    break;
            }
        }
        person_queue.SetConfirmed(building_list);
    }
    test_tube->SetRegistered();
    return true;
}

TestTube * NATSystem::NATest(bool is_single){    // 核酸检测
    if(is_single && !person_queue.single_queue_.empty()){
        std::string test_personal_code;
        person_queue.single_queue_.pop(test_personal_code); // 取队头
        auto person = building_list.FindPerson(test_personal_code);
        if(person != nullptr){  // 能找到人，则将人的指针加入试管并标记人为待上传状态
            person->UpdateTestStatus(Person::kToUpload);
            auto &tube_refer = tube_list.AddTube(true);
            tube_refer.AddPerson(*person);
            return &tube_refer;
        }
    }else if(!is_single && !person_queue.hybrid_queue_.empty()){
        std::string test_personal_code;
        person_queue.hybrid_queue_.pop(test_personal_code);
        auto person = building_list.FindPerson(test_personal_code);
        if(person == nullptr)
            throw std::bad_alloc();
        static TestTube* tube_pointer(nullptr);  // 方便下次混检加入试管
        if(tube_pointer == nullptr || tube_pointer->PersonNum() >= 10)   // 如果试管不存在或者满了则再加入新试管
            tube_pointer = &tube_list.AddTube(false);
        tube_pointer->AddPerson(*person);
        person->UpdateTestStatus(Person::kToUpload);
        return tube_pointer;
    }
    return nullptr;
}

bool NATSystem::NATest(int people_num, bool is_single, std::ostream &out) {
    if((is_single && people_num > person_queue.LengthOfSingleQueue())
            || (!is_single && people_num > person_queue.LengthOfHybridQueue()))
        throw std::out_of_range("欲检测人数超出排队人数");
    TestTube *tube_point(nullptr);
    for(int i = 0;i < people_num;++i) {
        tube_point = NATest(is_single);
        if(tube_point == nullptr) return false;
        if(is_single || tube_point->PersonNum() == 10) // 单人或混检满十个输出
            out << *tube_point << std::endl;
    }
    if(tube_point != nullptr && !is_single && tube_point->PersonNum() < 10) // 输出混检最后不满十人的部分
        out << *tube_point << std::endl;
    return true;
}

void NATSystem::ShowPersonalStatus(const std::string& personal_code,std::ostream& out){
    auto person = building_list.FindPerson(personal_code);
    if(person == nullptr){
        out << " 未检测 " << std::endl;
        return;
    }
    switch (person->PersonalTestStatus()) { // 检测状态输出
        case Person::kConfirmed:
            out << " 确诊 ";
            break;
        case Person::kSuspected:
            out << " 疑似 ";
            break;
        case Person::kNegative:
            out << " 阴性 ";
            break;
        case Person::kToUpload:
            out << " 待上传结果 ";
            break;
        case Person::kQueuing:
            out << " 排队中 ";
            break;
        case Person::kNoTesting:
        case Person::TestSize:
            out << "        ";
            break;
    }
    if(person->PersonalStatus().test_status != Person::kConfirmed){ // 阳性时不输出密接状态
        switch (person->PersonalContiguityStatus()) { // 密接状态输出
            case Person::kContiguity:
                out << " 密接 " << std::endl;
                break;
            case Person::kSecContiguity:
                out << " 次密接 " << std::endl;
                break;
            case Person::kNoContiguity:
            case Person::ContiguitySize:
                out << std::endl;
                break;
        }
    }
}

void NATSystem::ShowAllStatus(ostream &out) {
    building_list.FlushStatuses();
    out << "   阳性: " << std::endl;
    building_list.ShowTestStatus(Person::kConfirmed, out);
    out << std::endl;
    out << "   可疑: " << std::endl;
    building_list.ShowTestStatus(Person::kSuspected, out);
    out << std::endl;
    out << "   密接: " << std::endl;
    building_list.ShowContiguityStatus(Person::kContiguity, out);
    out << std::endl;
    out << "   次密接: " << std::endl;
    building_list.ShowContiguityStatus(Person::kSecContiguity, out);
    out << std::endl;
    out << "   阴性: " << std::endl;
    building_list.ShowTestStatus(Person::kNegative, out);
    out << std::endl;
    out << "   待上传: " << std::endl;
    building_list.ShowTestStatus(Person::kToUpload, out);
    out << std::endl;
    out << "   排队中: " << std::endl;
    building_list.ShowTestStatus(Person::kQueuing, out);
    out << std::endl;
}
