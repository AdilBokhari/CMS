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
        while (file >> id) {
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
        while (file >> dept >> id) {
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
        while (file >> dept >> id) {
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
        for (const Complaint& complaint : C)
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
        for (const Complaint& complaint : C)
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

        for (const Complaint& complaint : C)
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
                cout << "ID: " << employee.ID << endl;
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
            if (complaint.c_id == id)
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
                    for (const Complaint& c : C)
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
        for (const Complaint& complaint : C)
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
                exists = true;
                complaint.Status = "Resolved";
            }
        }

        if (!exists)
        {
            cout << "Invalid complaint ID.\n";
            return;
        }

        ofstream file("Complaints.txt");
        if (file.is_open())
        {
            for (const Complaint& c : C)
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
        int printedComplaints = 0; // Track the number of printed complaints
        for (const Complaint& complaint : C)
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
            if (complaint.c_id == id)
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
                    for (const Complaint& c : C)
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
        for (const Complaint& complaint : C)
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
            if (complaint.c_id == cid)
            {
                complaint.Status = "Assigned";
                exists = true;
                ofstream file("Complaints.txt");
                if (file.is_open())
                {
                    for (const Complaint& c : C)
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
            do
            {
                found = true;
                cout << "Welcome Manager of " << manager.Department << endl;
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
        cout << "6. Exit";
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
            EmployeeInterface(cms);
        }
        else if (option == 4)
        {

        }
        else if (option == 5)
        {

        }
        else if (option == 6)
        {
            break;
        }
    } while (true);
    return 0;
}