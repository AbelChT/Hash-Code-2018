import random

problem_data = {}  # {"L": None, "H": None, "PIZZA": None}  # L número múnimo de cada ingrediente y h el múximo de celdas
output_data = []  # {"R1": None, "R2": None, "C1": None, "C2": None}


def validate_slice(r1, r2, c1, c2):
    if r1 > r2:
        r1, r2 = r2, r1
    if c1 > c2:
        c1, c2 = c2, c1

    if r1 < 0 or c1 < 0 or r2 >= problem_data["MAX_R"] or c2 >= problem_data["MAX_C"]:
        return False

    # Comprobar si el trozo está ya en el output data
    for outputs in output_data:
        c1_o = outputs["C1"]
        c2_o = outputs["C2"]
        r1_o = outputs["R1"]
        r2_o = outputs["R2"]
        colisionan = ((
                              ((c1 <= c1_o <= c2) or (c1 <= c2_o <= c2)) and
                              ((r1 <= r1_o <= r2) or (r1 <= r2_o <= r2))
                      ) or (
                              ((c1_o <= c1 <= c2_o) or (c1_o <= c2 <= c2_o)) and
                              ((r1_o <= r1 <= r2_o) or (r1_o <= r2 <= r2_o))
                      ) or (
                              ((c1 <= c1_o <= c2) or (c1 <= c2_o <= c2)) and
                              ((r1_o <= r1 <= r2_o) or (r1_o <= r2 <= r2_o))
                      ) or (
                              ((c1_o <= c1 <= c2_o) or (c1_o <= c2 <= c2_o)) and
                              ((r1 <= r1_o <= r2) or (r1 <= r2_o <= r2))
                      ) or (
                              ((r1 <= r1_o <= r2) or (r1 <= r2_o <= r2)) and
                              ((c1_o <= c1 <= c2_o) or (c1_o <= c2 <= c2_o))
                      ) or (
                              ((r1_o <= r1 <= r2_o) or (r1_o <= r2 <= r2_o)) and
                              ((c1 <= c1_o <= c2) or (c1 <= c2_o <= c2))
                      )
                      )
        if colisionan:
            return False

    # Comprobar si es un trozo válido
    num_of_m = 0
    num_of_t = 0
    for j in problem_data["PIZZA"][r1:r2 + 1]:
        for i in j[c1:c2 + 1]:
            if i == "T":
                num_of_t = num_of_t + 1
            else:
                num_of_m = num_of_m + 1
    if (num_of_m < problem_data["L"]) or (num_of_t < problem_data["L"]) or (num_of_m + num_of_t > problem_data["H"]):
        return False
    return True


def algoritmo_n1():
    for i in range(len(problem_data["PIZZA"])):
        c1 = 0
        act = problem_data["PIZZA"]
        for c2 in range(len(act)):
            if validate_slice(i, i, c1, c2):
                output_data.append({"R1": i, "R2": i, "C1": c1, "C2": c2})


""" Algoritmo n2 """


def algoritmo_n2(num_intentos):
    posibles_dim_slice = obtain_posibles_slices(problem_data["L"], problem_data["H"])
    iterador_debug = 0
    for i in range(num_intentos):
        print("Progreso: " + str(iterador_debug) + "/" + str(num_intentos))
        iterador_debug = iterador_debug + 1
        c1 = random.randrange(0, problem_data["MAX_C"])
        r1 = random.randrange(0, problem_data["MAX_R"])
        cortes_posibles = []
        for next_node in posibles_dim_slice:
            for direction in [(1, 1), (1, -1), (-1, 1), (-1, -1)]:
                c2 = c1 + direction[0] * next_node[0]
                r2 = r1 + direction[1] * next_node[1]
                if validate_slice(r1, r2, c1, c2):
                    if r1 > r2:
                        r1, r2 = r2, r1
                    if c1 > c2:
                        c1, c2 = c2, c1
                    cortes_posibles.append({"R1": r1, "R2": r2, "C1": c1, "C2": c2})

        if len(cortes_posibles) > 0:
            mejor_heuristica = 0
            mejor_nodo = None
            for nodo in cortes_posibles:
                heuristica_actual = heuristica_n1(nodo)
                if mejor_heuristica < heuristica_actual:
                    mejor_nodo = nodo
                    mejor_heuristica = heuristica_actual

            output_data.append(mejor_nodo)


# Heuristica tamaño del trozo


def heuristica_n1(nodo):
    r1 = nodo["R1"]
    r2 = nodo["R2"]
    c1 = nodo["C1"]
    c2 = nodo["C2"]

    return (r2 - r1 + 1) * (c2 - c1 + 1)


def obtain_posibles_slices(l, h):
    result = []
    for i in range(1, h + 1):
        for j in range(1, h + 1):
            if (i * j >= 2 * l) and (i * j <= h):
                result.append((i - 1, j - 1))
    return result


""" Load data """


def load_data(path):
    i_file = open(path, 'rb')
    i_str = i_file.read()
    i_text = i_str.decode('utf-8', errors='ignore')  # Convert to Unicode
    i_text = i_text.split("\n")
    datos = i_text[0].split(" ")

    problem_data["L"] = int(datos[2])
    problem_data["H"] = int(datos[3])
    problem_data["PIZZA"] = i_text[1:-1]  # La última línea está en blanco
    problem_data["MAX_R"] = len(problem_data["PIZZA"])
    problem_data["MAX_C"] = len(problem_data["PIZZA"][0])

    i_file.close()


def obtener_posible_resultado():
    resultado = 0
    for i in output_data:
        resultado = resultado + heuristica_n1(i)
    return resultado


def save_output(path):
    print("Posible resultado: " + str(obtener_posible_resultado()))
    print("Maximo resultado posible: " + str(problem_data["MAX_R"] * problem_data["MAX_C"]))
    with open(path, 'w') as file:
        file.write(str(len(output_data)) + "\n")
        for slice_pizza in output_data:
            file.write(str(slice_pizza["R1"]) + " " +
                       str(slice_pizza["C1"]) + " " +
                       str(slice_pizza["R2"]) + " " +
                       str(slice_pizza["C2"]) + "\n")


load_data("/home/abel/Escritorio/big.in")
algoritmo_n2(4000)#((problem_data["MAX_R"] + 1) * (problem_data["MAX_C"] + 1)) // 20
save_output("/home/abel/Escritorio/big.out")
