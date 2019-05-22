#pragma once
#include<iostream>
#include<vector>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
#include<boost/foreach.hpp>
#include"TransmissionRC.h"
namespace TransmissionRC{


bool authenticate();
std::vector<TransmissionRC::rcTorrent> * getTorrents();
bool resumeTorrent(int id);
bool stopTorrent(int id);
bool addTorrent(std::string);


}
