// =====================================================================
// HashGenerator.cpp
// Sistema de Registro y Verificación de Contratos Digitales
// Generador de identificadores únicos usando hashing polinómico base 37
// =====================================================================

#include <string>
using namespace std;

/*
 * Función: generateHash
 * Descripción: Genera un valor hash entero a partir de una clave tipo string.
 * Entrada:
 *   - key: la clave que representa un contrato (ej: nombre|fecha|tipo)
 *   - tableSize: tamaño de la tabla hash
 * Salida:
 *   - un número entero que representa el índice hash
 */
unsigned int generateHash(const string& key, int tableSize) {
    unsigned int hashValue = 0;
    for (int i = 0; i < key.length(); i++) {
        hashValue = 37 * hashValue + key[i]; // función polinómica base 37
    }
    return hashValue % tableSize; // se limita al tamaño de la tabla
}
