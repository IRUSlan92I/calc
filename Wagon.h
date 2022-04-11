#ifndef __calc__Wagon__
#define __calc__Wagon__

#include <stdio.h>

#define END_OF_ZUG '|'

class Wagon
{
private:
    double number;
    char operation;
    
public:
    Wagon();
    Wagon(double number);
    Wagon(char operation);
    
    double getNumber();
    char getOperation();
    
    void setNumber(double number);
    void setOperation(char operation);
    
    bool isNumber();
    bool isOperation();
    bool isUnary();
    bool isEnd();
};

#endif
