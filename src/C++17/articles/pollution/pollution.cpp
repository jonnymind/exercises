#include <iostream>
#include <string>

class WorkSchedule {
public:
    double worktime;
    double overtime;

    WorkSchedule(double wt, double ot): worktime{wt}, override{ot} {}
};

class Employee {
public:
    // Stuff about the employee.
    std::string name;
    std::string department;
    double hourlyWage;
    double overtimeRate;
    Employee(std::string nm, std::string dept, double hw, double or):
        name{nm}, department{dept}, hourlyWage{hw}, overtimeRate{or} {}
    
    // Stuff about the employee work.
    std::vector<WorkSchedule> schedule;

    // Stuff about state and computation.
    std::vector<double> dailyWage;

    // Stuff about computation result
    double thisMonthWage;
    double thisMonthTax;
};



int main(int argc, char* argv[]) {

    return 0;
}