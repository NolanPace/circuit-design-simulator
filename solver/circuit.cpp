#include "circuit.hpp"
#include <sstream>
#include <set>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <vector>

Circuit parseNetlist(const std::string& text){
 Circuit c;std::istringstream in(text);std::string line;
 while(std::getline(in,line)){
  if(line.empty()||line[0]=='#')continue;
  std::istringstream row(line);char type;std::string name,extra;int a,b;double value;
  if(!(row>>type>>name>>a>>b>>value)||row>>extra||a<0||b<0||a==b||!std::isfinite(value))throw std::invalid_argument("Malformed element: "+line);
  if(type=='R'){if(value<=0)throw std::invalid_argument("Resistance must be >0");c.resistors.push_back({name,a,b,value});}
  else if(type=='V')c.sources.push_back({name,a,b,value});
  else throw std::invalid_argument("Unknown element type");
 }
 return c;
}
Solution Circuit::solve() const {
 std::set<int> nodes;
 for(const auto&r:resistors){if(r.a)nodes.insert(r.a);if(r.b)nodes.insert(r.b);}
 for(const auto&s:sources){if(s.positive)nodes.insert(s.positive);if(s.negative)nodes.insert(s.negative);}
 if(nodes.empty()|| (resistors.empty()&&sources.empty()))throw std::invalid_argument("Circuit has no non-ground nodes");
 std::vector<int> ordered(nodes.begin(),nodes.end());
 std::unordered_map<int,int> indices;for(size_t i=0;i<ordered.size();++i)indices[ordered[i]]=static_cast<int>(i);
 int n=static_cast<int>(ordered.size()),m=static_cast<int>(sources.size()),sz=n+m;
 std::vector<std::vector<double>> a(sz,std::vector<double>(sz+1,0));
 for(const auto&r:resistors){
  double g=1.0/r.ohms;
  if(r.a)a[indices.at(r.a)][indices.at(r.a)]+=g;
  if(r.b)a[indices.at(r.b)][indices.at(r.b)]+=g;
  if(r.a&&r.b){a[indices.at(r.a)][indices.at(r.b)]-=g;a[indices.at(r.b)][indices.at(r.a)]-=g;}
 }
 for(int j=0;j<m;++j){
  const auto&s=sources[j];
  if(s.positive){a[indices.at(s.positive)][n+j]+=1;a[n+j][indices.at(s.positive)]+=1;}
  if(s.negative){a[indices.at(s.negative)][n+j]-=1;a[n+j][indices.at(s.negative)]-=1;}
  a[n+j][sz]=s.volts;
 }
 for(int col=0;col<sz;++col){
  int best=col;for(int row=col+1;row<sz;++row)if(std::abs(a[row][col])>std::abs(a[best][col]))best=row;
  if(std::abs(a[best][col])<1e-12)throw std::runtime_error("Singular matrix: floating or inconsistent circuit");
  std::swap(a[col],a[best]);
  double pivot=a[col][col];
  for(int k=col;k<=sz;++k)a[col][k]/=pivot;
  for(int row=0;row<sz;++row){if(row==col)continue;double f=a[row][col];for(int k=col;k<=sz;++k)a[row][k]-=f*a[col][k];}
 }
 Solution result;result.voltages[0]=0;
 for(int i=0;i<n;++i)result.voltages[ordered[i]]=a[i][sz];
 for(const auto&r:resistors)result.currents[r.name]=(result.voltages[r.a]-result.voltages[r.b])/r.ohms;
 for(int j=0;j<m;++j)result.currents[sources[j].name]=a[n+j][sz];
 return result;
}
