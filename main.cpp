// ======================================================================
// Sistema de Registro y Verificación de Contratos Digitales /
// Digital Contract Registration and Verification System
// Proyecto para la clase de Algoritmos y Estructuras de Datos
// Universidad del Rosario, Colombia 🇨🇴
// Desarrollado por: Maria Paula Chaparro y Daniel Felipe Mosquera
// ======================================================================

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include "HashTable.h"
#include "HashGenerator.cpp"

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

// -------------------- Funciones --------------------

void registerContract(AVLTree& avl, HashTable& hashTable) {
    Contract newContract;

    cout << "\n--- Registrar contrato / Register contract---\n";
    cout << "Fecha (YYYY-MM-DD): ";
    cin >> newContract.date;

    cout << "Tipo (préstamo/seguro/inversión): ";
    cin.ignore();
    getline(cin, newContract.type);

    cout << "Partes (ingrese 'fin' para terminar):\n";
    string party;
    while (true) {
        cout << "Nombre de la parte: ";
        getline(cin, party);
        if (party == "fin") break;
        newContract.parties.insert(party);
    }

    cout << "Cláusulas (ingrese 'fin' para terminar):\n";
    string clause;
    while (true) {
        cout << "Cláusula: ";
        getline(cin, clause);
        if (clause == "fin") break;
        newContract.clauses.push_back(clause);
    }

    string claveHash = "";
    for (const auto& p : newContract.parties) {
        claveHash += p + "|";
    }
    claveHash += newContract.date + "|" + newContract.type;

    int tableSize = 101;
    int hashIndex = generateHash(claveHash, tableSize);
    newContract.id = "ID-" + to_string(hashIndex);

    hashTable.insert(newContract);
    avl.insert(newContract.date, newContract.id);

    cout << "\n✅ Contrato registrado:\n";
    cout << "ID: " << newContract.id << endl;
    cout << "Fecha: " << newContract.date << endl;
    cout << "Tipo: " << newContract.type << endl;
    cout << "Partes: ";
    for (const auto& p : newContract.parties) cout << p << ", ";
    cout << "\nCláusulas: " << newContract.clauses.size() << " registradas\n";
}

void searchByDate(const AVLTree& avl) {
    string date;
    cout << "Ingrese fecha (YYYY-MM-DD): ";
    cin >> date;

    string id = avl.search(date);
    if (id.empty()) {
        cout << "⚠️ No hay contratos en esta fecha\n";
    } else {
        cout << "\n🔍 Contrato encontrado:\n";
        cout << "ID: " << id << endl;
        cout << "Fecha: " << date << endl;
    }
}

void displayContractDetails(const Contract& c) {
    cout << "\n🔎 Detalles del contrato:\n";
    cout << "ID: " << c.id << endl;
    cout << "Fecha: " << c.date << endl;
    cout << "Tipo: " << c.type << endl;
    cout << "Partes:\n";
    for (const auto& p : c.parties) {
        cout << "- " << p << endl;
    }
    cout << "Cláusulas:\n";
    for (const auto& cl : c.clauses) {
        cout << "• " << cl << endl;
    }
}

void displayMenu() {
    cout << "\n📜 **Sistema de Contratos Digitales** / **Digital Contract System**\n";
    cout << "1. Registrar contrato / Register contract\n";
    cout << "2. Buscar por fecha / Search by date\n";
    cout << "3. Buscar por ID / Search by ID\n";
    cout << "4. Mostrar por tipo / Display by type\n";
    cout << "5. Salir / Exit\n";
    cout << "Opción: ";
}

// -------------------- Función Principal --------------------

int main() {
    AVLTree contractsByDate;
    HashTable hashTable;
    int option;

    do {
        displayMenu();
        cin >> option;

        switch (option) {
            case 1:
                registerContract(contractsByDate, hashTable);
                break;
            case 2:
                searchByDate(contractsByDate);
                break;
            case 3: {
                string id;
                cout << "Ingrese el ID del contrato: ";
                cin >> id;
                Contract* c = hashTable.search(id);
                if (c != nullptr) {
                    displayContractDetails(*c);
                    delete c;
                } else {
                    cout << "⚠️ Contrato no encontrado.\n";
                }
                break;
            }
            case 4: {
                string tipo;
                cin.ignore();
                cout << "Ingrese el tipo de contrato (préstamo / seguro / inversión): ";
                getline(cin, tipo);
                hashTable.displayByType(tipo);
                break;
            }
            case 5:
                cout << "Saliendo del sistema. ¡Hasta luego!\n";
                break;
            default:
                cout << "Opción inválida / Invalid option\n";
        }

    } while (option != 5);

    return 0;
}
