//
//  test_main.cpp
//  EECS381P4
//
//  Created by wjkcow on 3/6/15.
//  Copyright (c) 2015 wjkcow. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <memory>
using namespace std;
class c{
    virtual void v() = 0;
};
int main(){
//    cout.setf(ios::fixed, ios::floatfield); // set fixed floating format
//    cout.precision(2);
    cout.precision(0);
    int a = 10.1;
    shared_ptr<c> sp;
    sp.reset();
    cout   << setw(10)<<"abc"<< a << endl;
}
