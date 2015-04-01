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
#include <map>
#include <set>
#include <iterator>
#include <algorithm>
using namespace std;
using namespace std::placeholders;
int main(){
    map<string, int> m{
        {"1",1},
        {"2",2},
        {"3",3},
        {"4",5},
        {"5",6},
    };
    set<int> value_set;
    
    transform(m.begin(), m.end(),
              insert_iterator<set<int>>{value_set, value_set.begin()},
              bind(&map<string, int>::value_type::second, _1));
    copy(value_set.begin(), value_set.end(), ostream_iterator<int>(cout));
}
