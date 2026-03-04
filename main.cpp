/*
Colas con prioridad (con solo una lista)
3 tipos de nodos, Amenaza falsa, Urgente, Reconocimiento
el programa debe leer:  id, descripcion, clasificacion y hora de recepcion

en C++, haremos un codigo, empieza por esto
Haz un menu para cmd basado en esta plantilla en c++ que el
usuario ingrese estos datos:
ID(10 digitos):
Descripcion(libre):
Clasificacion(alto, bajo, falsa amenaza):
la hora del reporte: lo lee el progama

luego estos datos Ponlos en un nodo que tenga un apuntador de memoria,
de modo que solo exista una lista enlazada

ahora, vamos a entrar en el manejo de esa lista enlazada para imprimir un reporte
final, pero los registros se imprimiran en orden segun...
-   la hora, de modo que si se pide un informe para las 10:45,
imprima todos los reportes hasta esa hora
(convertiremos la hora en minutos para facilitar la comparacion)
comparando la hora de cada resgistro con la hora que se pide.
-   su clasificacion
(alto, Alto, bajo, Bajo, Amenaza Falsa, amenaza falsa)

 manejaremos ese orden de impresion retrasando las impresiones el el buffer.

este es el orden

Amenazas altas Deben ser atendidas en un máximo de 1 minuto y luego de este tiempo pasan al reporte final.
Amenazas Bajas Permanecen en la cola durante 3 minutos y luego pasan al reporte final.
Amenazas falsas Son archivadas de inmediato

TEMPORAL
ahora al imprimir el reporte,
Amenazas altas Deben esperar en el buffer 1 minuto y luego de este tiempo pasan al reporte final.
Amenazas Bajas Permanecen en la cola durante 3 minutos y luego pasan al reporte final.
Amenazas falsas Son archivadas de inmediato
*/
#include <iostream>
#include <limits>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

// =====================
// ESTRUCTURA DE DATOS
// =====================

struct Registro {
    string id;
    string descripcion;
    string clasificacion;
    string horaReporte;
};

struct Nodo {
    Registro dato;
    Nodo* siguiente;

    Nodo(Registro r) {
        dato = r;
        siguiente = nullptr;
    }
};

// Puntero global a la lista
Nodo* cabeza = nullptr;


// =====================
// DECLARACIÓN DE FUNCIONES
// =====================

void mostrarMenu();
void ejecutarOpcion(int opcion);

void opcion1(); // Agregar registro
void opcion2(); // Mostrar registros
void opcion3(); // Vaciar lista

void convertirAMinutos();
void imprimirReportePorHora();
void verEstructuraLista(); //DEBUG
void agregarRegistro();
void mostrarRegistros();
void liberarMemoria();


bool idExiste(const string& id);
bool horaValida(const string& hora);// Valida formato y rango de hora
int convertirAMinutos(const string& hora);// Convierte hora HH:MM a minutos totales
string obtenerHoraActual();
string generarNuevoID();


// =====================
// FUNCIÓN PRINCIPAL
// =====================

int main() {
    int opcion;
    bool continuar = true;

    while (continuar) {
        mostrarMenu();
        cin >> opcion;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nEntrada inválida. Intente nuevamente.\n";
            continue;
        }

        if (opcion == 0) {
            continuar = false;
            liberarMemoria();
            cout << "\nSaliendo del programa...\n";
        } else {
            ejecutarOpcion(opcion);
        }
    }

    return 0;
}


// =====================
// IMPLEMENTACIÓN
// =====================

void mostrarMenu() {
    cout << "\n=================================\n";
    cout << "        MENU PRINCIPAL\n";
    cout << "=================================\n";
    cout << "1. Agregar Registro\n";
    cout << "2. Mostrar Registros\n";
    cout << "3. Vaciar Lista\n";
    cout << "4. Imprimir reporte por hora\n";
    // =====================
    cout << "5. Ver estructura interna (DEBUG)\n";//solo DEBUG
    // =====================
    cout << "0. Salir\n";
    cout << "---------------------------------\n";
    cout << "Seleccione una opcion: ";
}

void ejecutarOpcion(int opcion) {

    switch (opcion) {
    case 1:
        opcion1();
        break;
    case 2:
        opcion2();
        break;
    case 3:
        opcion3();
        break;

    case 4:
        imprimirReportePorHora();
        break;
        // =====================
    case 5: //solo DEBUG
        verEstructuraLista();
        break;
        // =====================
    default:
        cout << "\nOpcion no valida.\n";
    }
}

void opcion1() {
    cout << "\n--- Agregar Nuevo Registro ---\n";
    agregarRegistro();
}

void opcion2() {
    cout << "\n--- Lista de Registros ---\n";
    mostrarRegistros();
}

void opcion3() {
    liberarMemoria();
    cout << "\nLista vaciada correctamente.\n";
}


// =====================
// FUNCIONES DE LISTA
// =====================
string generarNuevoID() {

    int maxNumero = 0;

    Nodo* actual = cabeza;

    while (actual != nullptr) {

        string idActual = actual->dato.id;

        // Quitar la M
        int numero = stoi(idActual.substr(1));

        if (numero > maxNumero) {
            maxNumero = numero;
        }

        actual = actual->siguiente;
    }

    return "M" + to_string(maxNumero + 1);
}
bool idExiste(const string& id) {

    Nodo* actual = cabeza;

    while (actual != nullptr) {

        if (actual->dato.id == id) {
            return true;  // Ya existe
        }

        actual = actual->siguiente;
    }

    return false; // No existe
}
string obtenerHoraActual() {

    time_t ahora = time(nullptr);
    tm* tiempoLocal = localtime(&ahora);

    int horas = tiempoLocal->tm_hour;
    int minutos = tiempoLocal->tm_min;

    stringstream ss;
    ss << setw(2) << setfill('0') << horas
       << ":"
       << setw(2) << setfill('0') << minutos;

    return ss.str();
}
void imprimirReportePorHora() {

    if (cabeza == nullptr) {
        cout << "\nNo hay registros en la lista.\n";
        return;
    }

    string horaConsulta;
    bool horaCorrecta = false;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    while (!horaCorrecta) {
        cout << "Ingrese hora limite (HH:MM): ";
        getline(cin,horaConsulta);

        if (horaValida(horaConsulta)) {
            horaCorrecta = true;
        } else {
            cout << "Hora invalida.\n";
        }
    }

    int minutosLimite = convertirAMinutos(horaConsulta);

    Nodo* actual = cabeza;
    bool encontrado = false;

    cout << "\n====== REPORTE HASTA " << horaConsulta << " ======\n";

    while (actual != nullptr) {

        int minutosRegistro = convertirAMinutos(actual->dato.horaReporte);

        if (minutosRegistro <= minutosLimite) {

            encontrado = true;

            cout << "\nID: " << actual->dato.id << endl;
            cout << "Descripcion: " << actual->dato.descripcion << endl;
            cout << "Clasificacion: " << actual->dato.clasificacion << endl;
            cout << "Hora: " << actual->dato.horaReporte << endl;
            cout << "-----------------------------\n";
        }

        actual = actual->siguiente;
    }

    if (!encontrado) {
        cout << "No hay registros anteriores o iguales a esa hora.\n";
    }
}
bool idValido(const string& id) {

    // Debe tener al menos 2 caracteres: M + al menos un dígito
    if (id.length() < 2)
        return false;

    // Primer carácter debe ser M mayúscula
    if (id[0] != 'M')
        return false;

    // El resto deben ser solo dígitos
    for (size_t i = 1; i < id.length(); i++) {
        if (!isdigit(id[i]))
            return false;
    }

    return true;
}
bool horaValida(const string& hora) {

    if (hora.length() != 5) return false;

    // Formato HH:MM
    if (hora[2] != ':') return false;

    // Verificar que los demás sean dígitos
    if (!isdigit(hora[0]) || !isdigit(hora[1]) ||
        !isdigit(hora[3]) || !isdigit(hora[4])) {
        return false;
        }

    int horas = stoi(hora.substr(0, 2));
    int minutos = stoi(hora.substr(3, 2));

    if (horas < 0 || horas > 24) return false;
    if (minutos < 0 || minutos > 59) return false;

    // Caso especial: si es 24, los minutos deben ser 00
    if (horas == 24 && minutos != 0) return false;

    return true;
}
int convertirAMinutos(const string& hora) {
    int horas = stoi(hora.substr(0, 2));
    int minutos = stoi(hora.substr(3, 2));
    return horas * 60 + minutos;
}
void agregarRegistro() {

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Registro nuevo;

    nuevo.id = generarNuevoID();
    cout << "ID asignado automaticamente: " << nuevo.id << endl;

    cout << "Descripcion: ";
    getline(cin, nuevo.descripcion);

    // ===== VALIDACION DE CLASIFICACION =====
    bool clasificacionValida = false;

    while (!clasificacionValida) {
        cout << "Clasificacion (Alto, Bajo, Falsa Amenaza): ";
        getline(cin, nuevo.clasificacion);

        if (nuevo.clasificacion == "Alto" ||
            nuevo.clasificacion == "alto" ||
            nuevo.clasificacion == "Bajo" ||
            nuevo.clasificacion == "bajo" ||
            nuevo.clasificacion == "Falsa Amenaza" ||
            nuevo.clasificacion == "falsa amenaza") {

            clasificacionValida = true;

            } else {
                cout << "Clasificacion invalida. Intente nuevamente.\n";
            }
    }
    nuevo.horaReporte = obtenerHoraActual();
    cout << "Hora registrada automaticamente: " << nuevo.horaReporte << endl;

    Nodo* nuevoNodo = new Nodo(nuevo);

    if (cabeza == nullptr) {
        cabeza = nuevoNodo;
    } else {
        Nodo* actual = cabeza;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoNodo;
    }

    cout << "\nRegistro agregado exitosamente.\n";
}
void mostrarRegistros() {
    if (cabeza == nullptr) {
        cout << "No hay registros.\n";
        return;
    }

    Nodo* actual = cabeza;
    int contador = 1;

    while (actual != nullptr) {
        cout << "\nRegistro #" << contador++ << endl;
        cout << "ID: " << actual->dato.id << endl;
        cout << "Descripcion: " << actual->dato.descripcion << endl;
        cout << "Clasificacion: " << actual->dato.clasificacion << endl;
        cout << "Hora de Reporte: " << actual->dato.horaReporte << endl;
        cout << "----------------------------\n";

        actual = actual->siguiente;
    }
}
// =====================
// DEBUG
// =====================
void verEstructuraLista() {

    cout << "\n=========== DEBUG LISTA ENLAZADA ===========" << endl;

    if (cabeza == nullptr) {
        cout << "La lista esta vacia.\n";
        return;
    }

    cout << "Direccion de cabeza: " << cabeza << endl;

    Nodo* actual = cabeza;
    int contador = 0;

    while (actual != nullptr) {
        contador++;

        cout << "\nNodo #" << contador << endl;
        cout << "Direccion nodo: " << actual << endl;
        cout << "ID almacenado: " << actual->dato.id << endl;
        cout << "Apunta a: " << actual->siguiente << endl;

        actual = actual->siguiente;
    }

    cout << "\nTotal nodos conectados: " << contador << endl;
    cout << "============================================\n";
}

void liberarMemoria() {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
    cabeza = nullptr;
}
