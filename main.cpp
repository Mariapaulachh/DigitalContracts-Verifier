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

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

// -------------------- Clase AvlTree/Class AVLTree --------------------
class AVLTree {
private:
    struct AvlNode {
        string date;        
        string contractId;  
        AvlNode* left;
        AvlNode* right;
        int height;

        AvlNode(const string& d, const string& id) 
            : date(d), contractId(id), left(nullptr), right(nullptr), height(0) {}
    };

    AvlNode* root;
    static const int ALLOWED_IMBALANCE = 1;


    int height(AvlNode* t) const {
        return t == nullptr ? -1 : t->height;
    }

    void rotateWithLeftChild(AvlNode*& k2) {
        AvlNode* k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1->height = max(height(k1->left), k2->height) + 1;
        k2 = k1;
    }

    void rotateWithRightChild(AvlNode*& k1) {
        AvlNode* k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        k2->height = max(height(k2->right), k1->height) + 1;
        k1 = k2;
    }

    void doubleWithLeftChild(AvlNode*& k3) {
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }

    void doubleWithRightChild(AvlNode*& k1) {
        rotateWithLeftChild(k1->right);
        rotateWithRightChild(k1);
    }

    void balance(AvlNode*& t) {
        if (!t) return;

        if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE) {
            if (height(t->left->left) >= height(t->left->right))
                rotateWithLeftChild(t);
            else
                doubleWithLeftChild(t);
        }
        else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE) {
            if (height(t->right->right) >= height(t->right->left))
                rotateWithRightChild(t);
            else
                doubleWithRightChild(t);
        }

        t->height = max(height(t->left), height(t->right)) + 1;
    }

    void insert(const string& date, const string& id, AvlNode*& t) {
        if (!t) {
            t = new AvlNode(date, id);
        } 
        else if (date < t->date) {
            insert(date, id, t->left);
        }
        else if (date > t->date) {
            insert(date, id, t->right);
        }
        balance(t);
    }

    string search(const string& date, AvlNode* t) const {
        if (!t) return "";
        if (date < t->date) return search(date, t->left);
        else if (date > t->date) return search(date, t->right);
        else return t->contractId;
    }

public:
    AVLTree() : root(nullptr) {}
    
    void insert(const string& date, const string& id) {
        insert(date, id, root);
    }

    string search(const string& date) const {
        return search(date, root);
    }
};

// Estructura de un contrato / Contract structure. MP Chaparro, DF Mosquera.
struct Contract {
    string id;          // ID único / Unique ID. MP Chaparro, DF Mosquera.
    string date;        // Fecha de creación (YYYY-MM-DD) / Creation date. MP Chaparro, DF Mosquera.
    string type;        // Tipo: "préstamo", "seguro", "inversión" / Type: "loan", "insurance", "investment". MP Chaparro, DF Mosquera.
    set<string> parties; // Partes involucradas / Involved parties. MP Chaparro, DF Mosquera.
    vector<string> clauses; // Cláusulas / Contract clauses. MP Chaparro, DF Mosquera.
};

//  Funciones AVL/ AVL Functions
string generateTempId(const string& date, const string& type, const set<string>& parties); // Provisional
void registerContract(AVLTree& avl);
void searchByDate(const AVLTree& avl);
void displayContractDetails(const string& id); 

// Implementaciones  --- MEJORAR 
string generateTempId(const string& date, const string& type, const set<string>& parties) {
    // ID provisional ( reemplazar por el hash)
    return "TEMP-" + date.substr(0,4) + type.substr(0,3) + to_string(parties.size());
}

void registerContract(AVLTree& avl) {
    Contract newContract;
    
    // 1. Capturar todos los datos/Capture all data
    
    cout << "\n--- Registrar contrato / Register contract---\n";
    cout << "Fecha (YYYY-MM-DD)/ Enter date (YYYY-MM-DD): ";
    cin >> newContract.date;
    
    cout << "Tipo (préstamo/seguro/inversión)/Type (loan/insurance/investment): ";
    cin.ignore();
    getline(cin, newContract.type);
    
    cout << "Partes (ingrese 'fin' para terminar)/Parties (enter 'end' to end):\n";
    string party;
    while(true) {
        cout << "Nombre de la parte/ Name of the party: ";
        getline(cin, party);
        if(party == "fin") break;
        newContract.parties.insert(party);
    }
    
    cout << "Cláusulas (ingrese 'fin' para terminar)/Clauses (enter 'end' to end):\n";
    string clause;
    while(true) {
        cout << "Cláusula/ Clause: ";
        getline(cin, clause);
        if(clause == "fin") break;
        newContract.clauses.push_back(clause);
    }
    
    // 2. Generar ID provisional (reemplazar con el hash)
    newContract.id = generateTempId(newContract.date, newContract.type, newContract.parties);
    
    // 3. Almacenar en AVL (solo fecha e ID TEMPORAL por ahora)
    avl.insert(newContract.date, newContract.id);
    
    // 4. Mostrar resumen (esto luego se integrará con HashTable)
    cout << "\n✅ Contrato registrado:\n";
    cout << "ID: " << newContract.id << endl;
    cout << "Fecha/Date: " << newContract.date << endl;
    cout << "Tipo/Type: " << newContract.type << endl;
    cout << "Partes/Parties: ";
    for(const auto& p : newContract.parties) cout << p << ", ";
    cout << "\nCláusulas/ Clauses:" << newContract.clauses.size() << " registradas\n";
}

void searchByDate(const AVLTree& avl) {
    string date;
    cout << "Ingrese fecha (YYYY-MM-DD)/Enter date (YYYY-MM-DD): ";
    cin >> date;
    
    string id = avl.search(date);
    if(id.empty()) {
        cout << "⚠️ No hay contratos en esta fecha/ There are no contracts on this date\n";
    } else {
        cout << "\n🔍 Contrato encontrado / Contract found:\n";
        // Esto se conectará con HashTable para mostrar todos los datos
        cout << "ID: " << id << endl;
        cout << "Fecha/Date: " << date << endl;
        // ( más detalles desde HashTable)
    }
}

// -------------------- Generador de Hash / Hash Generator --------------------



// -------------------- Tabla Hash  / Hash Table --------------------





// -------------------- Menú principal / Main menu--------------------

void displayMenu() {
    cout << "\n📜 **Sistema de Contratos Digitales** / **Digital Contract System**\n";
    cout << "1. Registrar contrato / Register contract\n";
    cout << "2. Buscar por fecha / Search by date\n";
    cout << "3. Buscar por ID / Search by ID\n";
    cout << "4. Mostrar por tipo / Display by type\n";
    cout << "5. Salir / Exit\n";
    cout << "Opción / Choice: ";
}

// -------------------- Función Principal/ Main Function--------------------
int main() {
    AVLTree contractsByDate;
    int option;

    do {
        displayMenu();
        cin >> option;

        switch(option) {
            case 1: 
                registerContract(contractsByDate); 
                break;
            case 2: 
                searchByDate(contractsByDate); 
                break;
            case 3: 
                cout << "Salir / Exit\n";
                break;
            default: 
                cout << "Opción inválida/Invalid option\n";
        }
    } while(option != 3);

    return 0;
}
