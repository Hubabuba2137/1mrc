#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include <chrono>
#include <unordered_map>
#include <functional>
#include <random>
#include <future>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <sstream>

using namespace std;

struct station {
    string name;
    float mean_val;

    station() : name(""), mean_val(0.0) {}
    station(string n, float mean) : name(n), mean_val(mean) {}
};

station prep(const std::string& input) {
    string result = input.substr(1, input.length() - 2);

    size_t commaPos = result.find(',');
    if (commaPos == std::string::npos) {
        throw invalid_argument("Nieprawidłowy format wejściowy");
    }

    string name = result.substr(1, commaPos - 2);
    string number = result.substr(commaPos + 1);

    name.erase(0, name.find_first_not_of(' '));
    name.erase(name.find_last_not_of(' ') + 1);
    number.erase(0, number.find_first_not_of(' '));
    number.erase(number.find_last_not_of(' ') + 1);

    name.erase(0, 2);

    station temp_stat(name, stof(number));
    return temp_stat;
}

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> dis(-100, 100);

int random_temp(float mean) {
    return dis(gen) + static_cast<int>(mean * 10);
}

void write_to_file(vector<char>& buffer, ofstream& zapis) {
    zapis.write(buffer.data(), buffer.size());
    buffer.clear();
}

void process_station_data(const vector<station>& stations_list, int iterations, vector<char>& buffer, ofstream& zapis) {
    const int vector_len = stations_list.size();
    uniform_int_distribution<> station_dis(0, vector_len - 1);
    for (int i = 0; i < (iterations / 4); ++i) {
        station temp_1 = stations_list[station_dis(gen)];
        station temp_2 = stations_list[station_dis(gen)];
        station temp_3 = stations_list[station_dis(gen)];
        station temp_4 = stations_list[station_dis(gen)];   

        string t_st_1 = to_string(random_temp(temp_1.mean_val) * 0.1);
        string t_st_2 = to_string(random_temp(temp_2.mean_val) * 0.1);
        string t_st_3 = to_string(random_temp(temp_3.mean_val) * 0.1);
        string t_st_4 = to_string(random_temp(temp_4.mean_val) * 0.1);

        string line = temp_1.name + "," + t_st_1.erase(t_st_1.find('.')+2, t_st_1.length()) +';'+ "\n" +
                      temp_2.name + "," + t_st_2.erase(t_st_2.find('.')+2, t_st_2.length()) +';'+ "\n" +
                      temp_3.name + "," + t_st_3.erase(t_st_3.find('.')+2, t_st_3.length()) +';'+ "\n" +
                      temp_4.name + "," + t_st_4.erase(t_st_4.find('.')+2, t_st_4.length()) +';'+ "\n";

        buffer.insert(buffer.end(), line.begin(), line.end());

        if (buffer.size() >= buffer.capacity()) {
            write_to_file(buffer, zapis);
        }
    }
    write_to_file(buffer, zapis);
}

int main() {
    auto begin = chrono::high_resolution_clock::now();

    ios_base::sync_with_stdio(false);

    ifstream file("stations.txt");

    vector<station> stations_list;
    string line;
    while (getline(file, line)) {
        stations_list.push_back(prep(line));
    }
    file.close();

    const int iterations = 1000000;

    ofstream zapis("test.txt");

    const int buffer_size = 1 << 20;
    vector<char> buffer;
    buffer.reserve(buffer_size);

    
    for(int i = 0; i <10; ++i) {
        //don't know if 10 works, it would be 1 billion rows though my vs code just broke up after like 500 million lines
        ofstream zapis("temperature_data.txt", ios::app); 

        vector<char> buffer;
        buffer.reserve(buffer_size);

        process_station_data(stations_list, iterations, buffer, zapis);

        zapis.close();

        cout << "Iteracja " << i + 1 << " zakończona." << endl;
    }

    //process_station_data(stations_list, iterations, buffer, zapis);

    zapis.close();

    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - begin);
    cout << "Czas wykonywania " << elapsed.count() << "ms" << endl;

    return 0;
}
