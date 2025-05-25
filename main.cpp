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


// ---------- Estructura de un contrato / Contract structure -----------
struct Contract {
    string id;          // ID único / Unique ID. MP Chaparro, DF Mosquera.
    string date;        // Fecha de creación (YYYY-MM-DD) / Creation date. MP Chaparro, DF Mosquera.
    string type;        // Tipo: "préstamo", "seguro", "inversión" / Type: "loan", "insurance", "investment". MP Chaparro, DF Mosquera.
    string normalizedType; // Tipo normalizado bilingue / Normalized bilingual type. MP Chaparro, DF Mosquera.
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

// ---------------------------------------------------------------------
// ------- Funciones de validación / Validation functions --------------
string normalizeText(string input); 
bool isValidDate(string& date);bool isValidDate(string& date);
string normalizeForSearch(const string& input);

// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
//--------- Normalización de texto de fecha / Date normalization --------
string normalizeText(string input) {
    size_t start = input.find_first_not_of(" \t\n\r");
    size_t end = input.find_last_not_of(" \t\n\r");
    if (start == string::npos) return "";
    input = input.substr(start, end - start + 1);

    string result = "";
    for (size_t i = 0; i < input.length(); ++i) {
        unsigned char c = input[i];
        // Detección de multibyte de tilde
        if (c == 195 && i + 1 < input.length()) {
            unsigned char next = input[i + 1];
            if (next == 161) result += 'A'; // á
            else if (next == 169) result += 'E'; // é
            else if (next == 173) result += 'I'; // í
            else if (next == 179) result += 'O'; // ó
            else if (next == 186) result += 'U'; // ú
            else if (next == 177) result += 'N'; // ñ
            else result += '?'; // caracter raro
            i++; // saltar el segundo byte
        } else {
            result += toupper(c);
        }
    }

    return result;
}
// ---------------------------------------------------------------------
//------------------- Validación de fecha / Date validation ------------

bool isValidDate(string& date) {
    size_t start = date.find_first_not_of(" \t\n\r");
    size_t end = date.find_last_not_of(" \t\n\r");
    if (start == string::npos) return false;
    date = date.substr(start, end - start + 1);

    // Reemplazar / por - si es necesario
    for (char& ch : date) {
        if (ch == '/') ch = '-';
    }

    // Validar formato YYYY-MM-DD
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;

    string year = date.substr(0, 4);
    string month = date.substr(5, 2);
    string day = date.substr(8, 2);

    for (char c : year + month + day) {
        if (!isdigit(c)) return false;
    }

    int y = stoi(year);
    int m = stoi(month);
    int d = stoi(day);

    if (y < 1960 || y > 2025) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > 31) return false;

    // Validar días por mes
    if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30) return false;

    // Validar febrero
    if (m == 2) {
        bool isLeap = (y % 400 == 0) || (y % 100 != 0 && y % 4 == 0);
        if (d > (isLeap ? 29 : 28)) return false;
    }

    return true;
}
// ---------------------------------------------------------------------

// --------- Normalización para búsqueda/ Normalize for search ---------
string normalizeForSearch(const string& input) {
    string normalized = normalizeText(input);

    // Mapeo de sinónimos (solo para búsqueda)
    if (normalized == "PRESTAMO") return "LOAN";
    if (normalized == "LOAN") return "PRESTAMO";
    if (normalized == "SEGURO") return "INSURANCE";
    if (normalized == "INSURANCE") return "SEGURO";
    if (normalized == "INVERSION") return "INVESTMENT";
    if (normalized == "INVESTMENT") return "INVERSION";

    return normalized;
}
// ---------------------------------------------------------------------

// ------- Captura de datos del contrato/Capture contract data ----------
Contract captureContractData() {
    Contract newContract;

    cout << "\n--- Registrar contrato / Register contract---\n";

    do {
        cout << "Fecha (AAAA-MM-DD)/ Date (YYYY-MM-DD) : ";
        getline(cin, newContract.date);
        if (!isValidDate(newContract.date)) {
            cout << "⚠️ Formato inválido. Intente de nuevo/Invalid format. Please try again.\n";
        }
    } while (!isValidDate(newContract.date));

    cout << "Tipo (préstamo/seguro/inversión)/ Type (loan/insurance/investment)/: ";
    getline(cin, newContract.type); // Guardamos el texto original (sin normalizar aún)

    // Mantener el tipo original para mostrar, pero validar con normalizado
    string normalizedType = normalizeText(newContract.type);
    if (!(normalizedType == "PRESTAMO" || normalizedType == "LOAN" || 
          normalizedType == "SEGURO" || normalizedType == "INSURANCE" ||
          normalizedType == "INVERSION" || normalizedType == "INVESTMENT")) {
        cout << "⚠️ Tipo inválido. Usando 'GENERAL' como predeterminado.\n";
        newContract.type = "GENERAL";
    }

    cout << "Partes (ingrese 'fin' para terminar)/Parties (enter 'end' to finish):\n";
    string party;
    while (true) {
        cout << "Nombre de la parte/ Name of the party:";
        getline(cin, party);
        string partyCheck = normalizeText(party);
        if (partyCheck == "FIN" || partyCheck == "END") break;
        newContract.parties.insert(partyCheck);
    }

    cout << "Cláusulas (ingrese 'fin' para terminar)/ Clauses (enter 'end' to finish):\n";
    string clause;
    while (true) {
        cout << "Cláusula/ Clause: ";
        getline(cin, clause);
        string clauseCheck = normalizeText(clause);
        if (clauseCheck == "FIN" || clauseCheck == "END") break;
        newContract.clauses.push_back(clauseCheck);
    }

    newContract.normalizedType = normalizeText(newContract.type);
    return newContract;

}
// ---------------------------------------------------------------------



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

string toBase62(unsigned long long num) {
    const string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    string result;
    do {
        result = chars[num % 62] + result;
        num /= 62;
    } while (num > 0);
    return result;
}

unsigned long long djb2Hash(const string& str) {
    unsigned long long hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

string generateContractFingerprint(const Contract& c) {
    string data;
    data.reserve(256); // 

    data += c.date + "|" + c.normalizedType + "|";
    for (const auto& p : c.parties) data += p + "|";
    for (const auto& cl : c.clauses) data += cl + "|";

    unsigned long long hash1 = djb2Hash(data);
    unsigned long long hash2 = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        hash2 = (hash2 * 31) + data[i] ^ (i % 17); // Variante con XOR
    }

    unsigned long long combinedHash = hash1 ^ (hash2 << 16);
    return toBase62(combinedHash).substr(0, 16); // ID de 16 caracteres / 16-char ID
}

string generateUniqueId(const Contract& c) {
    string prefix;
    string typeNormalized = c.normalizedType;

    if (typeNormalized == "PRESTAMO" || typeNormalized == "LOAN") prefix = "PRE-";
    else if (typeNormalized == "SEGURO" || typeNormalized == "INSURANCE") prefix = "SEG-";
    else if (typeNormalized == "INVERSION" || typeNormalized == "INVESTMENT") prefix = "INV-";
    else prefix = "GEN-";

    string fingerprint = generateContractFingerprint(c);
    fingerprint.erase(remove_if(fingerprint.begin(), fingerprint.end(), ::isspace), fingerprint.end());

    return prefix + fingerprint;
}
    
// ---------------------------------------------------------------------

class HashTable {
private:
    vector<Lista<Contract>> table;
    int tableSize;

public:
    HashTable(int size = 101) {
        tableSize = size;
        table.resize(tableSize);
    }

    // Método search que falta
    Contract* search(const string& id) const {
        unsigned long long hash = djb2Hash(id);
        int pos = hash % tableSize;
        return table[pos].search(id);
    }

    void displayByType(const string& type) {
        for (int i = 0; i < tableSize; i++) {
            Node<Contract>* temp = table[i].getHead();
            while (temp != nullptr) {
                if (temp->getData().type == type) {
                    Contract c = temp->getData();
                    cout << "\n🧾 Contrato ID / Contract ID: " << c.id << "\n";
                    cout << "Fecha / Date: " << c.date << "\n";
                    cout << "Partes / Parties: ";
                    for (const auto& p : c.parties) cout << p << ", ";
                    cout << "\nCláusulas/ Clauses:\n"; 
                    for (const auto& cl : c.clauses) cout << "- " << cl << "\n";
                    cout << "---------------------------\n";
                }
                temp = temp->getNext();
            }
        }
    }

    int insert(const Contract& c) {
        unsigned long long hash = djb2Hash(c.id); 
        int pos = hash % tableSize;
        return table[pos].insert(c);
    }
};
// ---------------------------------------------------------------------

// ---------------- Clase multilista/ Class multilist- -----------------
class MultiList {
private:
    struct TypeNode {
        std::string type;                
        std::vector<std::string> ids;    
        TypeNode* next;                  

        TypeNode(const std::string& t) : type(t), next(nullptr) {}
    };

    TypeNode* head;

public:
    MultiList() : head(nullptr) {}

    // Añadir contrato a la lista de su tipo
    void addContract(const std::string& type, const std::string& id) {
        TypeNode* current = head;
        while (current) {
            if (current->type == type) {
                current->ids.push_back(id);
                return;
            }
            current = current->next;
        }
        
        TypeNode* newNode = new TypeNode(type);
        newNode->ids.push_back(id);
        newNode->next = head;
        head = newNode;
    }

    std::vector<std::string> getContractsByType(const std::string& type) const {
        TypeNode* current = head;
        while (current) {
            if (current->type == type) return current->ids;
            current = current->next;
        }
        return {};
    }
};
// ---------------------------------------------------------------------

// -------------------- PROTOTIPOS GLOBALES --------------------------
void registerContract(AVLTree& avl, HashTable& ht, MultiList& ml);
void searchByDate(const AVLTree& avl, const HashTable& hashTable);
void displayContractDetails(const Contract& c);

void displayMenu();
// ---------------------------------------------------------------------
// ---------- Registro de contrato AVL/Resgister contract AVL ----------
void registerContract(AVLTree& avl, HashTable& hashTable, MultiList& ml) {
    Contract newContract = captureContractData();

    string normalizedType = normalizeText(newContract.type);
    newContract.id = generateUniqueId(newContract); // Internamente usa normalizedType

    cout << "\n🔑 ID generado: " << newContract.id << "\n";

    avl.insert(newContract.date, newContract.id);
    hashTable.insert(newContract);
    ml.addContract(normalizedType, newContract.id); // Usa el tipo normalizado

    cout << "\n✅ Contrato registrado correctamente.\n";
}
// ---------------------------------------------------------------------

//---Función de búsqueda con fecha AVL/Search function with AVL date----
    void searchByDate(const AVLTree& avl, const HashTable& hashTable) {
        string date;
        cout << "\n🔍 Buscar por fecha (YYYY-MM-DD)/ Search by date: ";
        cin >> date;
        cin.ignore();

        vector<string> ids = avl.search(date);
        if (ids.empty()) {
            cout << "⚠️ No se encontró contrato con esa fecha/There are no contracts on this date\n";
        } else {
            cout << "📅 Contratos encontrados en esta fecha/Contracts found on this date:\n";
            for (const string& id : ids) {
                Contract* c = hashTable.search(id);
                if (c) {
                    cout << "\n🧾 Contrato ID/ Contract ID: " << c->id << "\n";
                    cout << "Fecha: " << c->date << "\n";
                    cout << "Tipo: " << c->type << "\n";
                    cout << "Partes: ";
                    for (const auto& p : c->parties) cout << p << ", ";
                    cout << "\nCláusulas:\n";
                    for (const auto& cl : c->clauses) cout << "- " << cl << "\n";
                    cout << "---------------------------\n";
                    delete c;
                } else {
                    cout << "⚠️ Contrato con ID/ Contract with ID" << id << " no encontrado en la tabla hash/ not found in the hash table.\n";
                }
            }
        }
    }
// ---------------------------------------------------------------------

//-------- Mostrar contratos por tipo / Show contracts by type ---------
void showContractsByType(const MultiList& ml, const HashTable& hashTable) {
    string type;
    cout << "Tipo-Type (préstamo-loan / seguro-insurance / inversión-investment) : ";
    getline(cin, type);

    // Normalización para búsqueda bilingüe
    string normalizedInput = normalizeText(type);
    string searchTerm = normalizeForSearch(normalizedInput);

    // Obtener IDs de ambos términos (inglés/español)
    vector<string> ids = ml.getContractsByType(normalizedInput);
    vector<string> altIds = ml.getContractsByType(searchTerm);

    // Combinar resultados evitando duplicados
    vector<string> allIds;
    allIds.reserve(ids.size() + altIds.size());

    // Insertar primeros IDs
    allIds.insert(allIds.end(), ids.begin(), ids.end());

    // Insertar IDs alternativos si no están ya presentes
    for (const auto& id : altIds) {
        if (find(allIds.begin(), allIds.end(), id) == allIds.end()) {
            allIds.push_back(id);
        }
    }

    // Mostrar resultados
    if (allIds.empty()) {
        cout << "⚠️ No hay contratos de tipo/There are no contracts of type " << type << "\n";
    } else {
        cout << "📋 Contratos de tipo/Contracts of type '" << type << "' [";
        if (normalizedInput != searchTerm) {
            cout << normalizedInput << "/" << searchTerm;
        } else {
            cout << normalizedInput;
        }
        cout << "]:\n";

        for (const string& id : allIds) {
            Contract* c = hashTable.search(id);
            if (c) {
                displayContractDetails(*c);
                delete c;
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
    cout << "4. Buscar por tipo / Search by type.\n";
    cout << "5. Salir / Exit.\n";
    cout << "Opción / Choice: ";
}
// ---------------------------------------------------------------------

// ------------------ Función Principal/ Main Function------------------
int main() {
    
    AVLTree avl;           
    HashTable hashTable;
    MultiList multiList;  

    int option;
    
    do {
        displayMenu();
        cin >> option;
        cin.ignore(); 

        switch(option) {
            case 1: 
                registerContract(avl, hashTable, multiList); 
                break;
            case 2: 
                searchByDate(avl, hashTable); 
                break;
            case 3: {
                string id;
                cout << "\n🔎 Buscar por ID / Search by ID: ";
                getline(cin, id);

                // Eliminar TODOS los espacios (incluidos internos)
                id.erase(remove_if(id.begin(), id.end(), ::isspace), id.end());

                Contract* c = hashTable.search(id);
                if (c) {
                    displayContractDetails(*c);
                    delete c;
                } else {
                    cout << "⚠️ Contrato no encontrado/Contract not found (ID: " << id << ")\n";
                }
                break;
            }
            case 4:
                cout << "🔎 Buscar por tipo / Search by type\n";
                showContractsByType(multiList, hashTable); 
                break;
            case 5:
                cout << "\n👋 Hasta luego/See you later.\n";
                break;
            default:
                cout << "Opción inválida/Invalid option.\n";
        }
    } while (option != 5);  

    return 0;
}
