#include <fstream>      // E/S
#include <utility>      // std::pair, std::make_pair
#include <string>       // std::string
#include <iostream>     // std::cout
#include <cmath>        // std::abs

int R, C, F, N, B, T, a, b, x, y , s, f;

using namespace std;

void load_data(const char path[]){
	char endline;
    ifstream myfile;
    myfile.open (path);
    if(myfile.is_open()){
        myfile >> R >> C >> F >> N >> B >> T;
		getline(myfile, endline);

        for(int i = 0; i < problem_data.MAX_R ; i++){
            char intermedio[1008];
            myfile >> intermedio;
            strncpy(problem_data.PIZZA[i], intermedio, problem_data.MAX_C);
        }
    }
    myfile.close();
}

int obtenerDistancia(pair <int,int> ini, pair<int,int> end){
    return abs(ini.first-end.first)+abs(ini.second-end.second);
}














int main(int argc, const char* argv[]) {
    if(argc != 4){
	return -1;
    }

    load_data(argv[1]);//"/home/abel/Escritorio/example.in"
    algoritmo_n2(atoi(argv[2])); // 1000
    save_output(argv[3]);// "/home/abel/Escritorio/example.out"
    return 0;
}