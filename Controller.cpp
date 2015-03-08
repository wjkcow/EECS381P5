#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "Ship.h"
#include "Island.h"
#include "Geometry.h"
#include "Ship_factory.h"

#include <iostream>
#include <map>
#include <functional>
using namespace std;
using namespace placeholders;

Controller::Controller(){
    cout << "Controller constructed" << endl;
}

Controller::~Controller(){
    cout << "Controller destructed" << endl;
}

void Controller::run(){
    View* view = new View{};
    g_Model_ptr->attach(view);
    string cmd_word;
    map<string, function<void(void)>> view_model_cmds = {
        {"default", bind(&Controller::view_cmd_default, this)},
        {"size",    bind(&Controller::view_cmd_default, this)},
        {"zoom",    bind(&Controller::view_cmd_default, this)},
        {"pan",     bind(&Controller::view_cmd_default, this)},
        {"show",    bind(&Controller::view_cmd_default, this)},
        {"status",  bind(&Controller::view_cmd_default, this)},
        {"go",      bind(&Controller::view_cmd_default, this)},
        {"create",  bind(&Controller::view_cmd_default, this)}
    };
    map<string, function<void(Ship*)>> ship_cmds = {
        {"course",      bind(&Controller::ship_course, this, _1)},
        {"position",    bind(&Controller::ship_position, this, _1)},
        {"destination", bind(&Controller::ship_destination, this, _1)},
        {"load_at",     bind(&Controller::ship_load_at, this, _1)},
        {"unload_at",   bind(&Controller::ship_unload_at, this, _1)},
        {"dock_at",     bind(&Controller::ship_dock_at, this, _1)},
        {"attack",      bind(&Controller::ship_attack, this, _1)},
        {"refuel",      bind(&Controller::ship_refuel, this, _1)},
        {"stop",        bind(&Controller::ship_stop, this, _1)},
        {"stop_attack", bind(&Controller::ship_stop_attack, this, _1)}
    };
    
    while (true) {
        cout << "\nTime " << g_Model_ptr->get_time() << ": Enter command: ";
        cin >> cmd_word;
        if (cmd_word == "quit") {
            g_Model_ptr->detach(view);
            delete view;
            cout << "Done" << endl;
            return;
        }
        if (g_Model_ptr->is_ship_present(cmd_word)) {
            
        } else {
            
        }
    }
}

