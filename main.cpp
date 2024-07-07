#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>
using namespace std;

class Student {
public:
    string name;
    int id_no;
    string stream;
    string book1, book2;
    int book_no, issuedbook;

    Student(string name, int id_no, string stream) {
        this->name = name;
        this->id_no = id_no;
        this->stream = stream;
        book1 = "";
        book2 = "";
        book_no = 0;
        issuedbook = 0;
    }

    Student() : id_no(0), book_no(0), issuedbook(0) {}
};

class Node {
public:
    string key;
    Node* left;
    Node* right;

    Node(string item) {
        key = item;
        left = nullptr;
        right = nullptr;
    }
};

class LibraryManagement {
private:
    Node* root;
    map<string, int> hashMapping;
    vector<Student> students;
    int arr[100][2];

public:
    LibraryManagement() {
        root = nullptr;
    }

    void insert(string key) {
        root = insertRec(root, key);
    }

    Node* insertRec(Node* root, string key) {
        if (root == nullptr) {
            root = new Node(key);
            return root;
        }

        if (key < root->key) 
            root->left = insertRec(root->left, key);
        else if (key > root->key) 
            root->right = insertRec(root->right, key);
        else
            cout << "error." << endl;

        return root;
    }

    void update(string key, string key1) {
        deleteKey(key);
        insert(key1);
    }

    bool containsNode(string value) {
        return containsNodeRecursive(root, value);
    }

    bool containsNodeRecursive(Node* current, string key) {
        if (current == nullptr) 
            return false;

        if (key == current->key) 
            return true;

        if (key < current->key)
            return containsNodeRecursive(current->left, key);
        else
            return containsNodeRecursive(current->right, key);
    }

    void printTree() {
        printTreeRec(root, 0);
    }

    void printTreeRec(Node* t, int space) {
        if (t == nullptr)
            return;

        space += 5;

        printTreeRec(t->right, space);

        cout << endl;

        for (int i = 5; i < space; i++)
            cout << " ";
        cout << "[" << t->key << "]";

        printTreeRec(t->left, space);
    }

    void deleteKey(string key) {
        root = deleteRec(root, key);
    }

    Node* deleteRec(Node* root, string key) {
        if (root == nullptr)  
            return root;

        if (key < root->key) 
            root->left = deleteRec(root->left, key);
        else if (key > root->key)
            root->right = deleteRec(root->right, key);
        else {
            if (root->left == nullptr) 
                return root->right;
            else if (root->right == nullptr) 
                return root->left;

            root->key = minValue(root->right);

            root->right = deleteRec(root->right, root->key);
        }

        return root;
    }

    string minValue(Node* root) {
        string minv = root->key;
        while (root->left != nullptr) {
            minv = root->left->key;
            root = root->left;
        }
        return minv;
    }

    void printInorder(Node* node) {
        if (node == nullptr)
            return;

        printInorder(node->left);

        cout << node->key << "\t\t";

        printInorder(node->right);
    }

    void printInorder() {
        printInorder(root);
    }

    void inorder() {
        inorderRec(root);
    }

    void inorderRec(Node* root) {
        if (root != nullptr) {
            inorderRec(root->left);
            cout << root->key << endl;
            inorderRec(root->right);
        }
    }

    void addStudent(string name, int id, string stream) {
        students.push_back(Student(name, id, stream));
        saveStudentsToFile();
    }

    void librarianMenu() {
        string pwd1 = "admin";
        string id1 = "admin";
        string id2, pwd2;
        
        cout << "\nEnter UserId: ";
        cin >> id2;

        cout << "\nEnter Password: ";
        cin >> pwd2;

        if (id1 != id2)
            cout << "Invalid Userid." << endl;
        else if (pwd1 != pwd2)
            cout << "Invalid Password." << endl;
        else {
            cout << "Login successfully." << endl;
            bool e2 = false;
            while (!e2) {
                cout << "\n....................................." << endl;
                cout << "1. Add book. " << endl;
                cout << "2. Delete book. " << endl;
                cout << "3. Update book. " << endl;
                cout << "4. Print Books Details. " << endl;
                cout << "5. Print Books in-order. " << endl;
                cout << "6. Print tree " << endl;
                cout << "7. Exit" << endl;

                cout << "\n....................................." << endl;

                cout << "\nEnter Your choice: ";
                int ch2;
                cin >> ch2;

                switch (ch2) {
                case 1:
                    addBook();
                    break;

                case 2:
                    deleteBook();
                    break;

                case 3:
                    updateBook();
                    break;

                case 4:
                    printBooksDetails();
                    break;

                case 5:
                    printInorder();
                    break;

                case 6:
                    printTree();
                    break;

                case 7:
                    e2 = true;
                    break;
                }
            }
        }
    }

    void addBook() {
        string name;
        int quantity;

        cout << "\nEnter name of book: ";
        cin >> name;

        if (containsNode(name)) {
            cout << "\nIt already exists." << endl;
        } else {
            cout << "\nEnter quantity of book: ";
            cin >> quantity;
            insert(name);
            hashMapping[name] = hashMapping.size();
            int idx = hashMapping[name];
            arr[idx][0] += quantity;
            arr[idx][1] += quantity;
        }
        saveBooksToFile();
    }

    void deleteBook() {
        string name;

        cout << "\nEnter name of book: ";
        cin >> name;

        if (containsNode(name)) {
            deleteKey(name);
            hashMapping.erase(name);
        }
        saveBooksToFile();
    }

    void updateBook() {
        string name;
        int quantity;

        cout << "\nEnter name of book: ";
        cin >> name;

        if (containsNode(name)) {
            int idx = hashMapping[name];
            cout << "\nEnter quantity of book to add more: ";
            cin >> quantity;
            arr[idx][0] += quantity;
        }
        saveBooksToFile();
    }

    void printBooksDetails() {
        for (auto& entry : hashMapping) {
            int idx = entry.second;
            cout << "Name of book is: " << entry.first << endl;
            cout << "Total Quantity of book is: " << arr[idx][0] << endl;
            cout << "Available Quantity of book is: " << arr[idx][1] << endl;
            cout << endl;
        }
    }

    void userMenu() {
        bool e3 = false;
        while (!e3) {
            cout << "\n....................................." << endl;
            cout << "1. Register. " << endl;
            cout << "2. Issue book. " << endl;
            cout << "3. Return book. " << endl;
            cout << "4. Exit" << endl;
            cout << "\n....................................." << endl;

            cout << "\nEnter Your choice: ";
            int ch3;
            cin >> ch3;

            switch (ch3) {
            case 1:
                registerUser();
                break;

            case 2:
                issueBook();
                break;

            case 3:
                returnBook();
                break;

            case 4:
                e3 = true;
                break;
            }
        }
    }

    void registerUser() {
        string name, stream;
        int id;

        cout << "\nEnter your name: ";
        cin >> name;
        cout << "\nEnter your id: ";
        cin >> id;
        cout << "\nEnter your stream: ";
        cin >> stream;

        addStudent(name, id, stream);
        cout << "Registration successful!" << endl;
    }

    void issueBook() {
        int id, idx = -1;
        string book;
        time_t now = time(0);
        tm* ltm = localtime(&now);

        cout << "\nEnter your id: ";
        cin >> id;

        for (int i = 0; i < students.size(); i++) {
            if (students[i].id_no == id)
                idx = i;
        }

        if (idx != -1) {
            if (students[idx].book_no == 2)
                cout << "\nYou already issued two books. So, you cannot issue more books." << endl;
            else {
                cout << "\nEnter name of book: ";
                cin >> book;

                if (containsNode(book)) {
                    int idx2 = hashMapping[book];
                    if (arr[idx2][1] > 0) {
                        if (students[idx].book1.empty())
                            students[idx].book1 = book;
                        else
                            students[idx].book2 = book;

                        arr[idx2][1]--;

                        cout << "Book issued on: ";
                        cout << ltm->tm_mday << "-"
                            << 1 + ltm->tm_mon << "-" << 1900 + ltm->tm_year << " ";
                        cout << 5 + ltm->tm_hour << ":";
                        cout << 30 + ltm->tm_min << ":";
                        cout << ltm->tm_sec << endl;
                        students[idx].book_no++;
                        students[idx].issuedbook++;
                    } else {
                        cout << "\nBook is not available." << endl;
                    }
                } else {
                    cout << "Book doesn't exist." << endl;
                }
            }
        } else {
            cout << "Student doesn't exist. First register yourself." << endl;
        }
    }

    void returnBook() {
        int id, idx = -1;
        string book;
        time_t now = time(0);
        tm* ltm = localtime(&now);

        cout << "\nEnter your id: ";
        cin >> id;

        for (int i = 0; i < students.size(); i++) {
            if (students[i].id_no == id)
                idx = i;
        }

        if (idx != -1) {
            cout << "\nEnter name of book: ";
            cin >> book;

            if (containsNode(book)) {
                if (students[idx].book1 == book) {
                    students[idx].book1.clear();
                    cout << "Book returned successfully." << endl;
                    arr[hashMapping[book]][1]++;
                    students[idx].book_no--;
                } else if (students[idx].book2 == book) {
                    students[idx].book2.clear();
                    cout << "Book returned successfully." << endl;
                    arr[hashMapping[book]][1]++;
                    students[idx].book_no--;
                } else {
                    cout << "You have not issued this book." << endl;
                }
            } else {
                cout << "Book doesn't exist." << endl;
            }
        } else {
            cout << "Student doesn't exist. First register yourself." << endl;
        }
    }

    void saveStudentsToFile() {
        ofstream file("students.txt");
        for (const auto& student : students) {
            file << student.name << " "
                << student.id_no << " "
                << student.stream << " "
                << student.book1 << " "
                << student.book2 << " "
                << student.book_no << " "
                << student.issuedbook << "\n";
        }
        file.close();
    }

    void loadStudentsFromFile() {
        ifstream file("students.txt");
        students.clear();
        string name, stream, book1, book2;
        int id, book_no, issuedbook;

        while (file >> name >> id >> stream >> book1 >> book2 >> book_no >> issuedbook) {
            Student student(name, id, stream);
            student.book1 = book1;
            student.book2 = book2;
            student.book_no = book_no;
            student.issuedbook = issuedbook;
            students.push_back(student);
        }
        file.close();
    }

    void saveBooksToFile() {
        ofstream file("books.txt");
        for (const auto& entry : hashMapping) {
            int idx = entry.second;
            file << entry.first << " "
                << arr[idx][0] << " "
                << arr[idx][1] << "\n";
        }
        file.close();
    }

    void loadBooksFromFile() {
        ifstream file("books.txt");
        hashMapping.clear();
        string name;
        int total_quantity, available_quantity;

        while (file >> name >> total_quantity >> available_quantity) {
            insert(name);
            int idx = hashMapping.size();
            hashMapping[name] = idx;
            arr[idx][0] = total_quantity;
            arr[idx][1] = available_quantity;
        }
        file.close();
    }
};

int main() {
    LibraryManagement libMgmt;
    libMgmt.loadStudentsFromFile();
    libMgmt.loadBooksFromFile();

    bool exit = false;
    while (!exit) {
        cout << "\n....................................." << endl;
        cout << "1. For librarian Menu. " << endl;
        cout << "2. For User Menu. " << endl;
        cout << "3. Exit. " << endl;
        cout << "\n....................................." << endl;

        cout << "\nEnter Your choice: ";
        int ch1;
        cin >> ch1;

        switch (ch1) {
        case 1:
            libMgmt.librarianMenu();
            break;

        case 2:
            libMgmt.userMenu();
            break;

        case 3:
            exit = true;
            break;
        }
    }

    return 0;
}
