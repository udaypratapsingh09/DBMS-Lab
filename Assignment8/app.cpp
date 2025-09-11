#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

using namespace std;

void display_students(unique_ptr<sql::Connection>& con){
    // Create a statement
    unique_ptr<sql::Statement> stmt(con->createStatement());

    // Execute a query
    try{
        unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM Students"));
        cout << "Result: " << endl; // Access by index (1-based)
        while (res->next()) {
            cout << res->getString("RollNo") <<" "
                    << res->getString("Name") <<" "
                    << res->getString("Marks")<<endl;
        }
    }
    catch (sql::SQLException& e) {
        cout<< e.what() << endl;
    }
}

void insert_student(unique_ptr<sql::Connection>& con){
    // Create a statement
    int rollno, marks;
    string name;
    cout<<"Enter rollno: ";
    cin>>rollno;

    cout<<"Enter name: ";
    cin.ignore();
    getline(cin, name);

    cout<<"Enter marks: ";
    cin>>marks;

    try {
        unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(
        "INSERT INTO Students (RollNo, Name, Marks) VALUES (?, ?, ?)"));
        pstmt->setInt(1, rollno);
        pstmt->setString(2, name);
        pstmt->setInt(3, marks);

        int count = pstmt->executeUpdate();

        cout<<"Inserted "<<count<<" new row"<<endl;
    }
    catch (sql::SQLException& e) {
        cout<< e.what() << endl;
    }
}

void delete_student(unique_ptr<sql::Connection>& con){
    // Create a statement
    int choice;
    cout<<"MENU:\n1. Delete By RollNo\n2. Delete by Name (may delete multiple records)\n";
    cin>>choice;
    try {
        if (choice==1) {
            int rno;
            cout<<"Enter rollno: ";
            cin>>rno;
            unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(
            "DELETE FROM Students WHERE RollNo = (?)"));
            pstmt->setInt(1, rno);
            int count = pstmt->executeUpdate();
            cout<<"Deleted "<<count<<" records"<<endl;
        }
        else if (choice==2){
            string name;
            cout<<"Enter name: ";
            cin.ignore();
            getline(cin, name);
            unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(
            "DELETE FROM Students WHERE Name = (?)"));
            pstmt->setString(1, name);
            int count = pstmt->executeUpdate();
            cout<<"Deleted "<<count<<" records"<<endl;
        }
    }
    catch (sql::SQLException& e) {
        cout<< e.what() << endl;
    }
}

int main() {
    try {
        // Create a driver instance
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();

        // Create a connection
        unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", "connector", "password"));

        // Connect to the MySQL database
        con->setSchema("Connection");

        cout << "Connected to the database successfully!" << endl;
        int ch = 1;
        while (ch!=0){
            cout<<"MENU:\n0. Exit\n1. Display\n2. Insert\n3. Delete\n";
            cin>>ch;
            if (ch==1){
                display_students(con);
            }
            else if (ch==2){
                insert_student(con);
            }
            else if (ch==3){
                delete_student(con);
            }
        }
    }

    catch (sql::SQLException& e) {
        cerr << "Error connecting to MySQL: " << e.what() << endl;
        return 1;
    }

    return 0;
}
