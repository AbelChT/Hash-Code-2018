#include <string>
#include <fstream>
#include <list>
#include <algorithm>
#include <vector>
#include <array>
#include <iostream>

using namespace std;
struct viaje;
struct coche;


unsigned int obtener_distancia(const pair<int, int> &ini, const pair<int, int> &end) {
    return abs(ini.first - end.first) + abs(ini.second - end.second);
}

struct initial_data {
    unsigned int R = 0, C = 0, F = 0, N = 0, B = 0, T = 0;
    vector<viaje> viajes;

    unsigned int number_of_vehicles_in_the_fleet() {
        return F;
    }

    unsigned int bonus_for_starting_the_rid_on_time() {
        return B;
    }

    unsigned int num_of_rides() {
        return N;
    }
};

struct solution_data {
    array<list<unsigned int>, 1000> solucion;
};

struct viaje {
    unsigned int a, b, x, y, s, f;

    pair<int, int> start_intersection() const {
        return pair<int, int>(a, b);
    };

    pair<int, int> finish_intersection() const {
        return pair<int, int>(x, y);
    };

    unsigned int earliest_start() const {
        return s;
    }

    unsigned int latest_finish() const {
        return f;
    }
};

struct coche {
    unsigned int num_steps = 0;

    pair<int, int> p = pair<int, int>(0, 0);

    void add_viaje(const viaje &v, const initial_data &problem_data) {
        num_steps += obtener_distancia(v.start_intersection(), p);
        if (num_steps < v.earliest_start()) {
            num_steps = v.earliest_start();
        }
        num_steps += obtener_distancia(v.start_intersection(), v.finish_intersection());
        p.first = v.finish_intersection().first;
        p.second = v.finish_intersection().second;
    }

    bool can_add(const viaje &v, const initial_data &problem_data) const {
        int num_steps2 = num_steps + obtener_distancia(v.start_intersection(), p);
        if (num_steps2 < v.earliest_start()) {
            num_steps2 = v.earliest_start();
        }
        num_steps2 += obtener_distancia(v.start_intersection(), v.finish_intersection());
        return num_steps2 <= v.latest_finish();
    }

    unsigned int puntos_viaje(const viaje &v, const initial_data &problem_data) const {
        unsigned int puntos = 0;
        if (this->can_add(v, problem_data)) {
            int num_steps2 = num_steps + obtener_distancia(v.start_intersection(), p);
            if (num_steps2 <= v.earliest_start()) {
                puntos += problem_data.B;
            }
            puntos += obtener_distancia(v.start_intersection(), v.finish_intersection());
        }
        return puntos;
    }
};


bool my_comparator_viajes(const pair<viaje, int> &first, const pair<viaje, int> &second) {
    return first.first.start_intersection() < second.first.start_intersection();
}

bool comparator_viajes_early(const pair<viaje, int> &first, const pair<viaje, int> &second) {
    return first.first.earliest_start() < second.first.earliest_start();
}

/*bool my_comparator_coches(const pair<coche,int> &first, const pair<coche,int> &second, Viaje v) {
    return first.first.puntos_viaje(v.inicio(),v.terminar(),v.muy_pronto(),v.muy_tarde()) > second.first.puntos_viaje(v.inicio(),v.terminar(),v.muy_pronto(),v.muy_tarde());
}*/

unsigned long heuristica_40(const coche &c, const viaje &v, const initial_data &problem_data) {
    unsigned long tiempo_perdido = c.num_steps + obtener_distancia(c.p, v.start_intersection());
    if (tiempo_perdido < v.earliest_start()) {
        tiempo_perdido = v.earliest_start();
    }

    tiempo_perdido = tiempo_perdido - c.num_steps;
    // tiempo_perdido = 20000000000L - tiempo_perdido;
    unsigned long puntos_ganados = c.puntos_viaje(v, problem_data);
    if (tiempo_perdido == 0) {
        return 4 * puntos_ganados;
    } else {
        return 2 * puntos_ganados / tiempo_perdido;
    }
}

// metropolis 1 18 7
// resto 128 32 8
unsigned long heuristica_n(const coche &c, const viaje &v, const initial_data &problem_data) {
    unsigned long tiempo_perdido = c.num_steps + obtener_distancia(c.p, v.start_intersection());
    int bonus = 1;
    if (tiempo_perdido < v.earliest_start()) {
        tiempo_perdido = v.earliest_start();
        bonus = 1;
    }

    tiempo_perdido = tiempo_perdido - c.num_steps;
    // tiempo_perdido = 20000000000L - tiempo_perdido;
    unsigned long puntos_ganados = c.puntos_viaje(v, problem_data) * bonus;
    if (tiempo_perdido == 0) {
        return 18 * puntos_ganados; // 8 4

    }
    else {
        return 7 * puntos_ganados / tiempo_perdido;
    }
}


unsigned long heuristica_37(const coche &c, const viaje &v, const initial_data &problem_data) {
    unsigned long tiempo_perdido = c.num_steps + obtener_distancia(c.p, v.start_intersection());
    if (tiempo_perdido < v.earliest_start()) {
        tiempo_perdido = v.earliest_start();
    }

    tiempo_perdido = tiempo_perdido - c.num_steps;
    // tiempo_perdido = 20000000000L - tiempo_perdido;
    unsigned long puntos_ganados = c.puntos_viaje(v, problem_data);
    return puntos_ganados;

}


unsigned long heuristica(const coche &c, const viaje &v, const initial_data &problem_data) {
    return heuristica_n(c, v, problem_data);
}

void algoritmo(initial_data &problem_data, solution_data &solucion) {
    list<pair<viaje, unsigned int>> viajes;
    for (unsigned int i = 0; i < problem_data.num_of_rides(); i++) {
        viajes.emplace_back(pair<viaje, unsigned int>(problem_data.viajes[i], i));
    }

    unsigned int coche_actual = 0;

    while (!viajes.empty() && coche_actual < problem_data.number_of_vehicles_in_the_fleet()) {
        coche c;
        viajes.sort(comparator_viajes_early);
        auto front_data = viajes.front();
        viajes.pop_front();
        if (c.can_add(front_data.first, problem_data)) {
            c.add_viaje(front_data.first, problem_data);
            solucion.solucion[coche_actual].emplace_back(front_data.second);

            unsigned long mejor_heuristica = 0;

            do {
                mejor_heuristica = 0;
                list<pair<viaje, unsigned int>>::iterator it_mejor;

                for (auto it = viajes.begin(); it != viajes.end(); it++) {
                    if (mejor_heuristica < heuristica(c, it->first, problem_data)) {
                        mejor_heuristica = heuristica(c, it->first, problem_data);
                        it_mejor = it;
                    }
                }

                if (mejor_heuristica != 0) {
                    solucion.solucion[coche_actual].emplace_back(it_mejor->second);
                    c.add_viaje(it_mejor->first, problem_data);
                    viajes.erase(it_mejor);
                }

            } while (mejor_heuristica != 0);
        }
        ++coche_actual;
    }
}


initial_data load_data(const char path[]) {
    initial_data problem_data;
    string endline;
    ifstream myfile;
    myfile.open(path);
    if (myfile.is_open()) {
        myfile >> problem_data.R >> problem_data.C >> problem_data.F >> problem_data.N >> problem_data.B
               >> problem_data.T;
        getline(myfile, endline);
        viaje aux;

        for (unsigned int i = 0; i < problem_data.N; i++) {
            myfile >> aux.a >> aux.b >> aux.x >> aux.y >> aux.s >> aux.f;
            getline(myfile, endline);
            problem_data.viajes.emplace_back(aux);
        }
    }
    myfile.close();

    return problem_data;
}

unsigned long comprobar_solucion(const initial_data &problem_data, const solution_data &solucion, char dataset_name[]) {
    vector<pair<coche, long>> coches2;
    unsigned long puntuacion = 0L;

    for (int i = 0; i < problem_data.F; i++) {
        coche c;
        auto &vl = solucion.solucion[i];
        for (auto j : vl) {
            auto &vv = problem_data.viajes[j];
            puntuacion += c.puntos_viaje(vv, problem_data);
            c.add_viaje(vv, problem_data);
        }
    }
    cout << dataset_name << ": " << puntuacion << endl;

    return puntuacion;
}

void save_output(const char path[], const initial_data &problem_data, const solution_data &solucion) {
    ofstream myfile;
    myfile.open(path);
    if (myfile.is_open()) {
        for (int i = 0; i < problem_data.F; i++) {
            if (!solucion.solucion[i].empty()) {
                myfile << solucion.solucion[i].size() << " ";
                for (auto x: solucion.solucion[i]) {
                    myfile << x << " ";
                }
                myfile << endl;
            } else {
                myfile << 0 << endl;
            }
        }
    }
    myfile.close();
}

int main() {
    solution_data soluciones[5];
    unsigned long puntuacion = 0L;

    auto data_set_1 = load_data("/home/abel/Descargas/a_example.in");//"/home/abel/Escritorio/example.in"ç
    algoritmo(data_set_1, soluciones[0]);
    save_output("/home/abel/Descargas/a_example.out", data_set_1, soluciones[0]);
    puntuacion += comprobar_solucion(data_set_1, soluciones[0], "a_example");

    auto data_set_2 = load_data("/home/abel/Descargas/b_should_be_easy.in");//"/home/abel/Escritorio/example.in"ç
    algoritmo(data_set_2, soluciones[1]);
    save_output("/home/abel/Descargas/b_should_be_easy.out", data_set_2, soluciones[1]);
    puntuacion += comprobar_solucion(data_set_2, soluciones[1], "b_should_be_easy");

    auto data_set_3 = load_data("/home/abel/Descargas/c_no_hurry.in");//"/home/abel/Escritorio/example.in"ç
    algoritmo(data_set_3, soluciones[2]);
    save_output("/home/abel/Descargas/c_no_hurry.out", data_set_3, soluciones[2]);
    puntuacion += comprobar_solucion(data_set_3, soluciones[2], "c_no_hurry");

    auto data_set_4 = load_data("/home/abel/Descargas/d_metropolis.in");//"/home/abel/Escritorio/example.in"ç
    algoritmo(data_set_4, soluciones[3]);
    save_output("/home/abel/Descargas/d_metropolis.out", data_set_4, soluciones[3]);
    puntuacion += comprobar_solucion(data_set_4, soluciones[3], "d_metropolis");

    auto data_set_5 = load_data("/home/abel/Descargas/e_high_bonus.in");//"/home/abel/Escritorio/example.in"ç
    algoritmo(data_set_5, soluciones[4]);
    save_output("/home/abel/Descargas/e_high_bonus.out", data_set_5, soluciones[4]);
    puntuacion += comprobar_solucion(data_set_5, soluciones[4], "e_high_bonus");

    cout << "Puntos totales: " << puntuacion << endl;
    return 0;
}