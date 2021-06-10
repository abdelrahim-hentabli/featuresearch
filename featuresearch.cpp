#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <time.h>
#include <math.h>
#include <cstring>
#include <ctime>

float accuracy_add(const std::vector<std::vector<float>> &data, const std::set<int> &current_set, int feature_to_add){
  double running_total;
  double** shadow = new double*[data.size()];
  for(int i = 0; i < data.size(); i++){
    shadow[i] = new double[data.size()];
  }
  for(int i = 0; i < data.size(); i++){
    for(int j = i+1; j < data.size(); j++){
      running_total = 0;
      for(auto it = current_set.begin(); it != current_set.end(); it++){
        running_total += pow((data[i][*it] - data[j][*it]),2);
      }
      running_total += pow((data[i][feature_to_add] - data[j][feature_to_add]),2);
      shadow[i][j] = shadow[j][i] = sqrt(running_total);
    }
  }
  int currentNearest;
  int correct = 0;
  for(int i = 0; i < data.size(); i++){
    currentNearest = (i==0)?1:0;
    for(int j = currentNearest+1; j < data.size(); j++){
      if(i != j){
        if( shadow[i][currentNearest] > shadow[i][j]){
          currentNearest = j;
        }
      }
    }
    if(data[i][0] == data[currentNearest][0]){
      correct++;
    }
  }
  for(int i = 0; i < data.size(); i++){
    delete[] shadow[i];
  }
  delete[] shadow;
  return float(correct)/float(data.size());
}

float accuracy_remove(const std::vector<std::vector<float>> &data, const std::set<int> &current_set, int feature_to_remove){
  double running_total;
  double** shadow = new double*[data.size()];
  for(int i = 0; i < data.size(); i++){
    shadow[i] = new double[data.size()];
  }
  for(int i = 0; i < data.size(); i++){
    for(int j = i+1; j < data.size(); j++){
      running_total = 0;
      for(auto it = current_set.begin(); it != current_set.end(); it++){
        if(*it != feature_to_remove){
          running_total += pow((data[i][*it] - data[j][*it]),2);
        }
      }
      shadow[i][j] = shadow[j][i] = sqrt(running_total);
    }
  }
  int currentNearest;
  int correct = 0;
  for(int i = 0; i < data.size(); i++){
    currentNearest = (i==0)?1:0;
    for(int j = currentNearest+1; j < data.size(); j++){
      if(i != j){
        if( shadow[i][currentNearest] > shadow[i][j]){
          currentNearest = j;
        }
      }
    }
    if(data[i][0] == data[currentNearest][0]){
      correct++;
    }
  }
  for(int i = 0; i < data.size(); i++){
    delete[] shadow[i];
  }
  delete[] shadow;
  return float(correct)/float(data.size());
}

float accuracy_add_pruning(const std::vector<std::vector<float>> &data, const std::set<int> &current_set, int feature_to_add, int correct_number){
  double running_total;
  double** shadow = new double*[data.size()];
  for(int i = 0; i < data.size(); i++){
    shadow[i] = new double[data.size()];
  }
  for(int i = 0; i < data.size(); i++){
    for(int j = i+1; j < data.size(); j++){
      running_total = 0;
      for(auto it = current_set.begin(); it != current_set.end(); it++){
        running_total += pow((data[i][*it] - data[j][*it]),2);
      }
      running_total += pow((data[i][feature_to_add] - data[j][feature_to_add]),2);
      shadow[i][j] = shadow[j][i] = sqrt(running_total);
    }
  }
  int currentNearest;
  int correct = 0;
  for(int i = 0; i < data.size(); i++){
    currentNearest = (i==0)?1:0;
    for(int j = currentNearest+1; j < data.size(); j++){
      if(i != j){
        if( shadow[i][currentNearest] > shadow[i][j]){
          currentNearest = j;
        }
      }
    }
    if(data[i][0] == data[currentNearest][0]){
      correct++;
    }
    if(data.size() - correct_number < (i+1) - correct){
      return 0.0;
    }
  }
  for(int i = 0; i < data.size(); i++){
    delete[] shadow[i];
  }
  delete[] shadow;
  return float(correct)/float(data.size());
}

void forward_search_demo(const std::vector<std::vector<float>>& data){
  std::set<int> max_set;
  float best_accuracy = 0.0;
  float acc;
  std::set<int> current_set;
  float max_accuracy = 0.0;
  int feature_to_add;
  //bool accuracy_declining = false;
  for(int i = 1; i < data[0].size(); i++){
    max_accuracy = 0;
    std::cout<<"On the "<<i<<"th level of the search tree\n";
    for(int j = 1; j < data[i].size(); j++){
      if(current_set.find(j) == current_set.end()){
        std::cout<<"Considering adding the "<<j<<"th feature\n";
        acc = accuracy_add(data, current_set, j);
        std::cout<<"  This has an accuracy of "<<acc<<'\n';
        if (acc > max_accuracy){
          max_accuracy = acc;
          feature_to_add = j;
        }
      }
    }
    current_set.insert(feature_to_add); 
    std::cout<<"On level "<<i<<" I added feature "<<feature_to_add<<" with accuracy of "<<max_accuracy<<"\n\n";
    if(max_accuracy > best_accuracy){
      best_accuracy = max_accuracy;
      max_set = current_set;
      // accuracy_declining = false;
    }
    // else if(accuracy_declining){
    //   break;
    // }
    // else{
    //   accuracy_declining = true;
    // }
  }
  std::cout<<"Best accuracy was on set {";
  for(auto it = max_set.begin(); it != max_set.end(); it++){
    std::cout<<*it;
    if(std::next(it) != max_set.end()){
      std::cout<<',';
    }
  }
  std::cout<<"} with accuracy "<<best_accuracy<<'\n';
}

void backward_search_demo(const std::vector<std::vector<float>>& data){
  std::set<int> max_set;
  float best_accuracy = 0.0;
  float acc;
  std::set<int> current_set;
  for(int i = 1; i < data[0].size(); i++){
    current_set.insert(i);
  }
  float max_accuracy = 0;
  int feature_to_remove;
  // bool accuracy_declining = false;
  for(int i = 1; i < data[0].size();i++) {
    max_accuracy = 0;
    std::cout<<"On the "<<i<<"th level of the search tree\n";
    for(auto it = current_set.begin(); it != current_set.end(); it++){
      std::cout<<"Considering removing the "<<*it<<"th feature\n";
      acc = accuracy_remove(data, current_set, *it);
      std::cout<<"  This has an accuracy of "<<acc<<'\n';
      if (acc > max_accuracy){
        max_accuracy = acc;
        feature_to_remove = *it;
      }  
    }
    current_set.erase(feature_to_remove);
    std::cout<<"On level "<<i<<" I removed feature "<<feature_to_remove<<" with accuracy of "<<max_accuracy<<"\n\n";
    if(max_accuracy > best_accuracy){
      best_accuracy = max_accuracy;
      max_set = current_set;
      // accuracy_declining = false;
    }
    // else if(accuracy_declining){
    //   break;
    // }
    // else{
    //   accuracy_declining = true;
    // }
  }
  std::cout<<"Best accuracy was on set {";
  for(auto it = max_set.begin(); it != max_set.end(); it++){
    std::cout<<*it;
    if(std::next(it) != max_set.end()){
      std::cout<<',';
    }
  }
  std::cout<<"} with accuracy "<<best_accuracy<<'\n';
}

void faster_search_demo(const std::vector<std::vector<float>>& data){
  std::set<int> max_set;
  
  float best_accuracy = 0.0;
  float acc;
  std::set<int> current_set;
  float max_accuracy = 0.0;
  int feature_to_add;
  bool accuracy_declining = false;
  for(int i = 1; i < data[0].size(); i++){
    max_accuracy = 0;
    std::cout<<"On the "<<i<<"th level of the search tree\n";
    for(int j = 1; j < data[i].size(); j++){
      if(current_set.find(j) == current_set.end()){
        std::cout<<"Considering adding the "<<j<<"th feature\n";
        acc = accuracy_add_pruning(data, current_set, j, int(std::max(best_accuracy,max_accuracy) * data.size()));
        std::cout<<"  This has an accuracy of "<<acc<<'\n';
        if (acc >= max_accuracy){
          max_accuracy = acc;
          feature_to_add = j;
        }
      }
    }
    std::cout<<"On level "<<i<<" I added feature "<<feature_to_add<<" with accuracy of "<<max_accuracy<<"\n\n";
    current_set.insert(feature_to_add);
    if(max_accuracy > best_accuracy){
      best_accuracy = max_accuracy;
      max_set = current_set;
    }
    else if(accuracy_declining){
      break;
    }
    else{
      accuracy_declining = true;
    }
  }
  std::cout<<"Best accuracy was on set {";
  for(auto it = max_set.begin(); it != max_set.end(); it++){
    std::cout<<*it;
    if(std::next(it) != max_set.end()){
      std::cout<<',';
    }
  }
  std::cout<<"} with accuracy "<<best_accuracy<<'\n';
}

void greedy_search_demo(const std::vector<std::vector<float>>& data){
  std::set<int> max_set;
  float best_accuracy = 0.0;
  float acc;
  std::set<int> current_set;
  float max_accuracy = 0.0;
  int feature_to_add;
  for(int i = 1; i < data[0].size(); i++){
    max_accuracy = 0;
    std::cout<<"On the "<<i<<"th level of the search tree\n";
    for(int j = 1; j < data[i].size(); j++){
      if(current_set.find(j) == current_set.end()){
        std::cout<<"Considering adding the "<<j<<"th feature\n";
        acc = accuracy_add(data, current_set, j);
        std::cout<<"  This has an accuracy of "<<acc<<'\n';
        if (acc > max_accuracy){
          max_accuracy = acc;
          feature_to_add = j;
        }
      }
    }
    current_set.insert(feature_to_add); 
    std::cout<<"On level "<<i<<" I added feature "<<feature_to_add<<"\n\n";
    if(max_accuracy > best_accuracy){
      best_accuracy = max_accuracy;
      max_set = current_set;
    }
    else{
      break;
    }
  }
  std::cout<<"Best accuracy was on set {";
  for(auto it = max_set.begin(); it != max_set.end(); it++){
    std::cout<<*it;
    if(std::next(it) != max_set.end()){
      std::cout<<',';
    }
  }
  std::cout<<"} with accuracy "<<best_accuracy<<'\n';
}

std::vector<std::vector<float>> parseFile(std::istream &in, bool comma = false){
  std::vector<std::vector<float>> output;
  std::string line;
  float number;
  char c;
  std::vector <float> lineVector;
  while(getline(in, line)){
    std::istringstream temp(line);
    lineVector.clear();
    while(temp>>number){
      lineVector.push_back(number);
      if(temp && comma){
        temp>>c;
      }
    }
    output.push_back(lineVector);
  }
  return output;
}

int main(int argc, char** argv){ 
  bool backward = false;
  std::ifstream file;
  std::clock_t start;
  bool comma = false;
  if(argc < 5 || (std::strcmp(argv[1], "-t") != 0 && (std::strcmp(argv[1], "-f") != 0) || (std::strcmp(argv[3], "-t") != 0 && (std::strcmp(argv[3], "-f") != 0)))){
    std::cout<<"Usage: ./featuresearch -t <forward/backward> -f <filename>\n";
    return 0;
  }
  else{
    if(std::strcmp(argv[1], "-t") == 0){
      backward = std::strcmp(argv[2], "backward") == 0?true:false;
      file.open(argv[4]);
    }
    else{
      backward = std::strcmp(argv[4], "backward") == 0?true:false;
      file.open(argv[2]);
    }
    if(argc > 5 && std::strcmp(argv[5], "-c") == 0){
      comma = true;
    }
    start = std::clock();
    std::vector<std::vector<float>> data = parseFile(file, comma);
    // for(int i = 0; i < data.size(); i++){
    //   for(int j = 0; j < data[i].size(); j++){
    //     std::cout<<data[i][j]<<"  ";
    //   }
    //   std::cout<<'\n';
    // }   
    srand(time(NULL));
    if(backward){
      backward_search_demo(data);
    }
    else{
      forward_search_demo(data);
    }
    std::cout<<"Program took "<<(std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000)<<" milliseconds to complete.\n";
    return 0;
  }
}
