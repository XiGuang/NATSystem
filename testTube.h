//
// Created by 稀光 on 2022/4/11.
//

#ifndef NATSYSTEM_TESTTUBE_H
#define NATSYSTEM_TESTTUBE_H

#include <string>
#include <vector>
#include "person.h"

class TestTube {
protected:
    Person** individuals_;   // 对应人员
    std::string tube_code_; // 试管编号
    int individuals_num_;   // 目前存入人员数
    bool registered_{false};    // 标记试管是否已经登记过

public:
    explicit TestTube(bool is_single):individuals_(nullptr),individuals_num_(0){
        static int single_num(0),hybrid_num(0);
        if(is_single){
            if(single_num > 9999) throw std::out_of_range("流水号超范围!");
            tube_code_ = std::to_string(single_num++);
            // 补零
            tube_code_ = std::string("000").substr(0,4 - tube_code_.length()) + tube_code_;
            tube_code_ = '1' + tube_code_;  //单人
            individuals_ = new Person*[1];
        }else{
            if(hybrid_num > 9999) throw std::out_of_range("流水号超范围!");
            tube_code_ = std::to_string(hybrid_num++);
            tube_code_ = std::string("000").substr(0,4 - tube_code_.length()) + tube_code_;
            tube_code_ = '0' + tube_code_;
            individuals_ = new Person*[10];
        }
    }

    ~TestTube(){
        delete[] individuals_;
    }

    TestTube():individuals_(nullptr),individuals_num_(0){}

    TestTube(const TestTube&test_tube):individuals_(nullptr),individuals_num_(0){*this = test_tube;};

    TestTube& operator=(const TestTube& test_tube) {
        if(this == &test_tube) return *this;
        individuals_num_ = test_tube.individuals_num_;
        individuals_ = new Person*[(test_tube.IsSingle() ? 1 : 10)];
        for(int i = 0;i < individuals_num_;++i)
            individuals_[i] = test_tube.individuals_[i];
        tube_code_ = test_tube.tube_code_;
        return *this;
    }

    bool IsSingle() const{return tube_code_[0] == '1';}    // 是否单人测试管

    bool IsRegistered() const{return registered_;}  // 是否已经登记过

    void SetRegistered() {registered_ = true;}  // 设置试管为登记过
    // 试管号
    inline const std::string& TubeCode() const {return tube_code_;}
    // 返回试管中人员
    inline Person** Individuals() const {return individuals_;}
    // 目前试管人数
    inline auto PersonNum() const {return individuals_num_;}
    // 流水号
    __attribute__((unused)) inline std::string SerialNum() const {return tube_code_.substr(1);}
    // 添加人的指针
    bool AddPerson(Person& person){
        if((IsSingle() && individuals_num_ > 0) || individuals_num_ > 9)   // 单人管或已满十人不允许添加
            return false;
        individuals_[individuals_num_++] = &person;
        return true;
    }

    friend std::ostream& operator<<(std::ostream& out,const TestTube& tube){
        for(int i = 0;i < tube.individuals_num_;++i)
            out << tube.individuals_[i]->PersonalCode() << "  ";
        out << "的试管编号是 " << tube.tube_code_;
        return out;
    }
};


#endif //NATSYSTEM_TESTTUBE_H
