#pragma once
#include <string>
#include <vector>
#include <unordered_map>
struct Resistor {std::string name; int a,b; double ohms;};
struct Source {std::string name; int positive,negative; double volts;};
struct Solution {std::unordered_map<int,double> voltages;std::unordered_map<std::string,double> currents;};
struct Circuit {
 std::vector<Resistor> resistors;
 std::vector<Source> sources;
 Solution solve() const;
};
Circuit parseNetlist(const std::string& text);
