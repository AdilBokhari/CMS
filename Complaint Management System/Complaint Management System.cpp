#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

string getCurrentDate()
{
    time_t currentTime = time(nullptr);
    tm localTime;
    localtime_s(&localTime, &currentTime);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y", &localTime);
    return buffer;
}

void getDate(string Date, int& d, int& m, int& y)
{
    size_t firstDash = Date.find('-');
    size_t secondDash = Date.find('-', firstDash + 1);
    d = stoi(Date.substr(0, firstDash));
    m = stoi(Date.substr(firstDash + 1, secondDash - firstDash - 1));
    y = stoi(Date.substr(secondDash + 1));
}

class Person
{
public:
    int ID;
    string position;
    Person(int id, string P)
    {
        ID = id;
        position = P;
    }
};

class Teacher :public Person
{
public:
    Teacher(int id) :Person(id, "Teacher") {};
};

class Manager :public Person
{
public:
    Manager(int id, string Dept) :Person(id, "Manager")
    {
        Department = Dept;
    };
    string Department;
};

class Employee :public Person
{
public:
    Employee(int id, string Dept) :Person(id, "Employee")
    {
        Department = Dept;
    };
    string Department;
};

class Complaint
{
public:
    string ComplaintDate;
    string AssignedDate;
    string Status;
    string Description;
    string Dept;
    int t_id;
    int m_id;
    int c_id;
    vector<int>emp_id;
};

class CMS
{
public:
    CMS()
    {
        countComplaints = 1;
        loadTeacherData();
        loadManagerData();
        loadComplaintData();
        loadEmployeeData();
    }
    vector<Teacher>T;
    vector<Manager>M;
    vector<Employee>E;
    vector<Complaint>C;
    int countComplaints;
    void loadTeacherData()
    {
        ifstream file("Teachers.txt");
        if (!file.is_open())
        {
            cerr << "Error opening Teachers.txt" << endl;
            return;
        }
        int id;
        while (file >> id)
        {
            T.push_back(Teacher(id));
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
        string dept;
        int id;
        while (file >> dept >> id)
        {
            M.push_back(Manager(id, dept));
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
            stringstream ss(line);
            Complaint complaint;
            getline(ss, complaint.ComplaintDate, '&');
            getline(ss, complaint.AssignedDate, '&');
            getline(ss, complaint.Status, '&');
            getline(ss, complaint.Description, '&');
            getline(ss, complaint.Dept, '&');
            ss >> complaint.t_id >> complaint.m_id >> complaint.c_id;
            int empId;
            while (ss >> empId)
            {
                complaint.emp_id.push_back(empId);
            }
            countComplaints++;
            C.push_back(complaint);
        }
        file.close();
    }
    void loadEmployeeData()
    {
        ifstream file("Employee.txt");
        if (!file.is_open())
        {
            cerr << "Error opening Employee.txt" << endl;
            return;
        }
        string dept;
        int id;
        while (file >> dept >> id)
        {
            E.push_back(Employee(id, dept));
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
            complaint.ComplaintDate = date;
            complaint.AssignedDate = "Not Assigned Yet";
            complaint.Status = "New";
            complaint.Description = description;
            complaint.t_id = teacher->ID;
            switch (choice)
            {
            case 1:
                complaint.Dept = "IT";
                break;
            case 2:
                complaint.Dept = "Account";
                break;
            case 3:
                complaint.Dept = "Admin";
                break;
            }
            for (Manager& manager : M)
            {
                if (manager.Department == complaint.Dept)
                {
                    complaint.m_id = manager.ID;
                    break;
                }
            }
            complaint.c_id = countComplaints;
            countComplaints++;
            C.push_back(complaint);
            WriteComplaintToFile(complaint);
            cout << "Complaint added successfully!" << endl;
        }
        else
        {
            cout << "Invalid Input. \n";
            return;
        }
    }
    void WriteComplaintToFile(Complaint& complaint)
    {
        ofstream file("Complaints.txt", ios::app);
        if (file.is_open())
        {
            file << complaint.ComplaintDate << "&";
            file << complaint.AssignedDate << "&";
            file << complaint.Status << "&";
            file << complaint.Description << "&";
            file << complaint.Dept << "&";
            file << complaint.t_id << " ";
            file << complaint.m_id << " ";
            file << complaint.c_id << " ";
            file << endl;
            file.close();
        }
        else
        {
            cerr << "Error opening Complaints.txt for writing." << endl;
        }
    }
    int viewAllComplaints()
    {
        cout << "Complaints:\n";
        int printedComplaints = 0;
        for (Complaint& complaint : C)
        {
            printedComplaints++;
            cout << "Date: " << complaint.ComplaintDate << endl;
            cout << "Status: " << complaint.Status << endl;
            cout << "Description: " << complaint.Description << endl;
            cout << "Department: " << complaint.Dept << endl;
            cout << "Teacher ID: " << complaint.t_id << endl;
            cout << "Manager ID: " << complaint.m_id << endl;
            cout << "Assigned Date: " << complaint.AssignedDate << endl;
            if (!complaint.emp_id.empty())
            {
                cout << "Employee IDs: ";
                for (int empId : complaint.emp_id)
                {
                    cout << empId << " ";
                }
                cout << endl;
            }
            cout << "\n---------------------------------------\n";
        }
        if (printedComplaints == 0)
        {
            cout << "No complaints found.\n";
            return -1;
        }
        return printedComplaints;
    }
    int viewAllTeacherComplaints(Teacher* teacher)
    {
        cout << "Complaints:\n";
        int printedComplaints = 0;
        for (Complaint& complaint : C)
        {
            if (complaint.t_id == teacher->ID)
            {
                printedComplaints++;
                cout << "Date: " << complaint.ComplaintDate << endl;
                cout << "Status: " << complaint.Status << endl;
                cout << "Description: " << complaint.Description << endl;
                cout << "Department: " << complaint.Dept << endl;
                cout << "Teacher ID: " << complaint.t_id << endl;
                cout << "Manager ID: " << complaint.m_id << endl;
                cout << "Assigned Date: " << complaint.AssignedDate << endl;
                if (!complaint.emp_id.empty())
                {
                    cout << "Employee IDs: ";
                    for (int empId : complaint.emp_id)
                    {
                        cout << empId << " ";
                    }
                    cout << endl;
                }
                cout << "\n---------------------------------------\n";
            }
        }
        if (printedComplaints == 0)
        {
            cout << "No complaints found for Teacher ID " << teacher->ID << ".\n";
            return -1;
        }
        return printedComplaints;
    }
    int viewAllDepartmentComplaints(string dept)
    {
        cout << "Complaints:\n";
        int printedComplaints = 0;

        for (Complaint& complaint : C)
        {
            if (complaint.Dept == dept)
            {
                printedComplaints++;

                cout << "ID: " << complaint.c_id << endl;
                cout << "Date: " << complaint.ComplaintDate << endl;
                cout << "Status: " << complaint.Status << endl;
                cout << "Description: " << complaint.Description << endl;
                cout << "Department: " << complaint.Dept << endl;
                cout << "Teacher ID: " << complaint.t_id << endl;
                cout << "Manager ID: " << complaint.m_id << endl;
                cout << "Assigned Date: " << complaint.AssignedDate << endl;
                if (!complaint.emp_id.empty())
                {
                    cout << "Employee IDs: ";
                    for (int empId : complaint.emp_id)
                    {
                        cout << empId << " ";
                    }
                    cout << endl;
                }
                cout << "\n---------------------------------------\n";
            }
        }
        if (printedComplaints == 0)
        {
            cout << "No complaints found for department " << dept << ".\n";
            return -1;
        }
        return printedComplaints;
    }
    void viewDeptEmps(string dept)
    {
        for (Employee& employee : E)
        {
            if (employee.Department==dept)
            {
                ifstream nameFile("Names.txt");
                if (!nameFile.is_open())
                {
                    cerr << "Error opening Names.txt" << endl;
                    return;
                }
                cout << "ID: " << employee.ID;
                while (!nameFile.eof())
                {
                    int id;
                    string name;
                    nameFile >> id >> name;

                    if (id == employee.ID)
                    {
                        cout << "   Name: " << name << endl;
                        break;
                    }
                }
                nameFile.close();
            }
        }
    }
    void managerAssignComplaint(string dept)
    {
        int id;
        cout << "Choose Complaint ID to assign: " << endl;
        if ((viewAllDepartmentComplaints(dept)) == -1)
        {
            return;
        }
        cout << "Enter: ";
        cin >> id;
        bool exists = false;
        for (Complaint& complaint : C)
        {
            if (complaint.c_id == id && complaint.Dept == dept)
            {
                exists = true;
                break;
            }
        }
        if (!exists)
        {
            cout << "Invalid complaint ID.\n";
            return;
        }
        cout << "Enter employee id to assign to: " << endl;
        viewDeptEmps(dept);
        int eid;
        cout << "Enter: ";
        cin >> eid;
        exists = false;
        for (Employee& employee : E)
        {
            if (employee.ID == eid)
            {
                exists = true;
                break;
            }
        }
        if (!exists)
        {
            cout << "Invalid emp ID.\n";
            return;
        }
        for (Complaint& complaint : C)
        {
            if (complaint.c_id == id)
            {
                bool alreadyAssigned = false;
                for (int assignedEmpId : complaint.emp_id)
                {
                    if (assignedEmpId == eid)
                    {
                        alreadyAssigned = true;
                        break;
                    }
                }
                if (alreadyAssigned)
                {
                    cout << "Employee already assigned to this complaint.\n";
                    return;
                }
                complaint.Status = "Assigned";
                complaint.AssignedDate = getCurrentDate();
                complaint.emp_id.push_back(eid);
                ofstream file("Complaints.txt");
                if (file.is_open())
                {
                    for (Complaint& c : C)
                    {
                        file << c.ComplaintDate << "&";
                        file << c.AssignedDate << "&";
                        file << c.Status << "&";
                        file << c.Description << "&";
                        file << c.Dept << "&";
                        file << c.t_id << " ";
                        file << c.m_id << " ";
                        file << c.c_id << " ";
                        for (int empId : c.emp_id)
                        {
                            file << empId << " ";
                        }
                        file << endl;
                    }
                    file.close();
                }
                else
                {
                    cerr << "Error opening Complaints.txt for writing." << endl;
                }
                break;
            }
        }
    }
    int viewAllemployeeComplaints(int id)
    {
        int printedComplaints = 0;
        for (Complaint& complaint : C)
        {
            bool employeeFound = false;
            for (int empId : complaint.emp_id)
            {
                if (empId == id)
                {
                    employeeFound = true;
                    break;
                }
            }
            if (employeeFound && (complaint.Status != "Resolved" && complaint.Status != "Closed"))
            {
                printedComplaints++;
                cout << "Complaint ID: " << complaint.c_id << endl;
                cout << "Date: " << complaint.ComplaintDate << endl;
                cout << "Status: " << complaint.Status << endl;
                cout << "Description: " << complaint.Description << endl;
                cout << "Department: " << complaint.Dept << endl;
                cout << "Teacher ID: " << complaint.t_id << endl;
                cout << "Manager ID: " << complaint.m_id << endl;
                cout << "Assigned Date: " << complaint.AssignedDate << endl;
                if (!complaint.emp_id.empty())
                {
                    cout << "Employee IDs: ";
                    for (int empId : complaint.emp_id)
                    {
                        cout << empId << " ";
                    }
                    cout << endl;
                }
                cout << "\n---------------------------------------\n";
            }
        }
        if (printedComplaints == 0)
        {
            cout << "No complaints found.\n";
            return -1;
        }
        return printedComplaints;
    }
    void markAsResolved(int id)
    {
        cout << "Choose Complaint ID to mark as resolved: " << endl;
        if ((viewAllemployeeComplaints(id)) == -1)
        {
            return;
        }
        int cid;
        cout << "Enter: ";
        cin >> cid;
        bool exists = false;
        for (Complaint& complaint : C)
        {
            if (complaint.c_id == cid)
            {
                bool employeeFound = false;
                for (int empId : complaint.emp_id)
                {
                    if (empId == id)
                    {
                        employeeFound = true;
                        break;
                    }
                }
                if (employeeFound)
                {
                    exists = true;
                    complaint.Status = "Resolved";
                }
            }
        }
        if (!exists)
        {
            cout << "Invalid complaint ID or employee ID.\n";
            return;
        }
        ofstream file("Complaints.txt");
        if (file.is_open())
        {
            for (Complaint& c : C)
            {
                file << c.ComplaintDate << "&";
                file << c.AssignedDate << "&";
                file << c.Status << "&";
                file << c.Description << "&";
                file << c.Dept << "&";
                file << c.t_id << " ";
                file << c.m_id << " ";
                file << c.c_id << " ";
                for (int empId : c.emp_id)
                {
                    file << empId << " ";
                }
                file << endl;
            }
            file.close();
        }
        else
        {
            cerr << "Error opening Complaints.txt for writing." << endl;
        }
    }
    int printResolvedComplaints(string dept)
    {
        cout << "Complaints:\n";
        int printedComplaints = 0;
        for (Complaint& complaint : C)
        {
            if (complaint.Dept == dept && complaint.Status == "Resolved")
            {
                printedComplaints++;
                cout << "ID: " << complaint.c_id << endl;
                cout << "Date: " << complaint.ComplaintDate << endl;
                cout << "Status: " << complaint.Status << endl;
                cout << "Description: " << complaint.Description << endl;
                cout << "Department: " << complaint.Dept << endl;
                cout << "Teacher ID: " << complaint.t_id << endl;
                cout << "Manager ID: " << complaint.m_id << endl;
                cout << "Assigned Date: " << complaint.AssignedDate << endl;
                if (!complaint.emp_id.empty())
                {
                    cout << "Employee IDs: ";
                    for (int empId : complaint.emp_id)
                    {
                        cout << empId << " ";
                    }
                    cout << endl;
                }
                cout << "\n---------------------------------------\n";
            }
        }
        if (printedComplaints == 0)
        {
            cout << "No resolved complaints found.\n";
            return -1;
        }
        return printedComplaints;
    }
    void CloseComplaint(string dept)
    {
        cout << "Choose ID to close or reassign: \n";
        if ((printResolvedComplaints(dept)) == -1)
        {
            return;
        }
        int id;
        cout << "Enter: ";
        cin >> id;
        bool exists = false;
        for (Complaint& complaint : C)
        {
            if (complaint.c_id == id && complaint.Dept == dept)
            {
                int opt;
                cout << "Press 1 to close, 2 to reassign: ";
                cin >> opt;
                if (opt==1)
                {
                    complaint.Status = "Closed";
                }
                else if (opt==2)
                {
                    complaint.Status = "Assigned";
                }
                else
                {
                    complaint.Status = "Resolved";
                    cout << "Invalid Option.\n";
                }
                exists = true;
                ofstream file("Complaints.txt");
                if (file.is_open())
                {
                    for (Complaint& c : C)
                    {
                        file << c.ComplaintDate << "&";
                        file << c.AssignedDate << "&";
                        file << c.Status << "&";
                        file << c.Description << "&";
                        file << c.Dept << "&";
                        file << c.t_id << " ";
                        file << c.m_id << " ";
                        file << c.c_id << " ";
                        for (int empId : c.emp_id)
                        {
                            file << empId << " ";
                        }
                        file << endl;
                    }
                    file.close();
                }
                else
                {
                    cerr << "Error opening Complaints.txt for writing." << endl;
                }
                break;
            }
        }
        if (!exists)
        {
            cout << "Invalid complaint ID.\n";
            return;
        }
    }
    int printClosedComplaintsOfTeacher(int id)
    {
        cout << "Closed Complaints:\n";
        int printedComplaints = 0;
        for (Complaint& complaint : C)
        {
            if (complaint.t_id == id && complaint.Status == "Closed")
            {
                printedComplaints++;
                cout << "ID: " << complaint.c_id << endl;
                cout << "Date: " << complaint.ComplaintDate << endl;
                cout << "Status: " << complaint.Status << endl;
                cout << "Description: " << complaint.Description << endl;
                cout << "Department: " << complaint.Dept << endl;
                cout << "Teacher ID: " << complaint.t_id << endl;
                cout << "Manager ID: " << complaint.m_id << endl;
                cout << "Assigned Date: " << complaint.AssignedDate << endl;
                if (!complaint.emp_id.empty())
                {
                    cout << "Employee IDs: ";
                    for (int empId : complaint.emp_id)
                    {
                        cout << empId << " ";
                    }
                    cout << endl;
                }
                cout << "\n---------------------------------------\n";
            }
        }
        if (printedComplaints == 0)
        {
            cout << "No closed complaints found.\n";
            return -1;
        }
        return printedComplaints;
    }
    void reOpenComplaint(Teacher* teacher)
    {
        cout << "Choose complaint ID to reopen: ";
        if ((printClosedComplaintsOfTeacher(teacher->ID)) == -1)
        {
            return;
        }
        cout << "Enter: ";
        int cid;
        cin >> cid;
        bool exists = false;
        for (Complaint& complaint : C)
        {
            if (complaint.c_id == cid && complaint.t_id == teacher->ID)
            {
                complaint.Status = "Assigned";
                exists = true;
                ofstream file("Complaints.txt");
                if (file.is_open())
                {
                    for (Complaint& c : C)
                    {
                        file << c.ComplaintDate << "&";
                        file << c.AssignedDate << "&";
                        file << c.Status << "&";
                        file << c.Description << "&";
                        file << c.Dept << "&";
                        file << c.t_id << " ";
                        file << c.m_id << " ";
                        file << c.c_id << " ";
                        for (int empId : c.emp_id)
                        {
                            file << empId << " ";
                        }
                        file << endl;
                    }
                    file.close();
                }
                else
                {
                    cerr << "Error opening Complaints.txt for writing." << endl;
                }
                break;
            }
        }
        if (!exists)
        {
            cout << "Invalid complaint ID.\n";
            return;
        }
    }
    int complaintInATimeLine(int startMonth, int startYear, int endMonth, int endYear)
    {
        cout << "Complaints:\n";
        int printedComplaints = 0;
        for (Complaint& complaint : C)
        {
            int d, m, y;
            getDate(complaint.ComplaintDate,d,m,y);
            if ((y <= endYear && y >= startYear) && (startMonth <= m && m <= endMonth))
            {
                printedComplaints++;
                cout << "Date: " << complaint.ComplaintDate << endl;
                cout << "Status: " << complaint.Status << endl;
                cout << "Description: " << complaint.Description << endl;
                cout << "Department: " << complaint.Dept << endl;
                cout << "Teacher ID: " << complaint.t_id << endl;
                cout << "Manager ID: " << complaint.m_id << endl;
                cout << "Assigned Date: " << complaint.AssignedDate << endl;
                if (!complaint.emp_id.empty())
                {
                    cout << "Employee IDs: ";
                    for (int empId : complaint.emp_id)
                    {
                        cout << empId << " ";
                    }
                    cout << endl;
                }
                cout << "\n---------------------------------------\n";
            }
        }
        if (printedComplaints == 0)
        {
            cout << "No complaints found.\n";
            return -1;
        }
        return printedComplaints;
    }
    void printTeachers()
    {
        for (Teacher& teacher : T)
        {
            ifstream nameFile("Names.txt");
            if (!nameFile.is_open())
            {
                cerr << "Error opening Names.txt" << endl;
                return;
            }
            cout << "ID: " << teacher.ID;
            while (!nameFile.eof())
            {
                int id;
                string name;
                nameFile >> id >> name;

                if (id == teacher.ID)
                {
                    cout << "   Name: " << name << endl;
                    break;
                }
            }
        }
    }
    void DeleteTeacher()
    {
        cout << "Choose ID to delete: " << endl;
        printTeachers();
        int tid;
        cout << "Enter: ";
        cin >> tid;
        bool exists = false;
        for (Teacher& teacher : T)
        {
            if (teacher.ID == tid)
            {
                exists = true;
                break;
            }
        }
        if (!exists)
        {
            cout << "Invalid Teacher ID.\n";
            return;
        }
        ifstream inputFile("Teachers.txt");
        if (!inputFile.is_open())
        {
            cerr << "Error opening Teachers.txt for reading." << endl;
            return;
        }
        vector<string> lines;
        string line;
        while (getline(inputFile, line))
        {
            lines.push_back(line);
        }
        inputFile.close();
        ofstream outputFile("Teachers.txt");
        if (!outputFile.is_open())
        {
            cerr << "Error opening Teachers.txt for writing." << endl;
            return;
        }
        for (auto& currentLine : lines)
        {
            istringstream iss(currentLine);
            int currentNumber;
            iss >> currentNumber;
            if (currentNumber != tid) {
                outputFile << currentLine << endl;
            }
        }
        outputFile.close();
        ifstream inputFile2("Names.txt");
        if (!inputFile2.is_open())
        {
            cerr << "Error opening Names.txt for reading." << endl;
            return;
        }
        vector<string> lines2;
        string line2;
        while (getline(inputFile2, line2))
        {
            lines2.push_back(line2);
        }
        inputFile2.close();
        ofstream outputFile2("Names.txt");
        if (!outputFile2.is_open())
        {
            cerr << "Error opening Names.txt for writing." << endl;
            return;
        }
        for (auto& currentLine2 : lines2)
        {
            istringstream iss(currentLine2);
            int currentNumber;
            iss >> currentNumber;
            if (currentNumber != tid) {
                outputFile2 << currentLine2 << endl;
            }
        }
        outputFile2.close();
        cout << "Teacher with ID " << tid << " deleted ." << endl;
        resetVectors();
    }
    bool checkIDExists(int tid)
    {
        ifstream file("Names.txt");
        if (!file.is_open())
        {
            cerr << "Error opening the file 'Names.txt'" << endl;
            return false;
        }
        string line;
        while (getline(file, line))
        {
            istringstream iss(line);
            int id;
            string name;
            if (iss >> id >> name)
            {
                if (id == tid)
                {
                    file.close();
                    return true;
                }
            }
            else 
            {
                cerr << "Error reading data from the file 'Names.txt'" << endl;
                file.close();
                return false;
            }
        }
        file.close();
        return false;
    }
    void AddTeacher()
    {
        cout << "Enter an ID: " << endl;
        int tid;
        cin >> tid;
        bool check = checkIDExists(tid);
        if (check)
        {
            cout << "This ID is already assigned." << endl;
            return;
        }
        cout << "Enter Name: ";
        string name;
        cin.ignore();
        getline(cin,name);
        ofstream file("Names.txt", ios::app);
        if (!file.is_open())
        {
            cerr << "Error opening the file 'Names.txt'" << endl;
            return;
        }
        file << tid << " " << name << endl;
        file.close();
        ofstream file2("Teachers.txt", ios::app);
        if (!file2.is_open())
        {
            cerr << "Error opening the file 'Teachers.txt'" << endl;
            return;
        }
        file2 << tid << endl;
        file2.close();
        resetVectors();
        cout << "Teacher Added." << endl;
    }
    void resetVectors()
    {
        T.clear();
        M.clear();
        E.clear();
        C.clear();
        countComplaints = 1;
        loadTeacherData();
        loadManagerData();
        loadComplaintData();
        loadEmployeeData();
    }
    void DeleteEmployee()
    {
        cout << "Choose ID to delete: " << endl;
        cout << "IT Dept: " << endl;
        viewDeptEmps("IT");
        cout << endl << "Account Dept: " << endl;
        viewDeptEmps("Account");
        cout << endl << "Admin Dept: " << endl;
        viewDeptEmps("Admin");
        int eid;
        cout << "Enter: ";
        cin >> eid;
        bool exists = false;
        for (Employee& employee : E)
        {
            if (employee.ID == eid)
            {
                exists = true;
                break;
            }
        }
        if (!exists)
        {
            cout << "Invalid Employee ID.\n";
            return;
        }
        ifstream inputFile("Employee.txt");
        if (!inputFile.is_open())
        {
            cerr << "Error opening Employee.txt for reading." << endl;
            return;
        }
        vector<string> lines;
        string line;
        while (getline(inputFile, line))
        {
            lines.push_back(line);
        }
        inputFile.close();
        ofstream outputFile("Employee.txt");
        if (!outputFile.is_open())
        {
            cerr << "Error opening Employee.txt for writing." << endl;
            return;
        }
        for (auto& currentLine : lines) 
        {
            istringstream iss(currentLine);
            string currentName;
            int currentNumber;
            iss >> currentName >> currentNumber;
            if (currentNumber != eid) {
                outputFile << currentName << " " << currentNumber << endl;
            }
        }
        outputFile.close();
        ifstream inputFile2("Names.txt");
        if (!inputFile2.is_open())
        {
            cerr << "Error opening Names.txt for reading." << endl;
            return;
        }
        vector<string> lines2;
        string line2;
        while (getline(inputFile2, line2))
        {
            lines2.push_back(line2);
        }
        inputFile2.close();
        ofstream outputFile2("Names.txt");
        if (!outputFile2.is_open())
        {
            cerr << "Error opening Names.txt for writing." << endl;
            return;
        }
        for (auto& currentLine2 : lines2)
        {
            istringstream iss(currentLine2);
            int currentNumber;
            iss >> currentNumber;
            if (currentNumber != eid) {
                outputFile2 << currentLine2 << endl;
            }
        }
        outputFile2.close();
        cout << "Employee with ID " << eid << " deleted ." << endl;
        resetVectors();
    }
    void AddEmployee()
    {
        cout << "Enter an ID: " << endl;
        int id;
        cin >> id;
        bool check = checkIDExists(id);
        if (check)
        {
            cout << "This ID is already assigned." << endl;
            return;
        }
        cout << "Enter Name: ";
        string name;
        cin.ignore();
        getline(cin, name);
        cout << "Choose Department: " << endl;
        cout << "1. IT\n";
        cout << "2. Accounts\n";
        cout << "3. Admin\n";
        int choice;
        cin >> choice;
        string Dept;
        if (choice >= 1 && choice <= 3)
        {
            switch (choice)
            {
            case 1:
                Dept = "IT";
                break;
            case 2:
                Dept = "Account";
                break;
            case 3:
                Dept = "Admin";
                break;
            }
        }
        else
        {
            cout << "Invalid Input." << endl;
            return;
        }
        ofstream file("Names.txt", ios::app);
        if (!file.is_open())
        {
            cerr << "Error opening the file Names.txt" << endl;
            return;
        }
        file << id << " " << name << endl;
        file.close();
        ofstream file2("Employee.txt", ios::app);
        if (!file2.is_open())
        {
            cerr << "Error opening the file Employee.txt" << endl;
            return;
        }
        file2 << Dept << " " << id << endl;
        resetVectors();
        cout << "Employee Added." << endl;
    }
    void deleteManager(string dept)
    {
        int delID;
        for (Manager& manager : M)
        {
            if (manager.Department == dept)
            {
                delID = manager.ID;
            }
        }
        ifstream inputFile("Managers.txt");
        if (!inputFile.is_open())
        {
            cerr << "Error opening Managers.txt for reading." << endl;
            return;
        }
        vector<string> lines;
        string line;
        while (getline(inputFile, line))
        {
            lines.push_back(line);
        }
        inputFile.close();
        ofstream outputFile("Managers.txt");
        if (!outputFile.is_open())
        {
            cerr << "Error opening Managers.txt for writing." << endl;
            return;
        }
        for (auto& currentLine : lines)
        {
            istringstream iss(currentLine);
            string currentName;
            int currentNumber;
            iss >> currentName >> currentNumber;
            if (currentNumber != delID)
            {
                outputFile << currentName << " " << currentNumber << endl;
            }
        }
        outputFile.close();
        ifstream inputFile2("Names.txt");
        if (!inputFile2.is_open())
        {
            cerr << "Error opening Names.txt for reading." << endl;
            return;
        }
        vector<string> lines2;
        string line2;
        while (getline(inputFile2, line2))
        {
            lines2.push_back(line2);
        }
        inputFile2.close();
        ofstream outputFile2("Names.txt");
        if (!outputFile2.is_open())
        {
            cerr << "Error opening Names.txt for writing." << endl;
            return;
        }
        for (auto& currentLine2 : lines2)
        {
            istringstream iss(currentLine2);
            int currentNumber;
            iss >> currentNumber;
            if (currentNumber != delID) 
            {
                outputFile2 << currentLine2 << endl;
            }
        }
        outputFile2.close();
    }
    void AddManager(int id, string Dept, string name)
    {
        ofstream file("Names.txt", ios::app);
        if (!file.is_open())
        {
            cerr << "Error opening the file Names.txt" << endl;
            return;
        }
        file << id << " " << name << endl;
        file.close();
        ofstream file2("Managers.txt", ios::app);
        if (!file2.is_open())
        {
            cerr << "Error opening the file Managers.txt" << endl;
            return;
        }
        file2 << Dept << " " << id << endl;
        resetVectors();
        cout << "Manager Updated." << endl;
    }
    void ReplaceManager()
    {
        cout << "Choose Department to assign a new Manager: " << endl;
        cout << "1. IT\n";
        cout << "2. Accounts\n";
        cout << "3. Admin\n";
        int choice;
        cin >> choice;
        string Dept;
        if (choice >= 1 && choice <= 3)
        {
            switch (choice)
            {
            case 1:
                Dept = "IT";
                break;
            case 2:
                Dept = "Account";
                break;
            case 3:
                Dept = "Admin";
                break;
            }
        }
        else
        {
            cout << "Invalid input.\n";
            return;
        }
        cout << "Enter an new ID: " << endl;
        int id;
        cin >> id;
        bool check = checkIDExists(id);
        if (check)
        {
            cout << "This ID is already assigned." << endl;
            return;
        }
        cout << "Enter Name: ";
        string name;
        cin.ignore();
        getline(cin, name);
        deleteManager(Dept);
        AddManager(id, Dept, name);
    }
};

void TeacherInterface(CMS& cms)
{
    cout << "Enter id: ";
    int id;
    cin >> id;
    bool found = false;
    int opt;
    for (Teacher& teacher : cms.T)
    {
        if (teacher.ID == id)
        {
            do
            {
                found = true;
                cout << "Choose Option: " << endl;
                cout << "1. Register Complaint.\n";
                cout << "2. View All Your Complaints.\n";
                cout << "3. Reopen Closed Complaints.\n";
                cout << "4. Exit.\n";
                cin >> opt;
                if (opt == 1)
                {
                    cms.AddComplaint(&teacher);
                }
                if (opt == 2)
                {
                    int x = cms.viewAllTeacherComplaints(&teacher);
                }
                if (opt == 3)
                {
                    cms.reOpenComplaint(&teacher);
                }
                if (opt == 4)
                {
                    break;
                }
            } while (true);
            break;
        }
    }
    if (!found)
    {
        cout << "ID does not exist in the System." << endl;
    }
}

void ManagerInterface(CMS& cms)
{
    cout << "Enter id: ";
    int id;
    cin >> id;
    bool found = false;
    int opt;
    for (Manager& manager : cms.M)
    {
        if (manager.ID == id)
        {
            cout << "Welcome Manager of " << manager.Department << endl;
            do
            {
                found = true;
                cout << "Choose Option: " << endl;
                cout << "1. View All Complaints of Your Department.\n";
                cout << "2. Assign new complaints to employees.\n";
                cout << "3. Close Resolved Complaints.\n";
                cout << "4. Exit.\n";
                cin >> opt;
                if (opt == 1)
                {
                    int x = cms.viewAllDepartmentComplaints(manager.Department);
                }
                if (opt == 2)
                {
                    cms.managerAssignComplaint(manager.Department);
                }
                if (opt == 3)
                {
                    cms.CloseComplaint(manager.Department);
                }
                if (opt == 4)
                {
                    break;
                }
            } while (true);
            break;
        }
    }
    if (!found)
    {
        cout << "ID does not exist in the System." << endl;
    }
}

void EmployeeInterface(CMS& cms)
{
    cout << "Enter id: ";
    int id;
    cin >> id;
    bool found = false;
    int opt;
    for (Employee& employee : cms.E)
    {
        if (employee.ID == id)
        {
            do
            {
                found = true;
                cout << "Choose Option: " << endl;
                cout << "1. View Complaints To Work On.\n";
                cout << "2. Mark a complaint as resolved.\n";
                cout << "3. Exit.\n";
                cin >> opt;
                if (opt == 1)
                {
                    int x = cms.viewAllemployeeComplaints(id);
                }
                if (opt == 2)
                {
                    cms.markAsResolved(id);
                }
                if (opt == 3)
                {
                    break;
                }
            } while (true);
            break;
        }
    }
    if (!found)
    {
        cout << "ID does not exist in the System." << endl;
    }
}

void DirectorInterface(CMS& cms)
{
    cout << "Enter Username: ";
    string usern;
    cin.ignore();
    getline(cin, usern);
    cout << "Enter Password: ";
    string pass;
    getline(cin, pass);
    ifstream directorFile("Director.txt");
    if (!directorFile.is_open())
    {
        cerr << "Error opening Director.txt" << endl;
        return;
    }
    string user;
    string password;
    getline(directorFile, user);
    getline(directorFile, password);
    directorFile.close();
    if (usern == user && pass == password)
    {
        cout << "Login successful. Welcome, Director!" << endl;
        do
        {
            int opt;
            cout << "Choose Option: " << endl;
            cout << "1. View All Complaints.\n";
            cout << "2. View Complaints of any department.\n";
            cout << "3. View Complaints in a specific timeline.\n";
            cout << "4. Exit.\n";
            cin >> opt;
            if (opt == 1)
            {
                int x = cms.viewAllComplaints();
            }
            if (opt == 2)
            {
                cout << "Choose Department to view complaints of: " << endl;
                cout << "1. IT\n";
                cout << "2. Accounts\n";
                cout << "3. Admin\n";
                int choice;
                cin >> choice;
                string Dept;
                if (choice >= 1 && choice <= 3)
                {
                    switch (choice)
                    {
                    case 1:
                        Dept = "IT";
                        break;
                    case 2:
                        Dept = "Account";
                        break;
                    case 3:
                        Dept = "Admin";
                        break;
                    }
                    int x = cms.viewAllDepartmentComplaints(Dept);
                }
                else
                {
                    cout << "Invalid Input.\n";
                    return;
                }
            }
            if (opt == 3)
            {
                int startMonth, startYear, endMonth, endYear;
                cout << "Enter start Month: ";
                cin >> startMonth;
                cout << "Enter start Year: ";
                cin >> startYear;
                cout << "Enter end Month: ";
                cin >> endMonth;
                cout << "Enter end Year: ";
                cin >> endYear;
                int x = cms.complaintInATimeLine(startMonth, startYear, endMonth, endYear);
            }
            if (opt == 4)
            {
                break;
            }

        } while (true);
    }
    else
    {
        cout << "Incorrect username or password." << endl;
    }
}

void AdminInterface(CMS& cms)
{
    cout << "Enter Username: ";
    string usern;
    cin.ignore();
    getline(cin, usern);
    cout << "Enter Password: ";
    string pass;
    getline(cin, pass);
    ifstream adminFile("Admin.txt");
    if (!adminFile.is_open())
    {
        cerr << "Error opening Admin.txt" << endl;
        return;
    }
    string user;
    string password;
    getline(adminFile, user);
    getline(adminFile, password);
    adminFile.close();
    if (usern == user && pass == password)
    {
        cout << "Login successful. Welcome, Admin!" << endl;
        do
        {
            int opt;
            cout << "Choose Option: " << endl;
            cout << "1. Delete a Teacher.\n";
            cout << "2. Add a Teacher.\n";
            cout << "3. Delete a Employee.\n";
            cout << "4. Add a Employee.\n";
            cout << "5. Replace a manager.\n";
            cout << "6. Exit.\n";
            cin >> opt;
            if (opt == 1)
            {
                cms.DeleteTeacher();
            }
            if (opt == 2)
            {
                cms.AddTeacher();
            }
            if (opt == 3)
            {
                cms.DeleteEmployee();
            }
            if (opt == 4)
            {
                cms.AddEmployee();
            }
            if (opt == 5)
            {
                cms.ReplaceManager();
            }
            if (opt == 6)
            {
                break;
            }

        } while (true);
    }
    else
    {
        cout << "Incorrect username or password." << endl;
    }
}

int main()
{
    CMS cms;
    do {
        system("cls");
        cout << "Complaint Management System.\n";
        cout << "Select option: \n";
        cout << "1. Teacher\n";
        cout << "2. Manager\n";
        cout << "3. Employee\n";
        cout << "4. Director\n";
        cout << "5. Administrator\n";
        cout << "6. Exit\n";
        cout << "Enter: ";
        int option;
        cin >> option;
        if (option == 1)
        {
            system("cls");
            TeacherInterface(cms);
        }
        else if (option == 2)
        {
            system("cls");
            ManagerInterface(cms);
        }
        else if (option == 3)
        {
            system("cls");
            EmployeeInterface(cms);
        }
        else if (option == 4)
        {
            system("cls");
            DirectorInterface(cms);
        }
        else if (option == 5)
        {
            system("cls");
            AdminInterface(cms);
        }
        else if (option == 6)
        {
            break;
        }
    } while (true);
    return 0;
}