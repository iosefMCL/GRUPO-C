#include <iostream>
#include <string>

using namespace std;

struct Proceso {
    int id;
    string nombre;
    int prioridad;
};

struct NodoLista {
    Proceso proceso;
    NodoLista* siguiente;
};

NodoLista* listaProcesos = NULL;

void insertarProceso(int id, string nombre, int prioridad) {
    NodoLista* nuevoNodo = new NodoLista();
    nuevoNodo->proceso.id = id;
    nuevoNodo->proceso.nombre = nombre;
    nuevoNodo->proceso.prioridad = prioridad;
    nuevoNodo->siguiente = listaProcesos;
    listaProcesos = nuevoNodo;
    cout << "Proceso '" << nombre << "' registrado con exito.\n";
}

void mostrarProcesos() {
    if (listaProcesos == NULL) {
        cout << "No hay procesos registrados en el sistema.\n";
        return;
    }
    NodoLista* actual = listaProcesos;
    cout << "\n--- LISTA DE PROCESOS REGISTRADOS ---\n";
    while (actual != NULL) {
        cout << "ID: " << actual->proceso.id 
             << " | Nombre: " << actual->proceso.nombre
             << " | Prioridad: " << actual->proceso.prioridad << "\n";
        actual = actual->siguiente;
    }
}

void eliminarProceso(int id) {
    NodoLista* actual = listaProcesos;
    NodoLista* anterior = NULL;
    bool encontrado = false;

    while (actual != NULL && !encontrado) {
        if (actual->proceso.id == id) {
            encontrado = true;
            if (anterior == NULL) {
                listaProcesos = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            delete actual;
            cout << "Proceso eliminado correctamente del registro.\n";
        } else {
            anterior = actual;
            actual = actual->siguiente;
        }
    }
    if (!encontrado) cout << "Error: Proceso con ID " << id << " no encontrado.\n";
}

void buscarProceso(string criterio) {
    NodoLista* actual = listaProcesos;
    bool encontrado = false;
    
    int idBuscado = -1;
    try {
        idBuscado = stoi(criterio);
    } catch (...) {
        idBuscado = -1;
    }

    cout << "\n--- RESULTADO DE BUSQUEDA ---\n";
    while (actual != NULL) {
        if (actual->proceso.id == idBuscado || actual->proceso.nombre == criterio) {
            cout << "Encontrado -> ID: " << actual->proceso.id 
                 << " | Nombre: " << actual->proceso.nombre 
                 << " | Prioridad: " << actual->proceso.prioridad << "\n";
            encontrado = true;
        }
        actual = actual->siguiente;
    }
    if (!encontrado) cout << "No se encontro ningun proceso que coincida con: " << criterio << "\n";
}

void modificarPrioridad(int id, int nuevaPrioridad) {
    NodoLista* actual = listaProcesos;
    while (actual != NULL) {
        if (actual->proceso.id == id) {
            actual->proceso.prioridad = nuevaPrioridad;
            cout << "Prioridad del proceso ID " << id << " cambiada a " << nuevaPrioridad << ".\n";
            return;
        }
        actual = actual->siguiente;
    }
    cout << "Error: Proceso no encontrado.\n";
}

int main() {
	
    int opcion;

    do {
        cout<<"===== SISTEMA DE GESTION DE PROCESOS ======"<<endl;
        cout << "1. Registrar proceso\n";
        cout << "2. Mostrar procesos registrados\n";
        cout << "3. Eliminar proceso del sistema\n";
        cout << "4. Buscar proceso (por ID o Nombre)\n";
        cout << "5. Modificar prioridad de un proceso\n";
        cout << "6. Salir\n";
        cout << "opcion: ";
        cin >> opcion;

        int id, prio;
        string criterio, nom;

        switch (opcion) {
            case 1:
                cout << "ID unico del proceso: "; cin >> id;
                cout << "Nombre del proceso: "; cin >> nom;
                cout << "Prioridad (1-Alta, 2-Media, 3-Baja): "; cin >> prio;
                insertarProceso(id, nom, prio);
                break;
            case 2:
                mostrarProcesos();
                break;
                
            case 3:
                cout << "ID del proceso a eliminar: "; cin >> id;
                eliminarProceso(id);
                break;
                
            case 4:
                cout << "Ingrese el ID o Nombre a buscar: "; cin >> criterio;
                buscarProceso(criterio);
                break;
                
            case 5:
                cout << "ID del proceso a modificar: "; cin >> id;
                cout << "Nueva prioridad: "; cin >> prio;
                modificarPrioridad(id, prio);
                break;
                
            case 6:
                cout << "Saliendo de la primera parte...\n";
                break;
                
            default:
                cout << "Opcion no valida, intente nuevamente.\n";
        }
    } while (opcion != 6);
    
    cout<<"saliendo"<<endl;

    return 0;
}