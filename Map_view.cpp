#include "Map_view.h"
#include "Utility.h"

#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

// consts for map views
const string empty_placeholder = ". ";
const int default_size_c = 25;
const double default_scale_c = 2.;
const double default_origin_c = -10.;
const int max_size_c = 30; // size <= 30
const int min_size_c = 6;  // size > 6
// consts to help plot the map
const int output_y_label_w_c = 4;
const int output_x_label_w_c = 6;
const int label_gap = 3;


Map_view::Map_view(): size{default_size_c}, scale{default_scale_c},
origin{default_origin_c, default_origin_c}{
}

void Map_view::set_size(int size_){
    if (size_ <= min_size_c) {
        throw Error("New map size is too small!");
    }
    if(size_ > max_size_c){
        throw Error("New map size is too big!");
    }
    size = size_;
}

void Map_view::set_scale(double scale_){
    if (scale_ <= 0.) {
        throw Error("New map scale must be positive!");
    }
    scale = scale_;
}

void Map_view::set_origin(Point origin_){
    origin = origin_;
}

void Map_view::set_defaults(){
    size = default_size_c;
    scale = default_scale_c;
    origin = Point{default_origin_c, default_origin_c};
}

void Map_view::draw(){
    cout <<  "Display size: " << size << ", scale: " << scale <<
    ", origin: " << origin << endl;
    Grid_view::draw();
}

vector<Grid_view::Obj_index> Map_view::get_indexes(const std::map<std::string, Point>& plot_objectes){
    vector<Grid_view::Obj_index> obj_idx;
    int idx,idy;
    bool some_obj_out_of_map = false;
    for (auto &plot_pair: plot_objectes) {
        if (!Grid_view::get_subscripts(idx, idy, plot_pair.second,
                                        size, scale, origin)){
            if (some_obj_out_of_map) {
                cout << ", " << plot_pair.first;
            } else {
                cout << plot_pair.first;
                some_obj_out_of_map = true;
            }
        } else {
            obj_idx.push_back(Grid_view::Obj_index{
                plot_pair.first, idx, idy
            });
        }
    }
    if (some_obj_out_of_map) {
        cout << " outside the map" << endl;
    }
    return obj_idx;
}

string Map_view::get_placeholder(){
    return empty_placeholder;
}

int    Map_view::get_grid_size_x(){
    return size;
}

int    Map_view::get_grid_size_y(){
    return size;
}

void Map_view::print_y_label(int line){
    Cout_saver raii_saver{};
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(0);
    if ((size - line)%label_gap == 1) {
        double y_label = origin.y +
        scale * label_gap * ((size - line - 1) / label_gap);
        cout << setw(output_y_label_w_c) << y_label << " ";
    } else {
        cout << "     ";
    }
}

void Map_view::print_x_label(){
    Cout_saver raii_saver{};
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(0);
    int label_count = size % label_gap ? size / label_gap + 1 :
                      size / label_gap;
    double x_label = origin.x;
    for (int x = 0; x < label_count; x++) {
        cout << setw(output_x_label_w_c) << x_label;
        x_label += scale * label_gap;
    }
    cout << endl;
}
