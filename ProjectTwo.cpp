#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// Define a structure to hold course information
struct Course {
    string courseNumber;      // Course number (e.g., CSCI100)
    string courseTitle;       // Course title (e.g., Introduction to Computer Science)
    vector<string> prerequisites; // List of prerequisite courses
};

// Define a structure for tree nodes in the binary search tree
struct TreeNode {
    Course course;           // Course data
    TreeNode* left;          // Pointer to the left child
    TreeNode* right;         // Pointer to the right child

    // Constructor to initialize a tree node
    TreeNode(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// Function to insert a course into the binary search tree
TreeNode* insert(TreeNode* root, Course course) {
    if (root == nullptr) {
        // Create a new node if the tree is empty
        return new TreeNode(course);
    }
    // Insert the course in the left subtree if its number is smaller
    if (course.courseNumber < root->course.courseNumber) {
        root->left = insert(root->left, course);
    }
    // Insert the course in the right subtree if its number is larger
    else if (course.courseNumber > root->course.courseNumber) {
        root->right = insert(root->right, course);
    }
    return root;
}

// Function to search for a course in the binary search tree
TreeNode* search(TreeNode* root, string courseNumber) {
    if (root == nullptr || root->course.courseNumber == courseNumber) {
        // Return the node if found or if tree is empty
        return root;
    }
    // Search the left subtree if the course number is smaller
    if (courseNumber < root->course.courseNumber) {
        return search(root->left, courseNumber);
    }
    // Search the right subtree if the course number is larger
    return search(root->right, courseNumber);
}

// Function to perform in-order traversal and print the courses
void inOrder(TreeNode* root) {
    if (root != nullptr) {
        inOrder(root->left);
        cout << root->course.courseNumber << ", " << root->course.courseTitle << endl;
        inOrder(root->right);
    }
}

// Function to load data from file into a binary search tree
TreeNode* loadDataFromFile(const string& filename) {
    TreeNode* root = nullptr;  // Root of the binary search tree
    ifstream file(filename);   // Input file stream

    // Check if the file was opened successfully
    if (!file.is_open()) {
        cerr << "Unable to open file " << filename << endl;
        return root;
    }

    string line;
    // Read each line from the file
    while (getline(file, line)) {
        stringstream ss(line);    // String stream to parse the line
        string courseNumber, courseTitle, prerequisite;
        getline(ss, courseNumber, ','); // Read course number
        getline(ss, courseTitle, ',');  // Read course title

        Course course;             // Create a Course object
        course.courseNumber = courseNumber;
        course.courseTitle = courseTitle;

        // Read all prerequisites
        while (getline(ss, prerequisite, ',')) {
            course.prerequisites.push_back(prerequisite);
        }

        // Insert the course into the binary search tree
        root = insert(root, course);

        // Debug statement to confirm data loading
        cout << "Loaded course: " << courseNumber << ", " << courseTitle << endl;
        if (!course.prerequisites.empty()) {
            cout << "Prerequisites: ";
            for (const string& prereq : course.prerequisites) {
                cout << prereq << " ";
            }
            cout << endl;
        }
    }
    file.close(); // Close the file
    return root;  // Return the root of the binary search tree
}

// Function to print the details of a specific course
void printCourseDetails(TreeNode* root, const string& courseNumber) {
    // Search for the course in the binary search tree
    TreeNode* courseNode = search(root, courseNumber);
    if (courseNode != nullptr) {
        // Print the course details if found
        const Course& course = courseNode->course;
        cout << course.courseNumber << ", " << course.courseTitle << endl;
        // Print the prerequisites if any
        if (!course.prerequisites.empty()) {
            cout << "Prerequisites: ";
            for (size_t i = 0; i < course.prerequisites.size(); ++i) {
                cout << course.prerequisites[i];
                if (i < course.prerequisites.size() - 1) cout << ", ";
            }
            cout << endl;
        }
        else {
            cout << "Prerequisites: None" << endl;
        }
    }
    else {
        // Course not found
        cout << "Course not found." << endl;
    }
}

// Main function to run the course planner program
int main() {
    TreeNode* root = nullptr;   // Root of the binary search tree
    int choice;                 // Variable to store user choice
    string courseNumber;        // Variable to store course number input by user
    const string filename = "ABCU_Advising_Program_Input.csv"; // Filename for course data

    while (true) {
        // Display the menu options
        cout << "Welcome to the course planner." << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice; // Get user choice

        switch (choice) {
        case 1:
            // Load data from file
            root = loadDataFromFile(filename);
            // Check if data was loaded successfully
            if (root != nullptr) {
                cout << "Data loaded successfully." << endl;
            }
            else {
                cout << "Failed to load data." << endl;
            }
            break;
        case 2:
            // Print the list of courses if data is loaded
            if (root != nullptr) {
                cout << "Here is a sample schedule:" << endl;
                inOrder(root);
            }
            else {
                cout << "No data loaded. Please load data first." << endl;
            }
            break;
        case 3:
            // Print details of a specific course if data is loaded
            if (root != nullptr) {
                cout << "What course do you want to know about? ";
                cin >> courseNumber;
                // Convert course number to uppercase
                transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);
                // Print course details
                printCourseDetails(root, courseNumber);
            }
            else {
                cout << "No data loaded. Please load data first." << endl;
            }
            break;
        case 9:
            // Exit the program
            cout << "Thank you for using the course planner!" << endl;
            return 0;
        default:
            // Invalid choice
            cout << choice << " is not a valid option." << endl;
        }
    }
}
