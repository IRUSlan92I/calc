#include "Wagon.h"

Wagon::Wagon() {
    this->number = -1;
    this->operation = '\0';
}

Wagon::Wagon(double number)
{
    this->number = number;
    this->operation = '\0';
}

Wagon::Wagon(char operation)
{
    this->number = -1;
    this->operation = operation;
}

double Wagon::getNumber()
{
    return this->number;
}

char Wagon::getOperation()
{
    return this->operation;
}

void Wagon::setNumber(double number)
{
    if (number < 0) {
        return;
    }
    this->number = number;
}

void Wagon::setOperation(char operation)
{
    this->operation = operation;
}

bool Wagon::isNumber()
{
    return !this->isOperation();
}

bool Wagon::isOperation()
{
    return this->getOperation();
}

bool Wagon::isUnary()
{
    return this->operation < 0;
}

bool Wagon::isEnd()
{
    return this->operation == END_OF_ZUG;
}