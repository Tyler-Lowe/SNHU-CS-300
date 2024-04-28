#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct Course {
    string courseNumber;
    string name;
    vector<string> prerequisites;

    // Constructor
    Course(string number = "", string name = "") : courseNumber(number), name(name) {}
};

class BinarySearchTree {
private:
    struct Node {
        Course course;
        Node* left;
        Node* right;

        Node(Course course) : course(course), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // Helper function to insert a new node into the BST
    Node* insert(Node* node, Course course) {
        if (!node) return new Node(course);
        if (course.courseNumber < node->course.courseNumber)
            node->left = insert(node->left, course);
        else
            node->right = insert(node->right, course);
        return node;
    }

    // Helper function for in-order traversal of the BST
    void inOrder(Node* node) {
        if (!node) return;
        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.name << endl;
        inOrder(node->right);
    }

    // Helper function to find a node
    Node* find(Node* node, const string& courseNumber) {
        if (!node) return nullptr;
        if (courseNumber == node->course.courseNumber) return node;
        if (courseNumber < node->course.courseNumber)
            return find(node->left, courseNumber);
        else
            return find(node->right, courseNumber);
    }

public:
    BinarySearchTree() : root(nullptr) {}

    // Public function to insert courses
    void insert(Course course) {
        root = insert(root, course);
    }

    // Public function to print all courses
    void printCourses() {
        inOrder(root);
    }

    // Public function to print course details
    void printCourse(const string& courseNumber) {
        Node* result = find(root, courseNumber);
        if (result) {
            cout << "Course Number: " << result->course.courseNumber << endl;
            cout << "Course Title: " << result->course.name << endl;
            cout << "Prerequisites: ";
            if (!result->course.prerequisites.empty()) {
                for (const string& pre : result->course.prerequisites)
                    cout << pre << " ";
                cout << endl;
            }
            else {
                cout << "None" << endl;
            }
        }
        else {
            cout << "Course not found." << endl;
        }
    }

    // Destructor to clean up memory
    ~BinarySearchTree() {
        clear(root);
    }

    // Helper function to clear memory
    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
};

// This function now automatically loads courses from the fixed file path
void loadCoursesAutomatically(BinarySearchTree& bst) {
    // The path to 'courses.txt' within the 'Resource Files' directory
    string fileName = "courses.txt";
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Could not open file: " << fileName << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string part;
        vector<string> parts;
        while (getline(ss, part, ',')) {
            parts.push_back(part);
        }
        if (parts.size() < 2) {
            cout << "Invalid line format: " << line << endl;
            continue;
        }
        Course course(parts[0], parts[1]);
        for (size_t i = 2; i < parts.size(); ++i) {
            course.prerequisites.push_back(parts[i]);
        }
        bst.insert(course);
    }
    file.close();
    cout << "Data loaded successfully.\n";
}

int main() {
    BinarySearchTree bst;

    // Automatically load data on program start
    loadCoursesAutomatically(bst);

    int choice;
    do {
        cout << "1. Print Course List\n";
        cout << "2. Print Course\n";
        cout << "9. Exit\n";
        cout << "Select an option: ";
        cin >> choice;

        switch (choice) {
        case 1:
            bst.printCourses();
            break;
        case 2: {
            // Added braces to define scope for the variable courseNumber
            string courseNumber;
            cout << "Enter course number: ";
            cin >> courseNumber; // Ensure courseNumber is declared within the scope of this case
            bst.printCourse(courseNumber);
            break;
        }
        case 9:
            cout << "Exiting program." << endl;
            break;
        default:
            cout << "Invalid option. Please try again.\n";
            break; // Added a break statement for the default case
        }
    } while (choice != 9);

    return 0;
}

