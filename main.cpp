#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>
#include <cstdint>
#include <chrono>
#include <unordered_map>
#include <functional>

using namespace std;

struct stations{
    string name;
    float sum = 0;
    int num = 0;
    float min;
    float max = 0;

    stations(){
        name = "";
        sum = 0.0;
        num = 1;
    }

    stations(string nam){
        name = nam;
        sum =0;
        num =0;
    }

    float mean(){
        if(num!=0){
            return sum/num;
        }
        else{
            return -420.0;
        }
    }

    void update(const float& new_float){
        if(new_float>max){
            max = new_float;
        }
        if(new_float<min){
            min = new_float;
        }

        sum += new_float;
        num++;
    }

};

uint32_t bin_rep(const string& word) {
    uint32_t temp = 0;
    for (char ch : word) {
        temp |= 1 << (ch - 'a');
    }
    return temp;
}

int main(){
    auto begin = std::chrono::high_resolution_clock::now();

    //std::ios_base::sync_with_stdio(false);

    fstream plik;
    plik.open("test.txt");
    
    unordered_map<string, stations> stations_map;
    string line;
    string temp_name;
    float temp_float;

    const size_t lines_per_buffer = 100;

    vector<string> line_buffer;

    
    while(getline(plik,line)){
        size_t coma_pos = line.find(",");

        if (coma_pos == std::string::npos) {
            std::cerr << "Invalid line format: " << line << std::endl;
            continue;
        }
        
        temp_name = line.substr(0,coma_pos);
        temp_float = stof(line.substr(coma_pos+1, line.find(';')));   
        
        if (stations_map.find(temp_name) == stations_map.end()) {
            stations_map[temp_name] = stations(temp_name);
            stations_map[temp_name].min = temp_float;
        }

        stations_map[temp_name].update(temp_float);
    }

    plik.close();

    ofstream zapis("zapis.txt");
    
    for(auto& pair: stations_map){
        zapis<<pair.second.name <<": "<<pair.second.min<<"; " << pair.second.mean()<< "; " <<pair.second.max <<'\n';
    }

    zapis.close();
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout<<"Czas wykonywania "<< elapsed.count()<<"ms"<<std::endl;
    return 0;

}