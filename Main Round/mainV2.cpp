#include <string>
#include <fstream>
#include <list>
#include <algorithm>
#include <vector>

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
    pair<int,int> inicio() const{
        return pair<int, int>(a,b);
    };
    pair<int,int> terminar() const {
        return pair<int, int>(x,y);
    };

    int muy_pronto () const{
        return s;
    }

    int muy_tarde () const{
        return f;
    }
};

list<pair<Viaje,long>> viajes;

struct coche{
public:
    long num_steps = 0;
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
    int puntos_viaje(const pair<int,int> inicio,const pair<int,int> final, int muy_pronto, int muy_tarde){
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
list<long> solucion[10000];

vector<pair<coche, long>> coches;

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
            viajes.emplace_back(pair<Viaje,int >(Aux,i));
        }
    }
    myfile.close();
}

void save_output(const char path[]) {
    ofstream myfile;
    myfile.open(path);
    if (myfile.is_open()) {
        for (int i = 0; i < F; i++) {
            if(!solucion[i].empty()){
                myfile << solucion[i].size() << " ";
                for (auto x: solucion[i]){
                    myfile << x << " ";
                }
                myfile << endl;
            }
            else{
                myfile << 0 << endl;
            }
        }
    }
    myfile.close();
}


bool my_comparator_viajes(const pair<Viaje,int> &first, const pair<Viaje,int> &second) {
    return first.first.inicio() < second.first.inicio();
}

/*bool my_comparator_coches(const pair<coche,int> &first, const pair<coche,int> &second, Viaje v) {
    return first.first.puntos_viaje(v.inicio(),v.terminar(),v.muy_pronto(),v.muy_tarde()) > second.first.puntos_viaje(v.inicio(),v.terminar(),v.muy_pronto(),v.muy_tarde());
}*/


void algoritmo(){
    for(int i = 0; i < F; i++){
        coche c;
        auto sub = pair<coche, long>(c,i);
        coches.emplace_back(sub);
    }
    viajes.sort(my_comparator_viajes);

    for (auto i : viajes) {
        int mejor = 0;
        int mejor_k = 0;
        for(int k =0 ; k < coches.size(); k++){
            auto &j = coches[k];
            if(j.first.puntos_viaje(i.first.inicio(),i.first.terminar(),i.first.muy_pronto(),i.first.muy_tarde()) > mejor){
                mejor =j.first.puntos_viaje(i.first.inicio(),i.first.terminar(),i.first.muy_pronto(),i.first.muy_tarde());
                mejor_k = k;
            }
        }

        auto &j = coches[mejor_k];
        j.first.add_viaje(i.first.inicio(),i.first.terminar(),i.first.muy_pronto(),i.first.muy_tarde());
        solucion[j.second].push_back(i.second);

    }
}

int main() {
    load_data("/Users/adrian/Desktop/Hash-Code-2018/Main Round/e_high_bonus.in");//"/home/abel/Escritorio/example.in"
    algoritmo();
    save_output("/Users/adrian/Desktop/Hash-Code-2018/Main Round/e_high_bonus.out");// "/home/abel/Escritorio/example.out"
    return 0;
}