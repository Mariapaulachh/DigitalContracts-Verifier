/*
 * ======================================================================
 * Sistema de Registro y Verificación de Contratos Digitales /
 * Digital Contract Registration and Verification System
 * 
 * Proyecto para la clase de Algoritmos y Estructuras de Datos /
 * Project for Algorithms and Data Structures Class
 * 
 * Universidad del Rosario, Colombia 🇨🇴
 * 
 * Desarrollado por / Developed by:
 * - Maria Paula Chaparro
 * - Daniel Felipe Mosquera
 * ======================================================================
 */

// Estructura de un contrato / Contract structure. MP Chaparro, DF Mosquera.
struct Contract {
    string id;          // ID único / Unique ID. MP Chaparro, DF Mosquera.
    string date;        // Fecha de creación (YYYY-MM-DD) / Creation date. MP Chaparro, DF Mosquera.
    string type;        // Tipo: "préstamo", "seguro", "inversión" / Type: "loan", "insurance", "investment". MP Chaparro, DF Mosquera.
    set<string> parties; // Partes involucradas / Involved parties. MP Chaparro, DF Mosquera.
    vector<string> clauses; // Cláusulas / Contract clauses. MP Chaparro, DF Mosquera.
};

// --- Prototipos de funciones / Function prototypes ---. MP Chaparro, DF Mosquera.
void registerContract(HashTable& ht, AVLTree& avl, MultiList& ml); // Registra nuevo contrato / Registers new contract. MP Chaparro, DF Mosquera.
void searchByDate(const AVLTree& avl); // Busca por fecha / Searches by date. MP Chaparro, DF Mosquera.
void searchById(const HashTable& ht); // Busca por ID / Searches by ID. MP Chaparro, DF Mosquera.
void showContractsByType(const MultiList& ml); // Muestra por tipo / Displays by type. MP Chaparro, DF Mosquera.

// Menú principal / Main menu. MP Chaparro, DF Mosquera.

void displayMenu() {
    cout << "\n📜 **Sistema de Contratos Digitales** / **Digital Contract System**\n";
    cout << "1. Registrar contrato / Register contract\n";
    cout << "2. Buscar por fecha / Search by date\n";
    cout << "3. Buscar por ID / Search by ID\n";
    cout << "4. Mostrar por tipo / Display by type\n";
    cout << "5. Salir / Exit\n";
    cout << "Opción / Choice: ";
}
