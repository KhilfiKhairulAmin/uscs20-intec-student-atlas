#include <iostream> // For user input and output
#include <fstream> // For file operations
#include <cmath> // For complicated mathematical operations
#include <sstream> // For handling numerical to string conversion
using namespace std;


// DATA STRUCTURE DEFINITION

// Account datatype
struct Account
{
    int id = -1;
    string username;
    string password;
    string role;
    int refStudentId;
};

// Student datatype
struct Student
{
    int id = -1;
    string firstName;
    string lastName;
    unsigned short int age;
    string icNumber;
    string programme;
    unsigned short int numOfSubjects;
    float cgpa;
};


// GLOBAL VARIABLES DECLARATION

// Constant to control the maximum number of data an array can hold
const int MAX_SIZE = 200;

// Global array storing all `Account` data
Account Accounts[MAX_SIZE] = {Account()};

// Global array storing all `Student` data
Student Students[MAX_SIZE];

// Global variable storing the latest Error m6essage
string ErrMsg = "";


// PROGRAM FUNCTION PROTOTYPES

void loadAccounts();
void saveAccounts();
int createAccount(string, string, string);
bool updateAccount(int, string, string);
void printAccounts();
bool deleteAccountCascade(int);
void loadStudents();
void saveStudents();
int createStudent(int, string, string, int, string, string, int, float);
void printStudents();


int main()
{
    loadAccounts(); loadStudents();
    printAccounts();
    deleteAccountCascade(1);
    printAccounts();
    saveAccounts(); saveStudents();
    
    return 0;
}


// UTILITY FUNCTION PROTOTYPES

Account parseAccount(string);
Student parseStudent(string);
template<class T> int generateId(T [MAX_SIZE]);
template<class T> int getEmptyPosition(T [MAX_SIZE]);
template<class T> int findId(T [MAX_SIZE], int);
template<class T> bool deleteAtIndex(T [MAX_SIZE], int);
string accountToString(Account);
string studentToString(Student);
int stringToUint(string);
float stringToPositiveFloat(string);
template <class T>
string numToString(T);


// PROGRAM FUNCTIONS DEFINITION

/**
 * Retrieve all accounts from database.
*/
void loadAccounts()
{
    // Open the file
    ifstream readAccountsData("accounts.txt");
    string currentLine;

    int i = 0;
    // Read the file line by line
    while (getline(readAccountsData, currentLine))
    {
        Account account = parseAccount(currentLine);
        // Assign each account into the global array
        Accounts[i++] = account;
    }
    
    readAccountsData.close();

    return;
}

/**
 * Save all accounts into text file
*/
void saveAccounts()
{
    string save = "";
    int i = 0;

    // If i does not exceed max array size AND accountId is valid, continue loop
    while (i < MAX_SIZE && Accounts[i].id != -1)
    {
        // Convert to string form to be saved in .txt file
        save += accountToString(Accounts[i++]);
    }

    // Write string into text file
    ofstream writeAccounts("accounts.txt");
    writeAccounts << save;
    writeAccounts.close();

    return;
}

/**
 * Create a new `Account`
*/
int createAccount(string username, string password, string repeatPassword)
{
    // Get new id
    int accountId = generateId(Accounts);

    // Create new account
    Account newAccount = {
        accountId,
        username,
        password,
        "STUDENT"
    };

    // Find the position of empty space in the array
    int empty = getEmptyPosition(Accounts);

    // Array is fully used
    if (empty == -1)
    {
        ErrMsg = "Maximum size reached. Cannot add new Account.";
        // Return invalid id to represent error occurred
        return -1;
    }

    // Assign to empty location in `Accounts` array
    Accounts[empty] = newAccount;

    return accountId;
}

/**
 * Display all `Account` data
*/
void printAccounts()
{
    for (int i = 0; i <; i++)
        cout << accountToString(Accounts[i]);
}

/**
 * Update `Account` with `id` to the new data. Return `pos` if the account exists, else return `-1`.
*/
int updateAccount(int accountId, string username, string newPassword)
{
    // Find position of this `Account` in array
    int pos = findId(Accounts, accountId);

    // Account is not found
    if (pos == -1)
    {
        ErrMsg = "Account with id of " + numToString(accountId) + " does not exist.";
        return -1;
    }

    // Overwrite old username and password
    Accounts[pos].username = username;
    Accounts[pos].password = newPassword;

    // Return position pf this account in the array
    return pos;
}

/**
 * Delete an `Account` with `id` and the `Student` related to the account.
*/
bool deleteAccountCascade(int accountId)
{
    int accountIndex = findId(Accounts, accountId);

    if (accountIndex == -1)
    {
        ErrMsg = "Account with id of " + numToString(accountId) + " does not exist.";
        return false;
    }

    Account account = Accounts[accountIndex];

    if (account.role == "ADMIN")
    {
        ErrMsg = "Admin account can't be deleted.";
        return false;
    }
    
    int studentIndex = findId(Students, account.refStudentId);

    if (studentIndex == -1)
    {
        ErrMsg = "Student with id of " + numToString(accountId) + " does not exist.";
        return false;
    }
    
    if (!deleteAtIndex(Accounts, accountIndex) || !deleteAtIndex(Students, studentIndex))
    {
        ErrMsg = "Something went wrong.";
        return false;
    }

    return true;
}

/**
 * Loads all student data
*/
void loadStudents()
{
    ifstream readStudentsData("students.txt");
    string curLine;
    vector<Student> students;

    while (getline(readStudentsData, curLine))
    {
        vector<string> parsed = parseData(curLine);
        Student student;
        student.id = stringToUint(parsed[0]);
        student.firstName = parsed[1];
        student.lastName = parsed[2];
        student.age = stringToUint(parsed[3]);
        student.icNumber = parsed[4];
        student.programme = parsed[5];
        student.numOfSubjects = stringToUint(parsed[6]);
        student.cgpa = stringToPositiveFloat(parsed[7]);
    }

    readStudentsData.close();

    return;
}

/**
 * Save all students in this session
*/
void saveStudents()
{
    string save = "";
    for (int i = 0; i < Students.size(); i++)
    {
        save += studentToString(Students[i]);
    }

    ofstream writeStudents("students.txt");
    writeStudents << save;
    writeStudents.close();
}

int createStudent(int accountId, string firstName, string lastName, int age, string icNumber, string programme, int numOfSubjects, float cgpa)
{
    int studentId = Students.size();

    Student newStudent = {
        studentId,
        firstName,
        lastName,
        age,
        icNumber,
        programme,
        numOfSubjects,
        cgpa
    };

    // Link account with student data
    Accounts[accountId].refStudentId = studentId;

    Students.push_back(newStudent);

    return studentId;
}

void printStudents()
{
    for (int i = 0; i < Students.size(); i++)
        cout << studentToString(Students[i]);
}

bool updateStudent(int studentId, string firstName, string lastName, int age, string icNumber, string programme, int numOfSubjects, float cgpa)
{
    Student updateStudent = {
        studentId,
        firstName,
        lastName,
        age,
        icNumber,
        programme,
        numOfSubjects,
        cgpa
    };

    Students[searchIndexStudent(studentId)] = updateStudent;

    return true;
}

// ALL UTILITY FUNCTIONS

Account parseAccount(string unparsedText)
{
    string parsedString, parsedData[5];

    // Index used to parse string from startIndex to endIndex
    // Initial start is at index 0 and end is at the position of first delimiter (,)
    int startIndex = 0, endIndex = unparsedText.find_first_of(',');

    do
    {
        // Parse the data point
        parsedString = unparsedText.substr(startIndex, endIndex - startIndex);

        // Add the data point in this array
        parsedData->append(parsedString);

        // Update the start and end index
        startIndex = endIndex + 1;
        endIndex = unparsedText.find_first_of(',', startIndex);
    } // If endIndex == string.npos, then it means no more delimiter is found, thus ending the loop
    while (endIndex != unparsedText.npos);
    
    // Get the final data point
    parsedString = unparsedText.substr(startIndex);
    parsedData->append(parsedString);

    Account account;
    account.id = stringToUint(parsedData[0]);
    account.username = parsedData[1];
    account.password = parsedData[2];
    account.role = parsedData[3];
    if (account.role == "STUDENT")
        account.refStudentId = stringToUint(parsedData[4]);

    return account;
}

Student parseStudent(string rawString)
{
    string data[7], parsed;

    int startI = 0, endI;
    endI = rawString.find_first_of(',', startI);

    // When endI == npos is true, it means the comma character can't be found anymore
    while (endI != rawString.npos)
    {
        parsed = rawString.substr(startI, endI - startI); // Take the substring of the data
        data->append(parsed); // Store it
        startI = endI + 1;
        endI = rawString.find_first_of(',', startI);
    }

    // Parse the endmost data in the string
    parsed = rawString.substr(startI);
    data->append(parsed);

    Student student;
    student.id = stringToUint(data[0]);
    student.firstName = data[1];
    student.lastName = data[2];
    student.age = stringToUint(data[3]);
    student.icNumber = data[4];
    student.programme = data[5];
    student.numOfSubjects = stringToUint(data[6]);
    student.cgpa = stringToPositiveFloat(data[7]);

    return student;
}

/**
 * Generate a unique ID for new `Account` and new `Student`
*/
template<class T> 
int generateId(T array[MAX_SIZE])
{
    int maxId = array[0].id;
    // Search for highest index in the array (i.e. id which value is -1)
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (array[i].id == -1)
            break;
        else if (array[i].id > maxId)
            maxId = array[id].id
    }

    // Return the next highest id
    return ++maxId;
}

/**
 * Get the number of elements in the array
*/
template<class T>
int len(T array[MAX_SIZE])
{
    // Search for unused position in array (i.e. id which value is -1)
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (array[i].id == -1)
            return i + 1
    }
    // Array is fully used
    return MAX_SIZE
}

/**
 * Find index of target id. Return `-1` if not found.
*/
template<class T>
int findId(T array[MAX_SIZE], int targetId)
{
    // Search for index of target id
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (array[i].id == -1)
            break;
        if (array[i].id == targetId)
            return i;
    }
    return -1;
}

/**
 * Find an index in the array which is empty. Return `-1` if array is full.
*/
template<class T>
int getEmptyPosition(T array[MAX_SIZE])
{
    int length = len(array);
    if (length == MAX_SIZE)
        return -1;
    return length;
}

/**
 * Delete an element in array and move other elements to fill the empty space. Return `false` if an error occurred, else `True`.
*/
template<class T>
bool deleteAtIndex(T array[MAX_SIZE], int index)
{
    if (index >= MAX_SIZE)
    {
        ErrMsg = "Index at position " + numToString(index) + " is invalid.";
        return false;
    }
    delete array[index];

    for (int i = index; i < MAX_SIZE - 1; i++)
    {
        if (array[i].id == -1)
            break;
        array[i] = array[i+1];
    }
    return true;
}

/**
 * Parse an account into an account string
*/
string accountToString(Account account)
{
    return numToString(account.id) + ',' + account.username + ',' + account.password + ',' + account.role + ',' + numToString(account.refStudentId) + '\n';
}

/**
 * Convert student to string representation
*/
string studentToString(Student student)
{
    return numToString(student.id) + ',' + student.firstName + ',' + student.lastName + ',' + numToString(student.age)
           + ',' + student.icNumber + ',' + student.programme + ',' + numToString(student.numOfSubjects) + ','
           + numToString(student.cgpa) + '\n';
}

/**
 * Convert whole number string to positive integer
*/
int stringToUint(string s)
{
    int result;
    for(int i = 0; i < s.size(); i++)
    {
        int digit = s[i] - '0';
        result += digit * pow(10, s.size() - 1 - i);
    }
    return result;
}

/**
 * Convert numeric string to positive float
*/
float stringToPositiveFloat(string s)
{
    float result = 0.0;
    int lenDigitBeforeDecPoint = s.find_first_of('.');
    string beforeDec, afterDec;

    // Early return numerical string contains no decimal point, it represents integer!
    if (lenDigitBeforeDecPoint == s.npos)
        return stringToUint(s);
    else
    {
        beforeDec = s.substr(0, lenDigitBeforeDecPoint);
        afterDec = s.substr(lenDigitBeforeDecPoint + 1);
    }

    // Handles digit before decimal point
    result += stringToUint(beforeDec);

    // Handles digit after decimal point
    for (int i = 0; i < afterDec.size(); i++)
    {
        float digit = afterDec[i] - '0';
        result += digit / pow(10, i + 1);
    }

    return result;
}

template <class T>
string numToString(T n)
{
    stringstream out;
    out << n;
    return out.str();
}
