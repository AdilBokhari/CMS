#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Person
{
public:
    string name;
    string position;
    Person(string N, string P)
    {
        name = N;
        position = P;
    }
};

class Teacher :public Person
{
public:
    Teacher(string N) :Person(N, "Teacher") {};
};

class Manager :public Person
{
public:
    Manager(string N, string Dept) :Person(N, "Manager")
    {
        Department = Dept;
    };
    string Department;
};

class Employee :public Person
{
public:
    Employee(string N, string Dept) :Person(N, "Employee")
    {
        Department = Dept;
    };
    string Department;
};

class Complaint
{
public:
    string Date;
    string Status;
    string Description;
    Teacher* t;
    Manager* m;
    vector<Employee*>emps;
};

class CMS
{
public:
    CMS()
    {
        loadTeacherData();
        loadManagerData();
    }
    vector<Teacher>T;
    vector<Manager>M;
    vector<Employee>E;
    vector<Complaint>C;
    void loadTeacherData()
    {
        ifstream file("Teachers.txt");
        if (!file.is_open())
        {
            cerr << "Error opening Teachers.txt" << endl;
            return;
        }
        string line;
        while (getline(file, line))
        {
            T.push_back(Teacher(line));
        }
        file.close();
    }
    void loadManagerData()
    {
        ifstream file("Managers.txt");
        if (!file.is_open())
        {
            cerr << "Error opening Managers.txt" << endl;
            return;
        }
        string line;
        while (getline(file, line))
        {
            istringstream iss(line);
            string department, fullName;
            iss >> department >> ws;
            getline(iss, fullName);
            M.push_back(Manager(fullName, department));
        }
        file.close();
    }
};

void TeacherInterface(const CMS& cms)
{
    cout << "Enter name: ";
    string name;
    getchar();
    getline(cin, name);
    bool found = false;
    for (const Teacher& teacher : cms.T)
    {
        if (teacher.name == name)
        {
            found = true;
            break;
        }
    }
    if (found)
    {
        cout << "Complaint to Department" << endl;
    }
    else
    {
        cout << "Name does not exist in the System." << endl;
    }
}

void ManagerInterface(const CMS& cms)
{
    cout << "Enter name: ";
    string name;
    getchar();
    getline(cin, name);
    bool found = false;
    for (const Manager& manager : cms.M)
    {
        if (manager.name == name)
        {
            found = true;
            break;
        }
    }
    if (found)
    {
        cout << "Welcome" << endl;
    }
    else
    {
        cout << "Name does not exist in the System." << endl;
    }
}

int main()
{
    CMS cms;
    cout << "Complaint Management System.\n";
    cout << "Select option: \n";
    cout << "1. Teacher\n";
    cout << "2. Manager\n";
    cout << "3. Employee\n";
    cout << "4. Director\n";
    cout << "5. Administrator\n";
    int option;
    cin >> option;
    if (option == 1)
    {
        TeacherInterface(cms);
    }
    else if (option == 2)
    {
        ManagerInterface(cms);
    }
    else if (option == 3)
    {
        
    }
    else if (option == 4)
    {
        
    }
    else if (option == 5)
    {
        
    }
    return 0;
}