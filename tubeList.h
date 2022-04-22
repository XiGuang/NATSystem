//
// Created by 稀光 on 2022/4/12.
//

#ifndef NATSYSTEM_TUBELIST_H
#define NATSYSTEM_TUBELIST_H

#include "testTube.h"
#include "LinkList/LinkList.h"

// 试管链表
class TubeList{
private:
    LinkList<TestTube> tubes_;

public:
    TubeList() = default;
    // 添加试管
    inline TestTube& AddTube(bool is_single = true){
        return tubes_.InsertElem(TestTube(is_single));
    }
    // 以试管号查找试管
    inline TestTube* FindTube(const std::string &tube_code) const {
        for(int i = tubes_.GetLength() - 1;i >= 0;--i)      // 优先匹配最新的试管
            if(tubes_[i].TubeCode() == tube_code)
                return &tubes_[i];
        return nullptr;
    }
};

#endif //NATSYSTEM_TUBELIST_H
