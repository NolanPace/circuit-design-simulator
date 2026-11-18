#include "circuit.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
int main(int argc,char**argv){
 if(argc!=2){std::cerr<<"Usage: circuit_solver <netlist.txt>\n";return 2;}
 try{
  std::ifstream in(argv[1]);if(!in)throw std::runtime_error("Cannot open file");
  std::ostringstream buf;buf<<in.rdbuf();
  Solution r=parseNetlist(buf.str()).solve();std::cout<<std::setprecision(9);
  for(auto [node,v]:r.voltages)std::cout<<"V("<<node<<") = "<<v<<" V\n";
  for(auto [name,i]:r.currents)std::cout<<"I("<<name<<") = "<<i<<" A\n";
 }catch(const std::exception&e){std::cerr<<"Error: "<<e.what()<<"\n";return 1;}
}