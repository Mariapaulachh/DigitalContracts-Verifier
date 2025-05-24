// =====================================================================
// HashTable.h
// Tabla Hash con Encadenamiento Separado para almacenar contratos
// Proyecto de Algoritmos - Daniel Felipe Mosquera y María Paula Chaparro
// =====================================================================

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <string>
#include "HashGenerator.cpp" // Para usar generateHash()
#include "main.cpp" // Solo si Contract está definido ahí. Idealmente usar un .h

using namespace std;

// Nodo genérico para la lista
template <typename T>
class Node {
private:
    T data;
    Node<T>* next;
public:
    Node(const T& d) : data(d), next(nullptr) {}

    void setNext(Node<T>* n) {
        next = n;
    }

    Node<T>* getNext() const {
        return next;
    }

    T getData() const {
        return data;
    }

    string to_string() const {
        return "[Contrato ID: " + data.id + "]";
    }
};

// Lista genérica con inserción al frente
template <typename T>
class Lista {
private:
    Node<T>* head;
public:
    Lista() : head(nullptr) {}

    int insert(const T& val) {
        if (search(val.id) != nullptr) return 0; // ya existe
        Node<T>* nuevo = new Node<T>(val);
        nuevo->setNext(head);
        head = nuevo;
        return 1; // insertado
    }

    T* search(const string& id) const {
        Node<T>* temp = head;
        while (temp != nullptr) {
            if (temp->getData().id == id) {
                return new T(temp->getData()); // copia del contrato
            }
            temp = temp->getNext();
        }
        return nullptr; // no encontrado
    }

    int remove(const string& id) {
        Node<T>* temp = head;
        Node<T>* prev = nullptr;
        while (temp != nullptr) {
            if (temp->getData().id == id) {
                if (prev != nullptr) {
                    prev->setNext(temp->getNext());
                } else {
                    head = temp->getNext();
                }
                delete temp;
                return 1; // eliminado
            }
            prev = temp;
            temp = temp->getNext();
        }
        return 0; // no encontrado
    }

    void print() const {
        Node<T>* temp = head;
        while (temp != nullptr) {
            cout << temp->to_string() << " -> ";
            temp = temp->getNext();
        }
        cout << "NULL" << endl;
    }
};

// Clase HashTable que usa encadenamiento separado
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
        for (auto it = c.parties.begin(); it != c.parties.end(); ++it) {
            clave += *it + "|";
        }
        clave += c.date + "|" + c.type;

        int pos = generateHash(clave, tableSize);
        return table[pos].insert(c);
    }

    Contract* search(const string& id) const {
        // Extraer hash index directamente desde el ID
        if (id.substr(0,3) != "ID-") return nullptr;
        int pos = stoi(id.substr(3)) % tableSize;
        return table[pos].search(id);
    }

    int remove(const string& id) {
        if (id.substr(0,3) != "ID-") return 0;
        int pos = stoi(id.substr(3)) % tableSize;
        return table[pos].remove(id);
    }

    void printTable() const {
        for (int i = 0; i < tableSize; i++) {
            cout << "[" << i << "] ";
            table[i].print();
        }
    }
};

#endif
