
#include <fstream>

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
            char intermedio[1008]; //Evita desbordar PIZZA[i] con el null final
            myfile >> intermedio;
            strncpy(problem_data.PIZZA[i], intermedio, problem_data.MAX_C);
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
