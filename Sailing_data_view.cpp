#include "Sailing_data_view.h"

#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

const int sailing_data_width_c = 10;
void Sailing_data_view::update_ship_speed(const string& name, double speed){
    data[name].speed = speed;
}

void Sailing_data_view::update_ship_course(const string& name, double course){
    data[name].course = course;
}

void Sailing_data_view::update_ship_fuel(const string& name, double fuel){
    data[name].fuel = fuel;
}

void Sailing_data_view::update_remove(const string& name){
    data.erase(name);
}

void Sailing_data_view::draw(){
    cout << "----- Sailing Data -----" << endl;
    cout << setw(10) << "Ship" << setw(10) << "Fuel" <<  setw(10) <<"Course"
    << setw(10) << "Speed" << endl;
    for (auto& data_pair: data) {
        cout << setw(sailing_data_width_c) << data_pair.first
             << setw(sailing_data_width_c) << data_pair.second.fuel
             << setw(sailing_data_width_c) << data_pair.second.course
             << setw(10) << data_pair.second.speed << endl;
    }
}

void Sailing_data_view::clear(){
    data.clear();
}

