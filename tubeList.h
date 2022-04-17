//
// Created by ϡ�� on 2022/4/12.
//

#ifndef NATSYSTEM_TUBELIST_H
#define NATSYSTEM_TUBELIST_H

#include "testTube.h"
#include "LinkList/LinkList.h"

// �Թܶ���
class TubeList{
private:
    LinkList<TestTube> tubes_;

public:
    TubeList() = default;

    inline TestTube& AddTube(bool is_single = true){
        return tubes_.InsertElem(TestTube(is_single));
    }

    inline TestTube* FindTube(const std::string &tube_code) const {
        for(int i = tubes_.GetLength() - 1;i >= 0;--i)      // ����ƥ�����µ��Թ�
            if(tubes_[i].TubeCode() == tube_code)
                return &tubes_[i];
        return nullptr;
    }

    friend std::ostream& operator<<(std::ostream& out,const TubeList& tube_list){
        for(int i = 0; i < tube_list.tubes_.GetLength(); ++i)
            out << tube_list.tubes_[i] << std::endl;
        return out;
    }
};

#endif //NATSYSTEM_TUBELIST_H
