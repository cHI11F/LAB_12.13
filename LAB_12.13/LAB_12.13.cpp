#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <limits>

using namespace std;

struct Employee {
    string lastName;
    string phoneNumber;
    string roomNumber;

    Employee() : lastName(""), phoneNumber(""), roomNumber("") {}

    Employee(const string& ln, const string& pn, const string& rn)
        : lastName(ln), phoneNumber(pn), roomNumber(rn) {}
};

class TrieNode {
public:
    unordered_map<char, TrieNode*> children;
    Employee* employee;

    TrieNode() : employee(nullptr) {}

    ~TrieNode() {
        delete employee;
        for (auto& child : children) {
            delete child.second;
        }
    }
};

class Trie {
private:
    TrieNode* root;

public:
    Trie() : root(new TrieNode()) {}

    ~Trie() {
        clear(root);
    }

    void clear(TrieNode* node) {
        if (node == nullptr) return;
        for (auto& child : node->children) {
            clear(child.second);
        }
        delete node;
    }

    void addEmployee(const Employee& emp) {
        TrieNode* current = root;
        for (char c : emp.phoneNumber) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->employee = new Employee(emp);
    }

    void deleteEmployee(const string& phoneNumber) {
        if (deleteHelper(root, phoneNumber, 0)) {
            cout << "Employee with phone number " << phoneNumber << " deleted successfully." << endl;
        }
        else {
            cout << "Employee with phone number " << phoneNumber << " not found." << endl;
        }
    }

    bool deleteHelper(TrieNode* node, const string& phoneNumber, int depth) {
        if (node == nullptr) return false;

        if (depth == phoneNumber.size()) {
            if (node->employee != nullptr) {
                delete node->employee;
                node->employee = nullptr;
                return node->children.empty();
            }
            return false;
        }

        char c = phoneNumber[depth];
        if (deleteHelper(node->children[c], phoneNumber, depth + 1)) {
            delete node->children[c];
            node->children.erase(c);
            return node->employee == nullptr && node->children.empty();
        }

        return false;
    }

    Employee* searchByPhoneNumber(const string& phoneNumber) const {
        TrieNode* current = root;
        for (char c : phoneNumber) {
            if (current->children.find(c) == current->children.end()) {
                return nullptr;
            }
            current = current->children[c];
        }
        return current->employee;
    }

    void saveToFile(const string& filename) const {
        ofstream fout(filename);
        if (!fout.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }
        saveHelper(fout, root, "");
        fout.close();
    }

    void saveHelper(ofstream& fout, TrieNode* node, const string& phoneNumber) const {
        if (node == nullptr) return;
        if (node->employee != nullptr) {
            fout << node->employee->lastName << " " << phoneNumber << " " << node->employee->roomNumber << endl;
        }
        for (auto& child : node->children) {
            saveHelper(fout, child.second, phoneNumber + child.first);
        }
    }

    void readFromFile(const string& filename) {
        ifstream fin(filename);
        if (!fin.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }
        clear(root);
        root = new TrieNode();

        string lastName, phoneNumber, roomNumber;
        while (fin >> lastName >> phoneNumber >> roomNumber) {
            addEmployee(Employee(lastName, phoneNumber, roomNumber));
        }
        fin.close();
    }
};

string filename;

void addEmployee(Trie& phoneBook) {
    Employee newEmployee;
    cout << "Enter last name: ";
    cin >> newEmployee.lastName;
    cout << "Enter phone number (two digits): ";
    cin >> newEmployee.phoneNumber;
    cout << "Enter room number: ";
    cin >> newEmployee.roomNumber;

    phoneBook.addEmployee(newEmployee);
}

void deleteEmployee(Trie& phoneBook) {
    string phoneNumber;
    cout << "Enter phone number to delete: ";
    cin >> phoneNumber;

    phoneBook.deleteEmployee(phoneNumber);
}

void printEmployeeByPhoneNumber(const Trie& phoneBook) {
    string phoneNumber;
    cout << "Enter phone number: ";
    cin >> phoneNumber;

    Employee* employee = phoneBook.searchByPhoneNumber(phoneNumber);
    if (employee != nullptr) {
        cout << "Last name: " << employee->lastName << ", Room number: " << employee->roomNumber << endl;
    }
    else {
        cout << "Employee with phone number " << phoneNumber << " not found." << endl;
    }
}

void savePhoneBookToFile(const Trie& phoneBook) {
    phoneBook.saveToFile(filename);
    cout << "Phone book has been saved to file: " << filename << endl;
}

void readPhoneBookFromFile(Trie& phoneBook) {
    phoneBook.readFromFile(filename);
    cout << "Phone book has been read from file: " << filename << endl;
}

int main() {
    Trie phoneBook;

    cout << "Enter filename: ";
    getline(cin, filename);

    readPhoneBookFromFile(phoneBook);

    while (true) {
        cout << "Menu:" << endl;
        cout << "1. Add employee" << endl;
        cout << "2. Delete employee" << endl;
        cout << "3. Print employee by phone number" << endl;
        cout << "4. Save phone book to file" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch (choice) {
        case 1:
            addEmployee(phoneBook);
            break;
        case 2:
            deleteEmployee(phoneBook);
            break;
        case 3:
            printEmployeeByPhoneNumber(phoneBook);
            break;
        case 4:
            savePhoneBookToFile(phoneBook);
            break;
        case 5:
            cout << "Exiting program." << endl;
            savePhoneBookToFile(phoneBook);
            return 0;
        default:
            cout << "Invalid choice. Please enter a number from 1 to 5." << endl;
        }
    }

    return 0;
}
