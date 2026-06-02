#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// ==========================================
// ESTRUCTURAS BÁSICAS
// ==========================================
struct Proceso {
    int id;
    string nombre;
    int prioridad; // Menor número = mayor prioridad (Ej: 1 es más urgente que 5)
};

struct NodoProceso {
    Proceso dato;
    NodoProceso* siguiente;
};

struct NodoMemoria {
    int idBloque;
    string estado;
    NodoProceso* procesoAsignado; // Puntero al proceso que ocupa la memoria
    NodoMemoria* siguiente;
};

// ==========================================
// 1. GESTOR DE MEMORIA (PILA)
// ==========================================
class PilaMemoria {
private:
    NodoMemoria* tope;
    int contadorBloques;
public:
    PilaMemoria() {
        tope = NULL;
        contadorBloques = 0;
    }

    void push(int idBloque) {
        NodoMemoria* nuevo = new NodoMemoria();
        nuevo->idBloque = idBloque;
        nuevo->estado = "Libre";
        nuevo->procesoAsignado = NULL;
        nuevo->siguiente = tope;
        tope = nuevo;
        contadorBloques++;
        cout << "[Memoria] Bloque " << idBloque << " apilado (Asignado).\n";
    }

    void pop() {
        if (tope == NULL) {
            cout << "[Memoria] Error: La pila de memoria esta vacia.\n";
            return;
        }
        NodoMemoria* aux = tope;
        tope = tope->siguiente;
        cout << "[Memoria] Bloque " << aux->idBloque << " liberado (Pop).\n";
        delete aux;
        contadorBloques--;
    }

    void mostrar() {
        if (tope == NULL) {
            cout << "[Memoria] Pila vacia.\n";
            return;
        }
        NodoMemoria* actual = tope;
        cout << "\n--- ESTADO DE MEMORIA (PILA) ---\n";
        while (actual != NULL) {
            cout << "| Bloque ID: " << actual->idBloque << " | Estado: " << actual->estado << " |\n";
            actual = actual->siguiente;
        }
        cout << "--------------------------------\n";
    }
};

// ==========================================
// 2. PLANIFICADOR DE CPU (COLA DE PRIORIDAD)
// ==========================================
class ColaPrioridad {
private:
    NodoProceso* frente;
    NodoProceso* fin;
public:
    ColaPrioridad() { frente = NULL; fin = NULL; }

    void encolar(Proceso p) {
        NodoProceso* nuevo = new NodoProceso();
        nuevo->dato = p;
        nuevo->siguiente = NULL;

        // Si esta vacía o el nuevo tiene más prioridad (menor numero) que el frente
        if (frente == NULL || p.prioridad < frente->dato.prioridad) {
            nuevo->siguiente = frente;
            frente = nuevo;
        } else {
            // Buscar la posicion correcta segun prioridad
            NodoProceso* actual = frente;
            while (actual->siguiente != NULL && actual->siguiente->dato.prioridad <= p.prioridad) {
                actual = actual->siguiente;
            }
            nuevo->siguiente = actual->siguiente;
            actual->siguiente = nuevo;
        }
        cout << "[CPU] Proceso " << p.nombre << " encolado con prioridad " << p.prioridad << ".\n";
    }

    void desencolar() {
        if (frente == NULL) {
            cout << "[CPU] No hay procesos en la cola para ejecutar.\n";
            return;
        }
        NodoProceso* aux = frente;
        frente = frente->siguiente;
        cout << "\n[>> EJECUTANDO <<] Proceso ID: " << aux->dato.id << " | Nombre: " << aux->dato.nombre << "\n";
        delete aux;
    }

    void mostrar() {
        if (frente == NULL) {
            cout << "[CPU] Cola de procesos vacia.\n";
            return;
        }
        NodoProceso* actual = frente;
        cout << "\n--- COLA DE PRIORIDAD CPU ---\n";
        while (actual != NULL) {
            cout << "ID: " << actual->dato.id << " | Proceso: " << actual->dato.nombre 
                 << " | Prioridad: " << actual->dato.prioridad << "\n";
            actual = actual->siguiente;
        }
    }
};

// ==========================================
// 3. GESTOR DE PROCESOS (LISTA ENLAZADA SIMPLE)
// ==========================================
class ListaProcesos {
private:
    NodoProceso* cabeza;
public:
    ListaProcesos() { cabeza = NULL; }

    void insertar(Proceso p) {
        NodoProceso* nuevo = new NodoProceso();
        nuevo->dato = p;
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        cout << "[Gestor] Proceso registrado correctamente.\n";
    }

    void eliminar(int id) {
        if (cabeza == NULL) return;
        if (cabeza->dato.id == id) {
            NodoProceso* aux = cabeza;
            cabeza = cabeza->siguiente;
            delete aux;
            cout << "[Gestor] Proceso eliminado.\n";
            return;
        }
        NodoProceso* actual = cabeza;
        while (actual->siguiente != NULL && actual->siguiente->dato.id != id) {
            actual = actual->siguiente;
        }
        if (actual->siguiente != NULL) {
            NodoProceso* aux = actual->siguiente;
            actual->siguiente = aux->siguiente;
            delete aux;
            cout << "[Gestor] Proceso eliminado.\n";
        } else {
            cout << "[Gestor] Proceso no encontrado.\n";
        }
    }

    NodoProceso* buscar(int id) {
        NodoProceso* actual = cabeza;
        while (actual != NULL) {
            if (actual->dato.id == id) return actual;
            actual = actual->siguiente;
        }
        return NULL;
    }

    void modificarPrioridad(int id, int nuevaPrioridad) {
        NodoProceso* p = buscar(id);
        if (p != NULL) {
            p->dato.prioridad = nuevaPrioridad;
            cout << "[Gestor] Prioridad actualizada.\n";
        } else {
            cout << "[Gestor] Proceso no encontrado.\n";
        }
    }

    void mostrarTodo() {
        NodoProceso* actual = cabeza;
        cout << "\n--- REGISTRO GENERAL DE PROCESOS ---\n";
        while (actual != NULL) {
            cout << "ID: " << actual->dato.id << " | Nombre: " << actual->dato.nombre 
                 << " | Prioridad: " << actual->dato.prioridad << "\n";
            actual = actual->siguiente;
        }
    }

    // Persistencia de datos
    void guardarEnArchivo(const string& nombreArchivo) {
        ofstream archivo(nombreArchivo.c_str());
        NodoProceso* actual = cabeza;
        while (actual != NULL) {
            archivo << actual->dato.id << " " << actual->dato.nombre << " " << actual->dato.prioridad << "\n";
            actual = actual->siguiente;
        }
        archivo.close();
        cout << "[Sistema] Datos guardados en " << nombreArchivo << ".\n";
    }

    void cargarDesdeArchivo(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo.c_str());
        if (!archivo.is_open()) return;
        
        // Limpiar lista actual antes de cargar
        while(cabeza != NULL) eliminar(cabeza->dato.id);

        Proceso p;
        while (archivo >> p.id >> p.nombre >> p.prioridad) {
            insertar(p);
        }
        archivo.close();
        cout << "[Sistema] Datos cargados de " << nombreArchivo << ".\n";
    }
};

// ==========================================
// MENÚ PRINCIPAL
// ==========================================
int main() {
    ListaProcesos gestorProcesos;
    ColaPrioridad planificadorCPU;
    PilaMemoria gestorMemoria;
    
    // Cargar estado inicial si existe
    gestorProcesos.cargarDesdeArchivo("procesos.txt");

    int opcion;
    do {
        cout << "\n========== SISTEMA DE GESTION DE PROCESOS ==========\n";
        cout << "1. Registrar nuevo proceso (Lista)\n";
        cout << "2. Eliminar proceso por ID (Lista)\n";
        cout << "3. Modificar prioridad de proceso (Lista)\n";
        cout << "4. Ver todos los procesos registrados (Lista)\n";
        cout << "5. Enviar proceso a ejecucion (Cola CPU)\n";
        cout << "6. Ejecutar siguiente proceso (Desencolar CPU)\n";
        cout << "7. Ver cola de CPU (Cola)\n";
        cout << "8. Asignar bloque de memoria (Pila Push)\n";
        cout << "9. Liberar bloque de memoria (Pila Pop)\n";
        cout << "10. Ver estado de memoria (Pila)\n";
        cout << "11. Guardar estado y Salir\n";
        cout << "Ingrese opcion: ";
        cin >> opcion;

        if(opcion == 1) {
            Proceso p;
            cout << "ID del Proceso: "; cin >> p.id;
            cout << "Nombre: "; cin >> p.nombre;
            cout << "Prioridad (1 alta - 5 baja): "; cin >> p.prioridad;
            gestorProcesos.insertar(p);
        }
        else if(opcion == 2) {
            int id; cout << "ID a eliminar: "; cin >> id;
            gestorProcesos.eliminar(id);
        }
        else if(opcion == 3) {
            int id, prio; 
            cout << "ID del proceso: "; cin >> id;
            cout << "Nueva prioridad: "; cin >> prio;
            gestorProcesos.modificarPrioridad(id, prio);
        }
        else if(opcion == 4) {
            gestorProcesos.mostrarTodo();
        }
        else if(opcion == 5) {
            int id; cout << "ID del proceso a encolar en CPU: "; cin >> id;
            NodoProceso* nodo = gestorProcesos.buscar(id);
            if (nodo != NULL) {
                planificadorCPU.encolar(nodo->dato);
            } else {
                cout << "[Error] Proceso no existe en el registro general.\n";
            }
        }
        else if(opcion == 6) {
            planificadorCPU.desencolar();
        }
        else if(opcion == 7) {
            planificadorCPU.mostrar();
        }
        else if(opcion == 8) {
            static int contadorIdMemoria = 100;
            gestorMemoria.push(contadorIdMemoria++);
        }
        else if(opcion == 9) {
            gestorMemoria.pop();
        }
        else if(opcion == 10) {
            gestorMemoria.mostrar();
        }
        else if(opcion == 11) {
            gestorProcesos.guardarEnArchivo("procesos.txt");
            cout << "Saliendo del sistema...\n";
        }
    } while (opcion != 11);

    return 0;
}
