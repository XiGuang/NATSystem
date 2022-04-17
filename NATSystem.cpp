//
// Created by ϡ�� on 2022/4/13.
//

#include "NATSystem.h"

void NATSystem::QueueUp(const string &personal_code, bool is_single) {
    if(building_list.FindPerson(personal_code))
        throw std::invalid_argument("��Ա�����Ŷӹ���");
    person_queue.QueueUp(building_list.AddPerson(personal_code), is_single);
}

bool NATSystem::RegisterTube(const std::string &tube_code, Person::TestStatus status){
    auto test_tube = tube_list.FindTube(tube_code);
    if(test_tube == nullptr) return false; // �Ҳ����Թ�
    if(!test_tube->IsSingle() && test_tube->PersonNum() != 10)  return false;   // ����ʮ�˲��ܼ��
    if(test_tube->IsRegistered()) return false; // ���Թ��Ѿ��Ǽǹ�����ֹ
    if(status != Person::kNegative && status != Person::kConfirmed && status != Person::kSuspected)
        throw std::range_error("���������ʹ���");
    if(status == Person::kNegative)
        for(int i = 0;i < test_tube->PersonNum();++i)
            test_tube->Individuals()[i]->UpdateTestStatus(status);
    else if(!test_tube->IsSingle() || status == Person::kSuspected){ // �����������Ի��ǿ��ɣ��������ԱΪ����
        for(int i = 0;i < test_tube->PersonNum();++i)
            test_tube->Individuals()[i]->UpdateTestStatus(Person::kSuspected);
    }else{  // �������Ե����
        building_list.UpdateToConfirmed(*test_tube->Individuals()[0]);  // ��������,ͬʱͬ¥Ϊ�ܽ�
        for(int i = -10;true;++i){  // Ѱ��ǰ��ʮ���˺ͺ��������������Ϊ�ܽ�
            if(i == 0) continue;
            int tube_num(std::strtol(test_tube->TubeCode().c_str(), nullptr,10) + i);
            if(tube_num >= 0){
                auto contiguity_tube = tube_list.FindTube(std::to_string(tube_num));
                if(contiguity_tube){   // ȷ�����ǿ�ָ��
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

bool NATSystem::NATest(bool is_single){    // ������
    if(is_single && !person_queue.single_queue_.empty()){
        std::string test_personal_code;  // ȡ��ͷ
        person_queue.single_queue_.pop(test_personal_code);
        auto person = building_list.FindPerson(test_personal_code);
        if(person != nullptr){
            tube_list.AddTube(true).AddPerson(*person);
            person->UpdateTestStatus(Person::kToUpload);
        }
        return true;
    }else if(!is_single && !person_queue.hybrid_queue_.empty()){
        std::string test_personal_code;
        person_queue.hybrid_queue_.pop(test_personal_code);
        auto person = building_list.FindPerson(test_personal_code);
        if(person == nullptr)
            throw std::bad_alloc();
        static TestTube* tube(nullptr);  // �����´λ������Թ�
        if(tube == nullptr || tube->PersonNum() >= 10)   // ����Թܲ����ڻ����������ټ����Թ�
            tube = &tube_list.AddTube(false);
        tube->AddPerson(*person);
        person->UpdateTestStatus(Person::kToUpload);
        return true;
    }
    return false;
}

bool NATSystem::NATest(int people_num, bool is_single) {
    if(is_single) {
        if (people_num > person_queue.LengthOfSingleQueue())
            return false;
    }else {
        if (people_num > person_queue.LengthOfHybridQueue())
            return false;
    }
    for(int i = 0;i < people_num;++i)
        NATest(is_single);
    return true;
}

void NATSystem::ShowPersonalStatus(const std::string& personal_code,std::ostream& out){
    auto person = building_list.FindPerson(personal_code);
    if(person == nullptr){
        out << " δ��� " << std::endl;
        return;
    }
    switch (person->PersonalStatus().test_status) {
        case Person::kConfirmed:
            out << " ȷ�� ";
            break;
        case Person::kSuspected:
            out << " ���� ";
            break;
        case Person::kNegative:
            out << " ���� ";
            break;
        case Person::kToUpload:
            out << " ���ϴ���� ";
            break;
        case Person::kQueuing:
            out << " �Ŷ��� ";
            break;
        case Person::kNoTesting:
        case Person::TestSize:
            out << "        ";
            break;
    }
    if(person->PersonalStatus().test_status != Person::kConfirmed){
        switch (person->PersonalStatus().contiguity_status) {
            case Person::kContiguity:
                out << " �ܽ� " << std::endl;
                break;
            case Person::kSecContiguity:
                out << " ���ܽ� " << std::endl;
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
    out << "   ����: ";
    building_list.ShowTestStatus(Person::kConfirmed, out);
    out << std::endl;
    out << "   ����: ";
    building_list.ShowTestStatus(Person::kSuspected, out);
    out << std::endl;
    out << "   �ܽ�: ";
    building_list.ShowContiguityStatus(Person::kContiguity, out);
    out << std::endl;
    out << "   ���ܽ�: ";
    building_list.ShowContiguityStatus(Person::kSecContiguity, out);
    out << std::endl;
    out << "   ����: ";
    building_list.ShowTestStatus(Person::kNegative, out);
    out << std::endl;
    out << "   ���ϴ�: ";
    building_list.ShowTestStatus(Person::kToUpload, out);
    out << std::endl;
    out << "   �Ŷ���: ";
    building_list.ShowTestStatus(Person::kQueuing, out);
    out << std::endl;
}
