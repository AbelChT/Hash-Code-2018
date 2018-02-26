#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <random>
#include <cstring>
#include <algorithm>

using namespace std;

struct PROBLEM_DATA_STRUCTURE {
public:
    unsigned int L;
    unsigned int H;
    char PIZZA[1000][1000];
    unsigned int MAX_R;
    unsigned int MAX_C;
};

static PROBLEM_DATA_STRUCTURE problem_data;

struct slice {
public:
    slice() = default;

    unsigned int R1;
    unsigned int R2;
    unsigned int C1;
    unsigned int C2;

    slice(unsigned int R1, unsigned int C1) : R1(R1), C1(C1) {}
};

static list<slice> output;

unsigned int score_of_slice(const slice &c) {
    return (c.R2 - c.R1 + 1) * (c.C2 - c.C1 + 1);
}


bool validate_slice(slice to_validate) {
    if (to_validate.R2 >= problem_data.MAX_R || to_validate.C2 >= problem_data.MAX_C) return false;

    unsigned int num_of_m = 0;
    unsigned int num_of_t = 0;

    for (int i = to_validate.R1; i <= to_validate.R2; i++) {
        for (int j = to_validate.C1; j <= to_validate.C2; j++) {
            switch (problem_data.PIZZA[i][j]) {
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

vector<pair<unsigned int, unsigned int>> obtain_posibles_slices() {
    vector<pair<unsigned int, unsigned int>> result(0);

    for (unsigned int i = 1; i < problem_data.H + 1; ++i) {
        for (unsigned int j = 1; j < problem_data.H + 1; ++j) {
            if ((i * j >= 2 * problem_data.L) && (i * j <= problem_data.H)) {
                result.emplace_back(pair<unsigned int, unsigned int>(i - 1, j - 1));
            }

        }
    }

    vector<pair<unsigned int, unsigned int>> result1(0);
    for (long i = result.size() - 1; i >= 0; --i) {
        result1.emplace_back(result[i]);
    }
    return result1;
}

unsigned int heuristica(const slice &c, int PIZZA_SLICES_PROB[][1000]) {
    unsigned int total = 0;
    for (int i = c.R1; i <= c.R2; ++i) {
        for (int j = c.C1; j <= c.C2; ++j) {
            total = total + 190 - PIZZA_SLICES_PROB[i][j];
        }
    }
    // medium 190
    return total;
}

vector<pair<slice, unsigned int>> obtain_all_posibles_slices() {

    list<pair<slice, unsigned int>> posibles_cortes;

    int PIZZA_SLICES_PROB[1000][1000];

    const auto posibles_dim_slice = obtain_posibles_slices();

    for (unsigned int i = 0; i < problem_data.MAX_R; ++i) {
        for (unsigned int j = 0; j < problem_data.MAX_C; ++j) {
            PIZZA_SLICES_PROB[i][j] = 0;
        }
    }

    for (unsigned int i = 0; i < problem_data.MAX_R; ++i) {
        for (unsigned int j = 0; j < problem_data.MAX_C; ++j) {
            cout << i << " " << j << endl;
            slice actual_slice;
            actual_slice.C1 = j;
            actual_slice.R1 = i;

            for (auto next_node : posibles_dim_slice) {
                actual_slice.C2 = actual_slice.C1 + next_node.first;
                actual_slice.R2 = actual_slice.R1 + next_node.second;
                if (validate_slice(actual_slice)) {
                    for (int k = actual_slice.R1; k <= actual_slice.R2; ++k) {
                        for (int l = actual_slice.C1; l <= actual_slice.C2; ++l) {
                            ++PIZZA_SLICES_PROB[k][l];
                        }

                    }
                    posibles_cortes.emplace_back(pair<slice, unsigned int>(actual_slice, 0));
                }
            }
        }
    }

    auto my_vector = vector<pair<slice, unsigned int>> {std::make_move_iterator(std::begin(posibles_cortes)),
                                                        std::make_move_iterator(std::end(posibles_cortes))};

    for (int m = 0; m < my_vector.size(); ++m) {
        my_vector[m].second = heuristica(my_vector[m].first, PIZZA_SLICES_PROB);
    }


    return my_vector;

};

bool my_comparator(const pair<slice, unsigned int> &first, const pair<slice, unsigned int> &second) {
    return first.second >= second.second;
}

// big 800499
// medium 43370
void algoritmo() {
    auto posibles_cortes = obtain_all_posibles_slices();

    //sort (posibles_cortes.begin(), posibles_cortes.end(), my_comparator);

    for (auto i : posibles_cortes) {
        if (validate_slice(i.first)) {
            output.push_back(i.first);
            //Poner data a 'X'
            for (int k = i.first.R1; k <= i.first.R2; k++) {
                for (int r = i.first.C1; r <= i.first.C2; r++) {
                    problem_data.PIZZA[k][r] = 'X';
                }
            }
        }
    }
}

int getColor(int color) {
    return ((15 - color) * 180) / 15;
}

void render(const char path[]) {
    FILE *fp;

    fp = fopen(path, "w+");

    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", problem_data.MAX_C, problem_data.MAX_R);
    fprintf(fp, "255\n");

    int y;
    int x;
    for (y = 0; y < problem_data.MAX_R; y++) {
        for (x = 0; x < problem_data.MAX_C; x++) {
            if (problem_data.PIZZA[y][x] == 'X')
                fprintf(fp, "%d %d %d\n", getColor(1), getColor(1), getColor(1));
            else if(problem_data.PIZZA[y][x] == 'M')
                fprintf(fp, "%d %d %d\n", 45, 54, 120);
            else
                fprintf(fp, "%d %d %d\n", 30, 150, 10);
        }
    }

    fclose(fp);
}

void load_data(const char path[]) {
    ifstream myfile;
    myfile.open(path);
    if (myfile.is_open()) {
        myfile >> problem_data.MAX_R >> problem_data.MAX_C >> problem_data.L >> problem_data.H;

        for (int i = 0; i < problem_data.MAX_R; i++) {
            char intermedio[1008];
            myfile >> intermedio;
            strncpy(problem_data.PIZZA[i], intermedio, problem_data.MAX_C);
        }
    }
    myfile.close();
}

unsigned int obtain_score() {
    unsigned int resultado = 0;
    for (auto i : output) {
        resultado += score_of_slice(i);
    }
    return resultado;
}

void save_output(const char path[]) {
    cout << "Posible resultado: " << obtain_score() << endl;
    cout << "Maximo resultado posible: " << problem_data.MAX_R * problem_data.MAX_C << endl;

    ofstream myfile;
    myfile.open(path);
    if (myfile.is_open()) {
        myfile << output.size() << endl;
        for (auto i: output) {
            myfile << i.R1 << " " << i.C1 << " " << i.R2 << " " << i.C2 << endl;
        }
    }
    myfile.close();
}

#if 0
int main(int argc, const char* argv[]) {
    if(argc != 3){
        return -1;
    }

    load_data(argv[1]);//"/home/abel/Escritorio/example.in"
    clock_t tStart = clock();
    algoritmo();
    cout << "Tiempo = " << clock() - tStart << endl;
    save_output(argv[2]);// "/home/abel/Escritorio/example.out"
    return 0;
}

#else

int main() {
    load_data("/home/abel/Descargas/big.in");//"/home/abel/Escritorio/example.in"
    clock_t tStart = clock();
    algoritmo();
    cout << "Tiempo = " << clock() - tStart << endl;
    render("/home/abel/Descargas/big.ppm");
    save_output("/home/abel/Descargas/big.out");// "/home/abel/Escritorio/example.out"
    return 0;
}

#endif
