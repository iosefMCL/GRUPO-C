#include <iostream> 
#include <string>   
#include <fstream>  

using namespace std;


// Estructura que representa la información real de un estudiante
struct Estudiante {
    int id;           // Identificador único (DNI o código de alumno)
    string nombre;    // Nombre del estudiante
    int prioridad;    // Nivel de prioridad (1 es la más alta, 5 la más baja)
};

// Nodo para las estructuras dinámicas (Lista y Cola)
struct NodoEstudiante {
    Estudiante dato;           // Guarda la información del estudiante
    NodoEstudiante* siguiente; // Puntero que enlaza con el próximo nodo en la memoria
};

// Nodo específico para la Pila (Historial)
struct NodoHistorial {
    int idBoleta;              // Número de boleta generada en la matrícula
    Estudiante estudiante;     // Datos del estudiante matriculado
    NodoHistorial* siguiente;  // Puntero que enlaza con el trámite anterior en la pila
};

// ==========================================
// 1. PILA: HISTORIAL DE TRÁMITES (LIFO: Último en entrar, primero en salir)
// ==========================================
class PilaHistorial {
private:
    NodoHistorial* tope; // Puntero que siempre mira al último elemento agregado
    int contadorBoletas; // Generador automático de números de boleta

public:
    // Constructor: Se ejecuta al crear la pila. Inicia vacía y las boletas desde 1000.
    PilaHistorial() {
        tope = NULL; 
        contadorBoletas = 1000;
    }

    // Método PUSH: Agrega un nuevo elemento a la cima de la pila
    void push(Estudiante est) {
        NodoHistorial* nuevo = new NodoHistorial(); // Reserva memoria dinámica para el nuevo nodo
        nuevo->idBoleta = contadorBoletas++;        // Asigna la boleta actual y luego suma 1
        nuevo->estudiante = est;                    // Guarda los datos del estudiante
        nuevo->siguiente = tope;                    // El nuevo nodo apunta al antiguo "tope"
        tope = nuevo;                               // Ahora el "tope" es este nuevo nodo
        cout << "[Historial] Matricula procesada. Boleta Nro: " << nuevo->idBoleta << " generada.\n";
    }

    // Método POP: Saca el último elemento agregado (Deshacer trámite)
    void pop() {
        if (tope == NULL) { // Verifica si la pila está vacía (Underflow)
            cout << "[Historial] No hay matriculas recientes para deshacer.\n";
            return;
        }
        NodoHistorial* aux = tope; // Guardamos temporalmente el nodo actual del tope
        tope = tope->siguiente;    // El tope baja al siguiente elemento de la pila
        cout << "[Historial] Matricula deshecha (Pop). Boleta anulada: " << aux->idBoleta << " de " << aux->estudiante.nombre << "\n";
        delete aux; // Liberamos la memoria RAM del nodo eliminado
    }

    // Muestra todos los elementos de la pila desde el tope hasta la base
    void mostrar() {
        if (tope == NULL) {
            cout << "[Historial] Pila vacia.\n";
            return;
        }
        NodoHistorial* actual = tope; // Empezamos a recorrer desde el tope
        cout << "\n--- HISTORIAL DE MATRICULAS (PILA) ---\n";
        while (actual != NULL) { // Mientras no lleguemos al final (NULL)
            cout << "| Boleta: " << actual->idBoleta << " | Alumno: " << actual->estudiante.nombre << " |\n";
            actual = actual->siguiente; // Saltamos al siguiente nodo
        }
        cout << "--------------------------------------\n";
    }
};

// ==========================================
// COLA DE PRIORIDAD: FILA DE MATRÍCULA
// ==========================================
class ColaMatricula {
private:
    NodoEstudiante* frente; // Puntero al primer elemento de la cola (el próximo a atender)
    NodoEstudiante* fin;    // Puntero al último elemento de la cola

public:
    // Constructor: Inicia la cola vacía
    ColaMatricula() { frente = NULL; fin = NULL; }

    // Encolar: Inserta un estudiante ORDENADO según su prioridad
    void encolar(Estudiante p) {
        NodoEstudiante* nuevo = new NodoEstudiante(); // Crea el nuevo nodo
        nuevo->dato = p;
        nuevo->siguiente = NULL; // Como es el último que estamos evaluando, apunta a la nada

        // Caso 1: La cola está vacía o el nuevo tiene MÁS prioridad (menor número) que el frente
        if (frente == NULL || p.prioridad < frente->dato.prioridad) {
            nuevo->siguiente = frente; // El nuevo nodo apunta al antiguo frente
            frente = nuevo;            // El frente ahora es el nuevo nodo
        } 
        // Caso 2: Buscar su posición correcta en medio o al final de la cola
        else {
            NodoEstudiante* actual = frente; // Empezamos a buscar desde el frente
            // Avanzamos mientras no estemos en el final y la prioridad del nodo siguiente sea menor o igual a la nuestra
            while (actual->siguiente != NULL && actual->siguiente->dato.prioridad <= p.prioridad) {
                actual = actual->siguiente;
            }
            // Insertamos el nuevo nodo entre 'actual' y 'actual->siguiente'
            nuevo->siguiente = actual->siguiente;
            actual->siguiente = nuevo;
        }
        cout << "[Cola] Estudiante " << p.nombre << " en la fila con prioridad " << p.prioridad << ".\n";
    }

    // Desencolar: Saca al primer elemento de la cola (FIFO: Primero en entrar/tener prioridad, primero en salir)
    Estudiante desencolar() {
        Estudiante vacio = {-1, "", -1}; // Dato por defecto en caso de error
        if (frente == NULL) { // Si la cola está vacía
            cout << "[Cola] No hay estudiantes en espera.\n";
            return vacio;
        }
        NodoEstudiante* aux = frente; // Guardamos el frente actual
        Estudiante procesado = aux->dato; // Guardamos los datos para devolverlos
        frente = frente->siguiente;       // El frente se mueve al segundo de la fila
        cout << "\n[>> PROCESANDO <<] Matricula de: " << procesado.nombre << " (ID: " << procesado.id << ")\n";
        delete aux; // Liberamos la memoria del nodo desencolado
        return procesado; // Retornamos los datos para mandarlos al historial (Pila)
    }

    // Imprime la cola actual
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
// 3. LISTA ENLAZADA SIMPLE: REGISTRO GENERAL
// ==========================================
class ListaEstudiantes {
private:
    NodoEstudiante* cabeza; // Puntero al inicio de la lista
public:
    ListaEstudiantes() { cabeza = NULL; }

    // Insertar: Agrega al estudiante al inicio de la lista
    void insertar(Estudiante p) {
        NodoEstudiante* nuevo = new NodoEstudiante();
        nuevo->dato = p;
        nuevo->siguiente = cabeza; // El nuevo nodo empuja al resto
        cabeza = nuevo;            // La cabeza ahora es el nuevo nodo
        cout << "[Registro] Estudiante guardado en el sistema.\n";
    }

    // Eliminar: Busca un ID y borra el nodo que lo contiene
    void eliminar(int id) {
        if (cabeza == NULL) return; // Si está vacía, no hace nada

        // Si el elemento a eliminar es el primero (la cabeza)
        if (cabeza->dato.id == id) {
            NodoEstudiante* aux = cabeza;
            cabeza = cabeza->siguiente; // La cabeza pasa al segundo nodo
            delete aux;                 // Borramos el antiguo primer nodo
            cout << "[Registro] Estudiante eliminado.\n";
            return;
        }

        // Si el elemento está más adelante en la lista
        NodoEstudiante* actual = cabeza;
        // Buscamos el nodo ANTERIOR al que queremos eliminar
        while (actual->siguiente != NULL && actual->siguiente->dato.id != id) {
            actual = actual->siguiente;
        }
        
        // Si encontramos el nodo anterior
        if (actual->siguiente != NULL) {
            NodoEstudiante* aux = actual->siguiente; // Nodo a eliminar
            actual->siguiente = aux->siguiente;      // "Saltamos" el nodo a eliminar para desenlazarlo
            delete aux;                              // Liberamos memoria
            cout << "[Registro] Estudiante eliminado.\n";
        } else {
            cout << "[Registro] ID no encontrado.\n"; // Llegamos al final y no estaba
        }
    }

    // Buscar: Recorre la lista y retorna un puntero al nodo si lo encuentra
    NodoEstudiante* buscar(int id) {
        NodoEstudiante* actual = cabeza;
        while (actual != NULL) {
            if (actual->dato.id == id) return actual; // ¡Lo encontró!
            actual = actual->siguiente;               // Pasa al siguiente
        }
        return NULL; // Retorna nulo si no lo encontró
    }

    // Actualiza la prioridad académica de un estudiante existente
    void modificarPrioridad(int id, int nuevaPrioridad) {
        NodoEstudiante* p = buscar(id); // Reutilizamos el método buscar
        if (p != NULL) {
            p->dato.prioridad = nuevaPrioridad;
            cout << "[Registro] Prioridad academica actualizada.\n";
        } else {
            cout << "[Registro] Estudiante no encontrado.\n";
        }
    }

    // Imprime todo el registro
    void mostrarTodo() {
        NodoEstudiante* actual = cabeza;
        if (cabeza == NULL) {
            cout << "\n[Registro] No hay estudiantes registrados.\n";
            return;
        }
        cout << "\n--- REGISTRO GENERAL DE ESTUDIANTES ---\n";
        while (actual != NULL) {
            cout << "ID: " << actual->dato.id << " | Nombre: " << actual->dato.nombre 
                 << " | Prioridad: " << actual->dato.prioridad << "\n";
            actual = actual->siguiente;
        }
    }

    // Persistencia: Guarda la lista en un archivo .txt
    void guardarEnArchivo(const string& nombreArchivo) {
        ofstream archivo(nombreArchivo.c_str()); // Abre o crea el archivo en modo escritura
        NodoEstudiante* actual = cabeza;
        while (actual != NULL) {
            // Escribe en el archivo separado por espacios
            archivo << actual->dato.id << " " << actual->dato.nombre << " " << actual->dato.prioridad << "\n";
            actual = actual->siguiente;
        }
        archivo.close(); // Siempre cerrar el archivo al terminar
        cout << "[Sistema] Datos guardados exitosamente en " << nombreArchivo << ".\n";
    }

    // Persistencia: Carga la lista desde un archivo .txt
    void cargarDesdeArchivo(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo.c_str()); // Abre el archivo en modo lectura
        if (!archivo.is_open()) {
            cout << "[Sistema] No se encontro archivo previo. Se iniciara una base de datos limpia.\n";
            return; 
        }
        
        // Limpiamos la lista actual en memoria para evitar duplicados al recargar
        while(cabeza != NULL) eliminar(cabeza->dato.id);

        Estudiante p;
        // Lee id, nombre y prioridad iterativamente hasta que el archivo acabe
        while (archivo >> p.id >> p.nombre >> p.prioridad) {
            insertar(p);
        }
        archivo.close();
        cout << "[Sistema] Base de datos cargada correctamente.\n";
    }
};

// ==========================================
// MENÚ PRINCIPAL (PROGRAMA)
// ==========================================
int main() {
  
    ListaEstudiantes registroGeneral;
    ColaMatricula filaEspera;
    PilaHistorial historial;
    
    // NOTA: Se ha quitado la carga automática inicial para que sea manual desde el menú.

    int op;
    do {
        cout <<"==== UNIVERSIDAD CONTINENTAL ===="<<endl;
        cout <<"======= sistema de matricula  ======"<<endl;
        cout << "1.  Registrar estudiante"<<endl;
        cout << "2.  Eliminar estudiante del registro"<<endl;
        cout << "3.  Modificar prioridad academica"<<endl;
        cout << "4.  Ver todos los estudiantes"<<endl;
        cout << "5.  Enviar estudiante a fila de matricula"<<endl;
        cout << "6.  Procesar siguiente matricula "<<endl;
        cout << "7.  Ver fila de espera"<<endl;
        cout << "8.  Deshacer ultima matricula"<<endl;
        cout << "9.  Ver historial de tramites"<<endl;
        cout << "10. Guardar datos en archivo"<<endl;
        cout << "11. Cargar datos de archivo"<<endl;
        cout << "12. Salir del programa"<<endl;
        cout << "Ingrese opcion:  ";
        cin >> op;

        switch(op){

            case 1:
                Estudiante p;
                cout << "ID (DNI o Codigo): "; cin >> p.id;
                cout << "Nombre (Sin espacios): "; cin >> p.nombre;
                cout << "Prioridad (1 Tercio Superior - 5 Regular): "; cin >> p.prioridad;
                registroGeneral.insertar(p); // Llamada al método de la lista
            
            case 2:
                
                int id; cout << "ID a eliminar: "; cin >> id;
                registroGeneral.eliminar(id);

            case 3:

                int id, prio; 
                cout << "ID del estudiante: "; cin >> id;
                cout << "Nueva prioridad: "; cin >> prio;
                registroGeneral.modificarPrioridad(id, prio);

            case 4:

               registroGeneral.mostrarTodo();

            case 5:

                int id; cout << "ID del estudiante a encolar: "; cin >> id;
                NodoEstudiante* nodo = registroGeneral.buscar(id); // Primero verifica que exista
                if (nodo != NULL) {
                    filaEspera.encolar(nodo->dato); // Si existe, pasa una copia de los datos a la cola
                } else {
                    cout << " El estudiante no existe en el registro general.\n";
                }

            case 6:

               Estudiante procesado = filaEspera.desencolar(); // Saca de la cola
               if(procesado.id != -1) { // Verifica que desencolar() no haya devuelto el error por defecto
               historial.push(procesado); // Envía los datos a la Pila de historial
            }

            case 7:
                  filaEspera.mostrar();
            case 8:
                  historial.pop();
            case 9:
                  historial.mostrar();
            case 10:
                 registroGeneral.guardarEnArchivo("estudiantes.txt");
            case 11:
                registroGeneral.cargarDesdeArchivo("estudiantes.txt");
            case 12:
                cout << "Saliendo del sistema...\n";
            default: 
               cout<<"error de dato"<<endl;
        }

        
    } while (opcion != 12); 

    return 0; 
}
