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
	pair<int,int> inicio(){
		return pair<int, int>(a,b);
	};
	pair<int,int> terminar(){
		return pair<int, int>(x,y);
	};

	int muy_pronto (){
		return s;
	}

	int muy_tarde (){
		return f;
	}
};

static list<Viaje> viajes;


struct coche{
public:
	int num_steps = 0;
    pair<int,int> p = pair<int,int>(0,0);
    void add_viaje(pair<int,int> inicio, pair<int,int> final, int muy_pronto, int muy_tarde){
        num_steps += obtenerDistancia(inicio,p);
        if (num_steps < muy_pronto) {
            num_steps = muy_pronto;
        }
        num_steps += obtenerDistancia(inicio, final);
        p.first = final.first;
        p.second = final.second;
    }
    bool can_add (pair<int,int> inicio, pair<int,int> final, int muy_pronto, int muy_tarde){
        int num_steps2 = num_steps+obtenerDistancia(inicio,p);
        if (num_steps2 < muy_pronto) {
            num_steps2 = muy_pronto;
        }
        num_steps2 += obtenerDistancia(inicio, final);
        return num_steps2<=muy_tarde;
    }
    int puntos_viaje(pair<int,int> inicio, pair<int,int> final, int muy_pronto, int muy_tarde){
        int puntos = 0;
        if (this->can_add(inicio, final,muy_pronto, muy_tarde)) {
            int num_steps2 = num_steps+obtenerDistancia(inicio,p);
            if (num_steps2 <= muy_pronto) {
                puntos += B;
            }
            puntos += obtenerDistancia(inicio, final);
        }
        return puntos;
    }
    int obtenerDistancia(pair <int,int> ini, pair<int,int> end){
        return abs(ini.first-end.first)+abs(ini.second-end.second);
    }
};
list<int> solucion[1000];
static list <coche> coches;

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

void algoritmo(){
    for( auto i : viajes){
        for (auto j : coches){
            if(j.can_add(i.inicio(),i.terminar(),i.muy_pronto(),i.muy_tarde())){
                j.add_viaje(i.inicio(),i.terminar(),i.muy_pronto(),i.muy_tarde());
            }
        }
    }
}

int main() {

    load_data("/Users/adrian/Desktop/Hash-Code-2018/Main Round/a_example.in");//"/home/abel/Escritorio/example.in"
	algoritmo();
    save_output("/Users/adrian/Desktop/Hash-Code-2018/Main Round/a_example.out");// "/home/abel/Escritorio/example.out"
    return 0;
}
