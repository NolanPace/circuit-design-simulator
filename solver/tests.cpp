#include "circuit.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
int main(){
 auto c=parseNetlist("V V1 1 0 10\nR R1 1 2 1000\nR R2 2 0 1000\n");
 auto r=c.solve();
 assert(std::abs(r.voltages.at(1)-10)<1e-8);
 assert(std::abs(r.voltages.at(2)-5)<1e-8);
 assert(std::abs(r.currents.at("R1")-.005)<1e-8);
 bool failed=false;
 try{parseNetlist("R bad 1 0 -1").solve();}catch(...){failed=true;}
 assert(failed);
 std::cout<<"Circuit tests passed\n";
}