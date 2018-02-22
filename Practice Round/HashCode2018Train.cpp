#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <random>
#include <cstring>

using namespace std;

struct PROBLEM_DATA_STRUCTURE{
public:
    unsigned int L;
    unsigned int H;
    char PIZZA[1000][1000];
    unsigned int MAX_R;
    unsigned int MAX_C;
};

static PROBLEM_DATA_STRUCTURE problem_data;

struct slice{
public:
    unsigned int R1;
    unsigned int R2;
    unsigned int C1;
    unsigned int C2;
};

static list<slice> output;

unsigned int heuristica_n1(slice c){
    return (c.R2 - c.R1 + 1) * (c.C2 - c.C1 + 1);
}

bool validate_slice(slice to_validate){
    if(to_validate.R2 >= problem_data.MAX_R || to_validate.C2 >= problem_data.MAX_C) return false;

    unsigned int num_of_m = 0;
    unsigned int num_of_t = 0;

    for (int i = to_validate.R1; i <= to_validate.R2; i++){
        for (int j = to_validate.C1; j <= to_validate.C2; j++){
            switch(problem_data.PIZZA[i][j]) {
                case 'T':
                    ++num_of_t;
                    break;
                case 'M':
                    ++num_of_m;
                    break;
                default:
                    // Posicion ocupada por otro slice
                    return false;
            }
        }
    }

    return (num_of_m >= problem_data.L) && (num_of_t >= problem_data.L) && (num_of_m + num_of_t <= problem_data.H);

}

vector<pair<unsigned int,unsigned int>> obtain_posibles_slices(){
    vector<pair<unsigned int,unsigned int>> result(0);
    for (unsigned int i = 1; i < problem_data.H + 1 ; ++i) {
        for (unsigned int j = 1; j < problem_data.H + 1 ; ++j) {
            if((i * j >= 2 * problem_data.L) && (i * j <= problem_data.H)){
                result.emplace_back(pair<unsigned int,unsigned int>(i -1, j -1));
            }

        }
    }
    return result;
}

void algoritmo_n2(unsigned int casos_prueba){
    default_random_engine generator(time(0));
    uniform_int_distribution<unsigned int> distribution_c(0,problem_data.MAX_C - 1);
    uniform_int_distribution<unsigned int> distribution_r(0,problem_data.MAX_R - 1);

    auto posibles_dim_slice = obtain_posibles_slices();
    for (unsigned int i = 0; i < casos_prueba; ++i) {
        cout << "Progreso: " << i  << "/" << casos_prueba << endl;

        slice actual_slice;
        actual_slice.C1 = distribution_c(generator);
        actual_slice.R1 = distribution_r(generator);
        list<slice> posible_slices;

        for (auto next_node : posibles_dim_slice) {
            actual_slice.C2 = actual_slice.C1 + next_node.first;
            actual_slice.R2 = actual_slice.R1 + next_node.second;
            if(validate_slice(actual_slice)){
                posible_slices.push_back(actual_slice);
            }
        }

        if(!posible_slices.empty()){
            unsigned int mejor_heuristica = 0;
            slice mejor_nodo;
            for (auto nodo : posible_slices){
                auto heuristica_actual = heuristica_n1(nodo);
                if(mejor_heuristica < heuristica_actual){
                    mejor_nodo = nodo;
                    mejor_heuristica = heuristica_actual;
                }
            }
            output.push_back(mejor_nodo);
            //Poner data a 'X'
            for (int k = mejor_nodo.R1; k <= mejor_nodo.R2; k++){
                for (int r = mejor_nodo.C1; r <= mejor_nodo.C2; r++){
                    problem_data.PIZZA[k][r] = 'X';
                }
            }
        }
    }
}


void load_data(const char path[]){
    ifstream myfile;
    myfile.open (path);
    if(myfile.is_open()){
        myfile >> problem_data.MAX_R >> problem_data.MAX_C >> problem_data.L >> problem_data.H;

        for(int i = 0; i < problem_data.MAX_R ; i++){
            char intermedio[1008]; //Evita desbordar PIZZA[i] con el null final
            myfile >> intermedio;
            strncpy(problem_data.PIZZA[i], intermedio, problem_data.MAX_C);
        }
    }
    myfile.close();
}




unsigned int obtener_posible_resultado(){
    unsigned int resultado = 0;
    for(auto i : output){
        resultado += heuristica_n1(i);
    }
    return resultado;
}

void save_output(const char path[]){
    cout << "Posible resultado: " << obtener_posible_resultado() << endl;
    cout << "Maximo resultado posible: " << problem_data.MAX_R * problem_data.MAX_C << endl;

    ofstream myfile;
    myfile.open (path);
    if(myfile.is_open()){
        myfile << output.size() << endl;
        for (auto i: output) {
            myfile << i.R1 << " " << i.C1 << " " << i.R2 << " " << i.C2 << endl;
        }
    }
    myfile.close();
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
