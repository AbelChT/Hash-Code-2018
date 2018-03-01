#include <string>
#include <fstream>
#include <iostream>
#include <list>

int R, C, F, N, B, T;

using namespace std;

struct Viaje{
public:
    unsigned int a;
    unsigned int b;
    unsigned int x;
    unsigned int y;
	unsigned int s;
	unsigned int f;
};

static list<Viaje> viajes;

struct coche{
public:
    int num_steps = 0;
    void add_viaje(pair<int,int> inicio, pair<int,int> final, int muy_pronto, int muy_tarde){

    }
    bool can_add (pair<int,int> inicio, pair<int,int> final, int muy_pronto, int muy_tarde){

    }
    int puntos_viaje(pair<int,int> inicio, pair<int,int> final, int muy_pronto, int muy_tarde){

    }
};
list<int> solucion[1000];

void load_data(const char path[]){
	string endline;
    ifstream myfile;
    myfile.open (path);
    if(myfile.is_open()){
        myfile >> R >> C >> F >> N >> B >> T;
		getline(myfile, endline);
		Viaje Aux;

        for(int i = 0; i < N; i++){
			myfile >> Aux.a >> Aux.b >> Aux.x >> Aux.y >> Aux.s >> Aux.f;
			getline(myfile, endline);
			viajes.push_back(Aux);
        }
    }
    myfile.close();
}

void save_output(const char path[]) {
    ofstream myfile;
    myfile.open(path);
    if (myfile.is_open()) {
        for (int i = 0; i < F; i++) {
			if(solucion[i].empty()){;}
			else{
				myfile << solucion[i].size() << " ";
				for (auto x: solucion[i]){
					myfile << x << " ";
				}
				myfile << endl;
			}
        }
    }
    myfile.close();
}

int main(int argc, const char* argv[]) {
    if(argc != 4){
	return -1;
    }

    load_data(argv[1]);//"/home/abel/Escritorio/example.in"
    save_output(argv[3]);// "/home/abel/Escritorio/example.out"
    return 0;
}
