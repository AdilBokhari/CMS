#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

string getCurrentDate() {
    time_t currentTime = std::time(nullptr);
    tm localTime;
    localtime_s(&localTime, &currentTime);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%d-%m-%Y", &localTime);
    return buffer;
}

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
    string Dept;
    Teacher* t;
    Manager* m;
    vector<Employee*>emps;
    Complaint() : t(nullptr), m(nullptr) {}
    ~Complaint()
    {
        delete t;
        delete m;
    }
};

class CMS
{
public:
    CMS()
    {
        loadTeacherData();
        loadManagerData();
        loadComplaintData();
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
    void loadComplaintData()
    {
        ifstream file("Complaints.txt");
        if (!file.is_open())
        {
            cerr << "Error opening Complaints.txt" << endl;
            return;
        }
        string line;
        while (getline(file, line))
        {
            Complaint complaint;
            istringstream iss(line);
            getline(iss, complaint.Date, '&');
            getline(iss, complaint.Status, '&');
            getline(iss, complaint.Description, '&');
            getline(iss, complaint.Dept, '&');
            complaint.t = new Teacher("Teacher");
            complaint.m = new Manager("Manager", complaint.Dept);
            getline(iss, complaint.t->name, '&');
            getline(iss, complaint.m->name, '&');
            C.push_back(complaint);
        }
        file.close();
    }
    void AddComplaint(Teacher* teacher)
    {
        cout << "Choose Department to complaint to: \n" << endl;
        cout << "1. IT\n";
        cout << "2. Accounts\n";
        cout << "3. Admin\n";

        int choice;
        cin >> choice;

        if (choice >= 1 && choice <= 3)
        {
            cout << "Enter Complaint: ";
            string description;
            cin.ignore();
            getline(cin, description);
            string date = getCurrentDate();
            Complaint complaint;
            complaint.Date = date;
            complaint.Status = "New";
            complaint.Description = description;
            complaint.t = teacher;
            switch (choice)
            {
            case 1:
                complaint.Dept = "IT";
                break;
            case 2:
                complaint.Dept = "Accounts";
                break;
            case 3:
                complaint.Dept = "Admin";
                break;
            }
            for (Manager& manager : M)
            {
                if (manager.Department == complaint.Dept)
                {
                    complaint.m = &manager;
                    break;
                }
            }
            C.push_back(complaint);
            WriteComplaintToFile(complaint);
            cout << "Complaint added successfully!" << endl;
        }
    }
    void WriteComplaintToFile(Complaint& complaint)
    {
        ofstream file("Complaints.txt", ios::app);
        if (file.is_open())
        {
            file << complaint.Date << "&";
            file << complaint.Status << "&";
            file << complaint.Description << "&";
            file << complaint.Dept << "&";
            file << complaint.t->name << "&";
            file << complaint.m->name << "&";
            file << endl;
            file.close();
        }
        else
        {
            cerr << "Error opening Complaints.txt for writing." << endl;
        }
    }
    void ViewComplaints(Teacher* teacher)
    {
        cout << "Complaints for Teacher: " << teacher->name << endl;
        for (Complaint& complaint : C)
        {
            if (complaint.t->name == teacher->name)
            {
                cout << "------------------------" << endl;
                cout << "Date: " << complaint.Date << endl;
                cout << "Status: " << complaint.Status << endl;
                cout << "Description: " << complaint.Description << endl;
                cout << "Department: " << complaint.Dept << endl;
                cout << "------------------------" << endl;
            }
        }
    }
};

void TeacherInterface(CMS& cms)
{
    cout << "Enter name: ";
    string name;
    cin.ignore();
    getline(cin, name);
    bool found = false;
    int opt;
    for (Teacher& teacher : cms.T)
    {
        if (teacher.name == name)
        {
            found = true;
            cout << "Choose Option: " << endl;
            cout << "1. Register Complaint.\n";
            cout << "2. View Complaints.\n";
            cout << "3. Close or Open Resolved Complaints.\n";
            cin >> opt;
            if (opt == 1)
            {
                cms.AddComplaint(&teacher);
            }
            if (opt == 2)
            {
                cms.ViewComplaints(&teacher);
            }
            break;
        }
    }
    if (!found)
    {
        cout << "Name does not exist in the System." << endl;
    }
}

void ManagerInterface(CMS& cms)
{
    cout << "Enter name: ";
    string name;
    cin.ignore();
    getline(cin, name);
    bool found = false;
    for (Manager& manager : cms.M)
    {
        if (manager.name == name)
        {
            found = true;
            cout << "Welcome Manager of " << manager.Department << endl;
            break;
        }
    }
    if (found)
    {

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