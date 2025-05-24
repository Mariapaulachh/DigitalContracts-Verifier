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

//#include "HashTable.h"
//#include "HashGenerator.cpp"

using namespace std;

// ---------- Estructura de un contrato / Contract structure -----------
struct Contract {
    string id;          // ID único / Unique ID. MP Chaparro, DF Mosquera.
    string date;        // Fecha de creación (YYYY-MM-DD) / Creation date. MP Chaparro, DF Mosquera.
    string type;        // Tipo: "préstamo", "seguro", "inversión" / Type: "loan", "insurance", "investment". MP Chaparro, DF Mosquera.
    set<string> parties; // Partes involucradas / Involved parties. MP Chaparro, DF Mosquera.
    vector<string> clauses; // Cláusulas / Contract clauses. MP Chaparro, DF Mosquera.
};
// ---------------------------------------------------------------------d

// ------------ Lista enlazada simple / Singly linked list  -------------
template <typename T>
class Node {
private:
    T data;
    Node<T>* next;
public:
    Node(const T& d) : data(d), next(nullptr) {}
    void setNext(Node<T>* n) { next = n; }
    Node<T>* getNext() const { return next; }
    T getData() const { return data; }
    string to_string() const { return "[ID: " + data.id + "]"; }
};

template <typename T>
class Lista {
private:
    Node<T>* head;
public:
    Lista() : head(nullptr) {}

    int insert(const T& val) {
        Node<T>* nuevo = new Node<T>(val);
        nuevo->setNext(head);
        head = nuevo;
        return 1;
    }

    T* search(const string& id) const {
        Node<T>* temp = head;
        while (temp != nullptr) {
            if (temp->getData().id == id)
                return new T(temp->getData());
            temp = temp->getNext();
        }
        return nullptr;
    }

    int remove(const string& id) {
        Node<T>* temp = head;
        Node<T>* prev = nullptr;
        while (temp != nullptr) {
            if (temp->getData().id == id) {
                if (prev) prev->setNext(temp->getNext());
                else head = temp->getNext();
                delete temp;
                return 1;
            }
            prev = temp;
            temp = temp->getNext();
        }
        return 0;
    }

    void print() const {
        Node<T>* temp = head;
        while (temp != nullptr) {
            cout << temp->to_string() << " -> ";
            temp = temp->getNext();
        }
        cout << "NULL" << endl;
    }

    Node<T>* getHead() const { return head; }
};
// ---------------------------------------------------------------------


string normalizarTexto(string input); // declaración anticipada
bool esFechaValida(string& fecha);  // sin const
 // delcaracion anticipada



// ------- Captura de datos del contrato/Capture contract data ----------
Contract captureContractData() {
    Contract newContract;

    cout << "\n--- Registrar contrato / Register contract---\n";
    do {
        cout << "Fecha (YYYY-MM-DD o YYYY/MM/DD): ";
            getline(cin, newContract.date);
            if (!esFechaValida(newContract.date)) {
                cout << "⚠️ Formato inválido. Intente de nuevo (YYYY-MM-DD o YYYY/MM/DD).\n";
            }
    } while (!esFechaValida(newContract.date));



    cout << "Tipo (préstamo/seguro/inversión): ";
    cin.ignore();
    getline(cin, newContract.type);
    newContract.type = normalizarTexto(newContract.type);


    cout << "Partes (ingrese 'fin' para terminar):\n";
    string party;
    while (true) {
        cout << "Nombre de la parte: ";
        getline(cin, party);
        string partyCheck = normalizarTexto(party);
        if (partyCheck == "FIN") break;
        newContract.parties.insert(partyCheck);

    }

    cout << "Cláusulas (ingrese 'fin' para terminar):\n";
    string clause;
    while (true) {
        cout << "Cláusula: ";
        getline(cin, clause);
        string clauseCheck = normalizarTexto(clause);
        if (clauseCheck == "FIN") break;
        newContract.clauses.push_back(clauseCheck);
    }
    return newContract;
}
// ---------------------------------------------------------------------


string normalizarTexto(string input) {
    // 1. Eliminar espacios al inicio y final
    size_t start = input.find_first_not_of(" \t\n\r");
    size_t end = input.find_last_not_of(" \t\n\r");
    if (start == string::npos) return "";
    input = input.substr(start, end - start + 1);

    // 2. Reemplazar tildes y convertir a mayúsculas (sin switch)
    string resultado = "";
    for (size_t i = 0; i < input.length(); ++i) {
        unsigned char c = input[i];
        // Detección de multibyte de tilde
        if (c == 195 && i + 1 < input.length()) {
            unsigned char next = input[i + 1];
            if (next == 161) resultado += 'A'; // á
            else if (next == 169) resultado += 'E'; // é
            else if (next == 173) resultado += 'I'; // í
            else if (next == 179) resultado += 'O'; // ó
            else if (next == 186) resultado += 'U'; // ú
            else if (next == 177) resultado += 'N'; // ñ
            else resultado += '?'; // caracter raro
            i++; // saltar el segundo byte
        } else {
            resultado += toupper(c);
        }
    }

    return resultado;
}









// -------------------- Clase AvlTree/Class AVLTree --------------------
class AVLTree {
private:
    struct AvlNode {
        string date;
        vector<string> contractIds;  // Cambiamos a vector para múltiples IDs
        AvlNode* left;
        AvlNode* right;
        int height;

        AvlNode(const string& d, const string& id) 
            : date(d), left(nullptr), right(nullptr), height(0) {
            contractIds.push_back(id);
        }
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
            else {  // Fecha igual, añadimos el ID al vector
                t->contractIds.push_back(id);
            }
            balance(t);
        }

        vector<string> search(const string& date, AvlNode* t) const {
            if (!t) return {};
            if (date < t->date) return search(date, t->left);
            else if (date > t->date) return search(date, t->right);
            else return t->contractIds;
        }

    public:
        AVLTree() : root(nullptr) {}

        void insert(const string& date, const string& id) {
            insert(date, id, root);
        }

        vector<string> search(const string& date) const {
            return search(date, root);
        }
    };

// ---------------------------------------------------------------------

// ----------------- Generador de Hash / Hash Generator -----------------

unsigned int generateHash(const string& key, int tableSize) {
    unsigned int hashValue = 0;
    for (int i = 0; i < key.length(); i++) {
        hashValue = 37 * hashValue + key[i];
    }
    return hashValue % tableSize;
}
// ---------------------------------------------------------------------

string toBase36(unsigned int num) {
    const string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string result = "";
    do {
        result = chars[num % 36] + result;
        num /= 36;
    } while (num > 0);
    return result;
}

string generateUniqueId(const Contract& c, int tableSize = 101) {
    string prefix;
    if (c.type == "PRESTAMO")      prefix = "PRE-";
    else if (c.type == "SEGURO")   prefix = "SEG-";
    else if (c.type == "INVERSION")prefix = "INV-";
    else                           prefix = "GEN-";


    string clave = "";
    for (const auto& p : c.parties)    clave += p + "|";
    clave += c.date + "|" + c.type + "|";
    for (const auto& cl : c.clauses)   clave += cl + "|";

    unsigned int hashValue = 0;
    for (char ch : clave) {
        hashValue = 37 * hashValue + ch;
    }

    string encoded = toBase36(hashValue % tableSize);
    return prefix + encoded;
}





// -------------------- Clase HashTable/ Class HashTable ---------------
// HashGenerator.cpp 
class HashTable {
private:
    vector<Lista<Contract>> table;
    int tableSize;

public:
    HashTable(int size = 101) {
        tableSize = size;
        table.resize(tableSize);
    }

    int insert(const Contract& c) {
        string clave = "";
        for (const auto& p : c.parties) clave += p + "|";
        clave += c.date + "|" + c.type;
        int pos = generateHash(clave, tableSize);
        return table[pos].insert(c);
    }

    Contract* search(const string& id) const {
        if (id.substr(0, 3) != "ID-") return nullptr;
        int pos = stoi(id.substr(3)) % tableSize;
        return table[pos].search(id);
    }

    void displayByType(const string& type) const {
        for (int i = 0; i < tableSize; i++) {
            Node<Contract>* temp = table[i].getHead();
            while (temp != nullptr) {
                if (temp->getData().type == type) {
                    Contract c = temp->getData();
                    cout << "\n🧾 Contrato ID: " << c.id << "\n";
                    cout << "Fecha: " << c.date << "\n";
                    cout << "Partes: ";
                    for (const auto& p : c.parties) cout << p << ", ";
                    cout << "\nCláusulas:\n";
                    for (const auto& cl : c.clauses) cout << "- " << cl << "\n";
                    cout << "---------------------------\n";
                }
                temp = temp->getNext();
            }
        }
    }
};
// ---------------------------------------------------------------------

bool esFechaValida(string& fecha) {
    // Eliminar espacios alrededor
    size_t start = fecha.find_first_not_of(" \t\n\r");
    size_t end = fecha.find_last_not_of(" \t\n\r");
    if (start == string::npos) return false;
    fecha = fecha.substr(start, end - start + 1);

    // Reemplazar / por - si es necesario
    for (char& ch : fecha) {
        if (ch == '/') ch = '-';
    }

    // Validar formato YYYY-MM-DD
    if (fecha.length() != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;

    string anio = fecha.substr(0, 4);
    string mes  = fecha.substr(5, 2);
    string dia  = fecha.substr(8, 2);

    for (char c : anio + mes + dia) {
        if (!isdigit(c)) return false;
    }

    int m = stoi(mes);
    int d = stoi(dia);

    if (m < 1 || m > 12) return false;
    if (d < 1 || d > 31) return false;

    return true;
}




// ---------- Registro de contrato AVL/Resgister contract AVL ----------

void registerContract(AVLTree& avl, HashTable& hashTable) {
    Contract newContract = captureContractData();
    
    string clave = "";
    for (const auto& p : newContract.parties) clave += p + "|";
    clave += newContract.date + "|" + newContract.type;
    newContract.id = generateUniqueId(newContract);
    cout << "\n🔑 ID generado: " << newContract.id << "\n";

    avl.insert(newContract.date, newContract.id);
    hashTable.insert(newContract); // Aquí se guarda el contrato completo

    cout << "\n✅ Contrato registrado correctamente/Contract successfully registered:\n";
}
// ---------------------------------------------------------------------

//---Función de búsqueda con fecha AVL/Search function with AVL date----

    void searchByDate(const AVLTree& avl, const HashTable& hashTable) {
        string date;
        cout << "\n🔍 Buscar por fecha (YYYY-MM-DD)/ Search by date: ";
        cin >> date;
    
        vector<string> ids = avl.search(date);
        if (ids.empty()) {
            cout << "⚠️ No se encontró contrato con esa fecha/There are no contracts on this date\n";
        } else {
            cout << "📅 Contratos encontrados en esta fecha/Contracts found on this date:\n";
            for (const string& id : ids) {
                Contract* c = hashTable.search(id);
                if (c) {
                    cout << "\n🧾 Contrato ID: " << c->id << "\n";
                    cout << "Fecha: " << c->date << "\n";
                    cout << "Tipo: " << c->type << "\n";
                    cout << "Partes: ";
                    for (const auto& p : c->parties) cout << p << ", ";
                    cout << "\nCláusulas:\n";
                    for (const auto& cl : c->clauses) cout << "- " << cl << "\n";
                    cout << "---------------------------\n";
                    delete c;
                } else {
                    cout << "⚠️ Contrato con ID " << id << " no encontrado en la tabla hash.\n";
                }
            }
        }
    }
// ---------------------------------------------------------------------
    
// ------ Mostrar detalles del contrato / Display contract details  -----
void displayContractDetails(const Contract& c) {
    cout << "\n🔎 Detalles del contrato/ Contract details:\n";
    cout << "ID: " << c.id << "\n";
    cout << "Fecha/Date: " << c.date << "\n";
    cout << "Tipo/Type: " << c.type << "\n";
    cout << "Partes/Parties:\n";
    for (const auto& p : c.parties) cout << "- " << p << "\n";
    cout << "Cláusulas/Clauses:\n";
    for (const auto& cl : c.clauses) cout << "• " << cl << "\n";
}
// ---------------------------------------------------------------------

// -------------------- Menú principal / Main menu----------------------
void displayMenu() {
    cout << "\n📜 **Sistema de Contratos Digitales** / **Digital Contract System**\n";
    cout << "1. Registrar contrato / Register contract.\n";
    cout << "2. Buscar por fecha / Search by date.\n";
    cout << "3. Buscar por ID / Search by ID.\n";
    cout << "4. Salir / Exit.\n";
    cout << "Opción / Choice: ";
}
// ---------------------------------------------------------------------

// ------------------ Función Principal/ Main Function------------------
int main() {
    void registerContract(AVLTree& avl, HashTable& hashTable);
    void searchByDate(const AVLTree& avl, const HashTable& hashTable);
    void displayContractDetails(const Contract& c);
    void displayMenu();

    AVLTree avl;           
    HashTable hashTable;
        
    int option;

    do {
        displayMenu();
        cin >> option;
        cin.ignore();
        
        switch(option) {
            case 1: 
                registerContract(avl, hashTable); 
                break;
            case 2: 
                searchByDate(avl, hashTable); 
                break;
            case 3: {
                    string id;
                    cout << "\n🔎 BUSCAR POR ID\nIngrese el ID: ";
                    getline(cin, id);
                    Contract* c = hashTable.search(id);
                        if (c) {
                            displayContractDetails(*c);
                                delete c;
                        } else {
                                cout << "⚠️ Contrato no encontrado.\n";
                        }
                            break;
                    }
            case 4:
                cout << "\n👋 Hasta luego/See you later.\n";
                break;
            default:
                cout << "Opción inválida/Invalid option.\n";
        }
    } while (option != 4);

    return 0;
}
