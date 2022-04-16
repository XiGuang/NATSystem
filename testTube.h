//
// Created by ϡ�� on 2022/4/11.
//

#ifndef NATSYSTEM_TESTTUBE_H
#define NATSYSTEM_TESTTUBE_H

#include <string>
#include <vector>
#include "person.h"

class TestTube {
protected:
    Person** individuals_;   // ��Ӧ��Ա
    std::string tube_code_; // �Թܱ��
    int individuals_num_;   // Ŀǰ������Ա��
    bool registered_{false};

public:
    explicit TestTube(Person& person,bool is_single = true):individuals_(nullptr),individuals_num_(1){
        static int single_num(0),hybrid_num(0);
        if(is_single){
            tube_code_ = std::to_string(single_num++);
            tube_code_ = std::string("000").substr(0,4 - tube_code_.length()) + tube_code_;
            tube_code_ = '1' + tube_code_;
            individuals_ = new Person*[1];
        }else{
            tube_code_ = std::to_string(hybrid_num++);
            tube_code_ = std::string("000").substr(0,4 - tube_code_.length()) + tube_code_;
            tube_code_ = '0' + tube_code_;
            individuals_ = new Person*[10];
        }
        individuals_[0] = &person;
    }

    ~TestTube(){
        delete[] individuals_;
    }

    TestTube():individuals_(nullptr),individuals_num_(0){}

    TestTube(const TestTube&test_tube):individuals_(nullptr),individuals_num_(0){*this = test_tube;};

    TestTube& operator=(const TestTube& test_tube) {
        if(this == &test_tube) return *this;
        individuals_num_ = test_tube.individuals_num_;
        individuals_ = new Person*[individuals_num_];
        for(int i = 0;i < individuals_num_;++i)
            individuals_[i] = test_tube.individuals_[i];
        tube_code_ = test_tube.tube_code_;
        return *this;
    }

    bool IsSingle() const{return tube_code_[0] == '1';}    // �Ƿ��˲��Թ�

    bool IsRegistered() const{return registered_;}  // �Ƿ��Ѿ��Ǽǹ�

    void SetRegistered() {registered_ = true;}  // �����Թ�Ϊ�Ǽǹ�

    inline const std::string& TubeCode() const {return tube_code_;}

    inline Person** Individuals() const {return individuals_;}

    // Ŀǰ�Թ�����
    inline auto PersonNum() const {return individuals_num_;}

    // ��ˮ��
    __attribute__((unused)) inline std::string SerialNum() const {return tube_code_.substr(1);}

    bool AddPerson(Person& person){
        if(IsSingle() || individuals_num_ > 9)   // ���˹ܻ�����ʮ�˲��������
            return false;
        individuals_[individuals_num_++] = &person;
        return true;
    }

};


#endif //NATSYSTEM_TESTTUBE_H
