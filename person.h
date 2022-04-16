//
// Created by ϡ�� on 2022/4/11.
//

#ifndef NATSYSTEM_PERSON_H
#define NATSYSTEM_PERSON_H

#include <string>
#include <stdexcept>
#include <utility>

class Person {
public:
    /** \brief kNegative: ���� ,kConfirmed:ȷ�� ,kSuspected:���� ,
     *         kToUpload: �ȴ��ϴ���� ,kQueuing:���Ŷ� ,kNoTesting:δ���
     *         Size:ö��������
     *  \property ���ȼ���ȷ����ƣ��Դ�����
     */
    enum TestStatus { kConfirmed = 0, kSuspected = 1, kNegative = 2,
            kToUpload = 3, kQueuing = 4, kNoTesting = 5, TestSize = 6};
    enum ContiguityStatus { kContiguity = 0, kSecContiguity = 1, kNoContiguity = 2, ContiguitySize = 3};
    struct Status{
        TestStatus test_status;
        ContiguityStatus contiguity_status;

        Status(TestStatus test,ContiguityStatus contiguity):test_status(test),contiguity_status(contiguity){}

        Status():test_status(kNoTesting),contiguity_status(kNoContiguity){}
    };

    explicit Person(std::string personal_code,const TestStatus personal_test_status = kNoTesting,
                    const ContiguityStatus personal_contiguity_status = kNoContiguity):
                            personal_code_(std::move(personal_code)),
                            personal_status_(){
        if(personal_code_.length() != 8) throw std::length_error("���ǰ�λ���֣�");
        for(const auto& code:personal_code_)
            if(!std::isalnum(code))
                throw std::range_error("����ȫ���ֵ���Ա����!");
        UpdateStatus(personal_test_status,personal_contiguity_status);
    }

    Person():personal_status_(){}

    Person(const Person&) = default;

    Person& operator=(const Person&) = default;

    inline void UpdateTestStatus(const TestStatus& status){ // �������ȼ�����״̬
        if(status < personal_status_.test_status)
            personal_status_.test_status = status;
    }

    inline void UpdateContiguityStatus(const ContiguityStatus status){ // �������ȼ�����״̬
        if(status < personal_status_.contiguity_status)
            personal_status_.contiguity_status = status;
    }

    inline void UpdateStatus(const TestStatus test,const ContiguityStatus contiguity){
        UpdateTestStatus(test);
        UpdateContiguityStatus(contiguity);
    }

    inline const Status &PersonalStatus() const {return personal_status_;}

    inline const TestStatus &PersonalTestStatus() const {return personal_status_.test_status;}

    inline const ContiguityStatus &PersonalContiguityStatus() const {return personal_status_.contiguity_status;}

    inline const std::string &PersonalCode() const {return personal_code_;}

    // ¥����
    inline std::string BuildingNum() const {return personal_code_.substr(0, 3);}

    // �����
    inline std::string RoomNum() const {return personal_code_.substr(3,7);}

    // ��Ա��
    inline std::string PersonNum() const {return personal_code_.substr(7);}

private:
    Status personal_status_;
    std::string personal_code_;
};


#endif //NATSYSTEM_PERSON_H