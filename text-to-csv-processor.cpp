#include <iostream>
#include <exception>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <ctime>

using namespace std;

struct Hash {
    string name;
    string month;
    string year;
};

struct DayDTO {
    int day;
    int month;
    int year;
};

struct MonthDTO {
    int month;
    int year;
};

Hash extractHash(string hash) {
    char delim = ';';
    vector<string> exploaded;
    string tmp = "";    
    Hash hashObj;

    for (int i = 0; i < hash.length(); i++) {
        if (hash[i] == delim) {
            exploaded.push_back(tmp);
            tmp = "";
            i++;
        }
        tmp += hash[i];
    }

    hashObj.name = exploaded[0];
    hashObj.month = exploaded[1];
    hashObj.year = exploaded[2];

    return hashObj;
}

string createHash(string name, MonthDTO dto) {
    return name + "-" + std::to_string(dto.month) + "-" + std::to_string(dto.year);
}

struct OutputRow {
    public:
        string name;
        string month;
        int totalHours;
};

struct ValidateHeaders: public exception {
    const char * talk () const throw () {
        return "Expected headers must be Name;email;department;position;project;task;date;logged hours";
    }
};

// artifact
struct TM {
    int tm_sec;   // seconds of minutes from 0 to 61
    int tm_min;   // minutes of hour from 0 to 59
    int tm_hour;  // hours of day from 0 to 24
    int tm_mday;  // day of month from 1 to 31
    int tm_mon;   // month of year from 0 to 11
    int tm_year;  // year since 1900
    int tm_wday;  // days since sunday
    int tm_yday;  // days since January 1st
    int tm_isdst; // hours of daylight savings time
};

struct Row {
    public:
        int loggedHours;
        DayDTO day;
        MonthDTO month;
        string name;
        string email;
        string department;
        string position;
        string project;
        string task;
        string date;
};

DayDTO createDay(string date) {
    // 2021-10-19 - date
    // if date not that format throw error;

    DayDTO dto; // = new DayDTO();
    dto.day = 0;
    dto.month = 0;
    dto.year = 0;

    vector<string> exploaded;
    string tmp = "";
    char delim = '-';

    for (int i = 0; i < date.length(); i++) {
        if (date[i] == delim) {
            exploaded.push_back(tmp);
            tmp = "";
            i++;
        }
        tmp += date[i];
    }

    dto.year = std::stoi(exploaded[0]);
    dto.month = std::stoi(exploaded[1]);
    dto.day = std::stoi(tmp); 

    return dto;
}

MonthDTO createMonth(DayDTO dateDayDTO) {
    MonthDTO dto; // = new MonthDTO();
    dto.month = 0;
    dto.year = 0;
    dto.month = dateDayDTO.month;
    dto.year = dateDayDTO.year;

    return dto;
}

Row createRow(string row, char delim = ';') {
    Row dataRow;
    dataRow.name = "";
    dataRow.email = "";
    dataRow.department = "";
    dataRow.position = "";
    dataRow.project = "";
    dataRow.task = "";
    dataRow.date = "";
    dataRow.loggedHours = 0;
    vector<string> exploaded;
    string tmp = "";

    for (int i = 0; i < row.length(); i++) {
        if (row[i] == delim) {
            exploaded.push_back(tmp);
            tmp = "";
            i++;
        }
        tmp += row[i];
    }

    // Header order, can be programmed
    // 0    1     2          3         4      5     5    7
    // Name;email;department;position;project;task;date;logged hours 
    dataRow.name = exploaded[0];
    dataRow.email = exploaded[1];
    dataRow.department = exploaded[2];
    dataRow.position = exploaded[3];
    dataRow.project = exploaded[4];
    dataRow.task = exploaded[5];
    dataRow.date = exploaded[6];
    dataRow.loggedHours = std::stoi(tmp);
    dataRow.day = createDay(dataRow.date);
    dataRow.month = createMonth(dataRow.day);

    return dataRow;
}

bool isHeaderLine(string str) {
    return regex_match(str, regex("(Name\\;email\\;).*"));
}

bool validate(string text) {
    string erors[12];

    // cerr << "Error message : " << str << endl;

    // char str[] = "Unable to read....";
    
    // clog << "Error message : " << str << endl;
    // validate delimeter            // Data is supplied in text file in CSV format. 
    // validate separator           // Separator is semicolon (;)
    // validate headers             // First line of the file is columns names. 

    // Name
    // email
    // department
    // position
    // project
    // task
    // date                         // Date is in YYYY-MM-DD format. 
    // logged
    // hours                        // Measuring parsing time. 

    return true;
}

void displayErros(string text) {
    cout << "displayErros" << endl;
}

string formatOutput(string text) {
    cout << "formatOutput" << endl;

    return "result formatOutput";
}

int main () {
    ifstream file;
    fstream rawFile;
    string formattedOutput;
    bool isInputTextValid = false;
    map<int, string> Rows;
    std::map<string, int> aggrData;

    Rows.clear();
    rawFile.open("raw.csv");
    // IN:
        // Name;email;department;position;project;task;date;logged hours
        // John Doe;john.doe@company.com;Delivery;Software Engineer;Mars;MRS-2240;2021-10-19;5 
        // John Doe;john.doe@company.com;Delivery;Software Engineer;Mars;MRS-2241;2021-10-20;8 
        // Jane Doe;jane.doe@company.com;Delivery;Manager;Venus;VNS-1111;2021-10-20;4 

    if (rawFile.is_open()) {  // cheacking whether the file is open
        string str;
        int line = 0;

        while(getline(rawFile, str)) {
            bool isHeader = isHeaderLine(str);

            if (false == isHeader) {
                Rows.insert(std::pair<int, string>(line, str));
            }

            cout << str << "\n";
            cout << isHeader << "\n";
            line++;
        }

        rawFile.close();
    }

    for (map<int,string>::iterator  itr = Rows.begin(); itr != Rows.end(); ++itr) {
        Row row = createRow(itr->second);
        string keyHash = createHash(row.loggedHours, row.month);

        if (aggrData.find(keyHash) == aggrData.end()) {
            aggrData[keyHash] = 0;
        }

        aggrData[keyHash] = aggrData[keyHash] + row.loggedHours;
    }

    // displayResultHere
    // OUT:
    //  Name;Month;Total hours
    //  John Doe;October 2021;13
    //  Jane Doe;October 2021;4

    return 0;
}