#include <string>
#include <fstream>

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

int main(int argc, const char* argv[]) {
    if(argc != 4){
	return -1;
    }

    load_data(argv[1]);//"/home/abel/Escritorio/example.in"
    algoritmo_n2(atoi(argv[2])); // 1000
    save_output(argv[3]);// "/home/abel/Escritorio/example.out"
    return 0;
}
