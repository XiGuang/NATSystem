//
// Created by 稀光 on 2022/4/11.
//

#ifndef NATSYSTEM_PERSON_H
#define NATSYSTEM_PERSON_H

#include <string>
#include <stdexcept>
#include <utility>

class Person {
public:
    /** \brief kNegative: 阴性 ,kConfirmed:确诊 ,kSuspected:疑似 ,
     *         kToUpload: 等待上传结果 ,kQueuing:在排队 ,kNoTesting:未检测
     *         Size:枚举种类数
     *  \property 优先级：确诊，疑似，以此类推
     */
    enum TestStatus { kConfirmed = 0, kSuspected = 1, kNegative = 2,
            kToUpload = 3, kQueuing = 4, kNoTesting = 5, TestSize = 6};
    enum ContiguityStatus { kContiguity = 0, kSecContiguity = 1, kNoContiguity = 2, ContiguitySize = 3};
    struct Status{  // 用于同时存储与传递两个状态
        TestStatus test_status;
        ContiguityStatus contiguity_status;

        Status(TestStatus test,ContiguityStatus contiguity):test_status(test),contiguity_status(contiguity){}

        Status():test_status(kNoTesting),contiguity_status(kNoContiguity){}
    };

    explicit Person(std::string personal_code,const TestStatus personal_test_status = kNoTesting,
                    const ContiguityStatus personal_contiguity_status = kNoContiguity):
                            personal_code_(std::move(personal_code)),
                            personal_status_(){
        if(personal_code_.length() != 8) throw std::length_error("不是八位数字！");
        for(const auto& code:personal_code_)
            if(!std::isalnum(code))
                throw std::range_error("不是全数字的人员代码!");
        UpdateStatus(personal_test_status,personal_contiguity_status);
    }

    Person():personal_status_(){}

    Person(const Person& person):personal_status_(person.PersonalStatus()),personal_code_(person.personal_code_){}

    Person& operator=(const Person&) = default;
    // 更新检测状态
    inline void UpdateTestStatus(const TestStatus& status){ // 按照优先级更新状态
        if(status < personal_status_.test_status) // 若status优先级低，则不更改
            personal_status_.test_status = status;
    }
    // 更新密接状态
    inline void UpdateContiguityStatus(const ContiguityStatus status){ // 按照优先级更新状态
        if(status < personal_status_.contiguity_status)
            personal_status_.contiguity_status = status;
    }
    // 更新状态
    inline void UpdateStatus(const TestStatus test,const ContiguityStatus contiguity){
        UpdateTestStatus(test);
        UpdateContiguityStatus(contiguity);
    }
    // 获取人当前状态
    inline const Status &PersonalStatus() const {return personal_status_;}
    // 获取人当前检测状态
    inline const TestStatus &PersonalTestStatus() const {return personal_status_.test_status;}
    // 获取人当前密接状态
    inline const ContiguityStatus &PersonalContiguityStatus() const {return personal_status_.contiguity_status;}
    // 人员代码
    inline const std::string &PersonalCode() const {return personal_code_;}
    // 楼栋号
    inline std::string BuildingNum() const {return personal_code_.substr(0, 3);}
    // 房间号
    __attribute__((unused)) inline std::string RoomNum() const {return personal_code_.substr(3,7);}
    // 人员号
    __attribute__((unused)) inline std::string PersonNum() const {return personal_code_.substr(7);}

private:
    Status personal_status_;    // 个人状态
    std::string personal_code_; // 人员代码


};


#endif //NATSYSTEM_PERSON_H
