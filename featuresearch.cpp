#include <iostream>
#include <vector>
#include <cstdlib>
#include <set>


float accuracy(const std::vector<std::vector<float>> &data, const std::set<int> &current_set, int feature_to_add){
  return (float)rand() / (float) RAND_MAX;
}

void feature_search_demo(const std::vector<std::vector<float>>& data){
  float acc;
  std::set<int> current_set;
  float max_accuracy = 0.0;
  int feature_to_add;
  for(int i = 0; i < data.size(); i++){
    std::cout<<"On the "<<i<<"th level of the search tree\n";
    for(int j = 1; j < data[i].size(); j++){
      if(current_set.find(j) == current_set.end()){
        std::cout<<"Considering adding the "<<j<<"th feature\n";
        acc = accuracy(data, current_set, j);
        if (acc > max_accuracy){
          max_accuracy = acc;
          feature_to_add = j;
        }
      }
    }
    current_set.insert(feature_to_add);
    std::cout<<"On level "<<i<<" I added feature "<<feature_to_add<<'\n';
  }
}

int main(){

}
