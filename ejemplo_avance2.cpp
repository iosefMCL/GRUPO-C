#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// ==========================================
// ESTRUCTURAS BÁSICAS
// ==========================================
struct Estudiante {
    int id;
    string nombre;
    int prioridad; // 1 (Tercio Superior/Alta) a 5 (Baja)
};

struct NodoEstudiante {
    Estudiante dato;
    NodoEstudiante* siguiente;
};

struct NodoHistorial {
    int idBoleta;
    Estudiante estudiante;
    NodoHistorial* siguiente;
};

// ==========================================
// 1. PILA: HISTORIAL DE TRÁMITES (Reemplaza al Gestor de Memoria)
// ==========================================
class PilaHistorial {
private:
    NodoHistorial* tope;
    int contadorBoletas;
public:
    PilaHistorial() {
        tope = NULL;
        contadorBoletas = 1000;
    }

    void push(Estudiante est) {
        NodoHistorial* nuevo = new NodoHistorial();
        nuevo->idBoleta = contadorBoletas++;
        nuevo->estudiante = est;
        nuevo->siguiente = tope;
        tope = nuevo;
        cout << "[Historial] Matricula procesada. Boleta Nro: " << nuevo->idBoleta << " generada.\n";
    }

    void pop() {
        if (tope == NULL) {
            cout << "[Historial] No hay matriculas recientes para deshacer.\n";
            return;
        }
        NodoHistorial* aux = tope;
        tope = tope->siguiente;
        cout << "[Historial] Matricula deshecha (Pop). Boleta anulada: " << aux->idBoleta << " de " << aux->estudiante.nombre << "\n";
        delete aux;
    }

    void mostrar() {
        if (tope == NULL) {
            cout << "[Historial] Pila vacia.\n";
            return;
        }
        NodoHistorial* actual = tope;
        cout << "\n--- HISTORIAL DE MATRICULAS (PILA) ---\n";
        while (actual != NULL) {
            cout << "| Boleta: " << actual->idBoleta << " | Alumno: " << actual->estudiante.nombre << " |\n";
            actual = actual->siguiente;
        }
        cout << "--------------------------------------\n";
    }
};

// ==========================================
// 2. COLA DE PRIORIDAD: FILA DE MATRÍCULA (Reemplaza al Planificador CPU)
// ==========================================
class ColaMatricula {
private:
    NodoEstudiante* frente;
    NodoEstudiante* fin;
public:
    ColaMatricula() { frente = NULL; fin = NULL; }

    void encolar(Estudiante p) {
        NodoEstudiante* nuevo = new NodoEstudiante();
        nuevo->dato = p;
        nuevo->siguiente = NULL;

        // Inserción ordenada por prioridad (Menor número = mayor prioridad)
        if (frente == NULL || p.prioridad < frente->dato.prioridad) {
            nuevo->siguiente = frente;
            frente = nuevo;
        } else {
            NodoEstudiante* actual = frente;
            while (actual->siguiente != NULL && actual->siguiente->dato.prioridad <= p.prioridad) {
                actual = actual->siguiente;
            }
            nuevo->siguiente = actual->siguiente;
            actual->siguiente = nuevo;
        }
        cout << "[Cola] Estudiante " << p.nombre << " en la fila con prioridad " << p.prioridad << ".\n";
    }

    Estudiante desencolar() {
        Estudiante vacio = {-1, "", -1};
        if (frente == NULL) {
            cout << "[Cola] No hay estudiantes en espera.\n";
            return vacio;
        }
        NodoEstudiante* aux = frente;
        Estudiante procesado = aux->dato;
        frente = frente->siguiente;
        cout << "\n[>> PROCESANDO <<] Matrícula de: " << procesado.nombre << " (ID: " << procesado.id << ")\n";
        delete aux;
        return procesado;
    }

    void mostrar() {
        if (frente == NULL) {
            cout << "[Cola] La fila de matricula esta vacia.\n";
            return;
        }
        NodoEstudiante* actual = frente;
        cout << "\n--- FILA DE ESPERA DE MATRICULA ---\n";
        while (actual != NULL) {
            cout << "ID: " << actual->dato.id << " | Alumno: " << actual->dato.nombre 
                 << " | Prioridad: " << actual->dato.prioridad << "\n";
            actual = actual->siguiente;
        }
    }
};

// ==========================================
// 3. LISTA ENLAZADA: REGISTRO DE ALUMNOS (Reemplaza al Gestor de Procesos)
// ==========================================
class ListaEstudiantes {
private:
    NodoEstudiante* cabeza;
public:
    ListaEstudiantes() { cabeza = NULL; }

    void insertar(Estudiante p) {
        NodoEstudiante* nuevo = new NodoEstudiante();
        nuevo->dato = p;
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        cout << "[Registro] Estudiante guardado en el sistema.\n";
    }

    void eliminar(int id) {
        if (cabeza == NULL) return;
        if (cabeza->dato.id == id) {
            NodoEstudiante* aux = cabeza;
            cabeza = cabeza->siguiente;
            delete aux;
            cout << "[Registro] Estudiante eliminado.\n";
            return;
        }
        NodoEstudiante* actual = cabeza;
        while (actual->siguiente != NULL && actual->siguiente->dato.id != id) {
            actual = actual->siguiente;
        }
        if (actual->siguiente != NULL) {
            NodoEstudiante* aux = actual->siguiente;
            actual->siguiente = aux->siguiente;
            delete aux;
            cout << "[Registro] Estudiante eliminado.\n";
        } else {
            cout << "[Registro] ID no encontrado.\n";
        }
    }

    NodoEstudiante* buscar(int id) {
        NodoEstudiante* actual = cabeza;
        while (actual != NULL) {
            if (actual->dato.id == id) return actual;
            actual = actual->siguiente;
        }
        return NULL;
    }

    void modificarPrioridad(int id, int nuevaPrioridad) {
        NodoEstudiante* p = buscar(id);
        if (p != NULL) {
            p->dato.prioridad = nuevaPrioridad;
            cout << "[Registro] Prioridad academica actualizada.\n";
        } else {
            cout << "[Registro] Estudiante no encontrado.\n";
        }
    }

    void mostrarTodo() {
        NodoEstudiante* actual = cabeza;
        cout << "\n--- REGISTRO GENERAL DE ESTUDIANTES ---\n";
        while (actual != NULL) {
            cout << "ID: " << actual->dato.id << " | Nombre: " << actual->dato.nombre 
                 << " | Prioridad: " << actual->dato.prioridad << "\n";
            actual = actual->siguiente;
        }
    }

    // Persistencia de datos
    void guardarEnArchivo(const string& nombreArchivo) {
        ofstream archivo(nombreArchivo.c_str());
        NodoEstudiante* actual = cabeza;
        while (actual != NULL) {
            archivo << actual->dato.id << " " << actual->dato.nombre << " " << actual->dato.prioridad << "\n";
            actual = actual->siguiente;
        }
        archivo.close();
        cout << "[Sistema] Datos guardados.\n";
    }

    void cargarDesdeArchivo(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo.c_str());
        if (!archivo.is_open()) return;
        
        while(cabeza != NULL) eliminar(cabeza->dato.id);

        Estudiante p;
        while (archivo >> p.id >> p.nombre >> p.prioridad) {
            insertar(p);
        }
        archivo.close();
        cout << "[Sistema] Base de datos cargada.\n";
    }
};

// ==========================================
// MENÚ PRINCIPAL E INTERFAZ DE USUARIO
// ==========================================
int main() {
    ListaEstudiantes registroGeneral;
    ColaMatricula filaEspera;
    PilaHistorial historial;
    
    // Cargar estado inicial si existe
    registroGeneral.cargarDesdeArchivo("estudiantes.txt");

    int opcion;
    do {
        cout << "\n========== UNIVERSIDAD CONTINENTAL ==========\n";
        cout << "   SISTEMA DE MATRICULA - ING. DE SISTEMAS\n";
        cout << "=============================================\n";
        cout << "1. Registrar estudiante (Lista)\n";
        cout << "2. Eliminar estudiante del registro (Lista)\n";
        cout << "3. Modificar prioridad academica (Lista)\n";
        cout << "4. Ver todos los estudiantes (Lista)\n";
        cout << "5. Enviar estudiante a fila de matricula (Cola)\n";
        cout << "6. Procesar siguiente matricula (Desencolar y Push a Pila)\n";
        cout << "7. Ver fila de espera (Cola)\n";
        cout << "8. Deshacer ultima matricula (Pop de Pila)\n";
        cout << "9. Ver historial de tramites (Pila)\n";
        cout << "10. Guardar y Salir\n";
        cout << "Ingrese opcion: ";
        cin >> opcion;

        if(opcion == 1) {
            Estudiante p;
            cout << "ID (DNI o Codigo): "; cin >> p.id;
            cout << "Nombre (Sin espacios): "; cin >> p.nombre;
            cout << "Prioridad (1 Tercio Superior - 5 Regular): "; cin >> p.prioridad;
            registroGeneral.insertar(p);
        }
        else if(opcion == 2) {
            int id; cout << "ID a eliminar: "; cin >> id;
            registroGeneral.eliminar(id);
        }
        else if(opcion == 3) {
            int id, prio; 
            cout << "ID del estudiante: "; cin >> id;
            cout << "Nueva prioridad: "; cin >> prio;
            registroGeneral.modificarPrioridad(id, prio);
        }
        else if(opcion == 4) {
            registroGeneral.mostrarTodo();
        }
        else if(opcion == 5) {
            int id; cout << "ID del estudiante a encolar: "; cin >> id;
            NodoEstudiante* nodo = registroGeneral.buscar(id);
            if (nodo != NULL) {
                filaEspera.encolar(nodo->dato);
            } else {
                cout << "[Error] El estudiante no existe en el registro general.\n";
            }
        }
        else if(opcion == 6) {
            Estudiante procesado = filaEspera.desencolar();
            if(procesado.id != -1) {
                historial.push(procesado); // Se guarda en la pila
            }
        }
        else if(opcion == 7) {
            filaEspera.mostrar();
        }
        else if(opcion == 8) {
            historial.pop();
        }
        else if(opcion == 9) {
            historial.mostrar();
        }
        else if(opcion == 10) {
            registroGeneral.guardarEnArchivo("estudiantes.txt");
            cout << "Saliendo del sistema...\n";
        }
    } while (opcion != 10);

    return 0;
}
