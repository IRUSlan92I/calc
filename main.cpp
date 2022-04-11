#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <math.h>
#include "Wagon.h"

std::vector<Wagon *> toPostfix(std::string s);
std::vector<Wagon *> splitToZug(std::string s);
std::vector<Wagon *> shuffleZug(std::vector<Wagon *> zug);
double calculate(std::vector<Wagon *> zug);
std::string zugToStr(std::vector<Wagon *>zug, char separator);
int getPriority(char operation);

int main(int argc, const char * argv[]) {
    std::string s;
    
    if (argc == 1) {
        std::cout << "Enter string:" << std::endl;
        std::cin >> s;
    }
    else {
        s = argv[1];
    }
    
    s += END_OF_ZUG;
    
    std::vector<Wagon *>zug = toPostfix(s);
    
    //std::cout << zugToStr(zug, ' ') << std::endl;
    
    std::cout << calculate(zug) << std::endl;
    
    return 0;
}

std::vector<Wagon *> toPostfix(std::string s)
{
    return shuffleZug(splitToZug(s));
}

std::string zugToStr(std::vector<Wagon *>zug, char separator)
{
    std::stringstream ss;
    for (int i = 0; i < zug.size(); i++) {
        if (zug[i]->isNumber())
            ss << zug[i]->getNumber() << separator;
        else
            ss << zug[i]->getOperation() << separator;
    }
    return ss.str();
}

std::vector<Wagon *> splitToZug(std::string s)
{
    std::vector<Wagon *>zug;
    
    double num = -1;
    long digitsAfterKomma = 0;
    bool isAfterKomma = false;
    
    for (int i = 0; i < s.length(); i++) {
        if ((s[i] >= '0')&&(s[i] <= '9')) {
            if (num < 0)
                num = 0;
            num += s[i] - '0';
            num *= 10;
            if (isAfterKomma) {
                digitsAfterKomma++;
            }
        }
        else if ((s[i] == ',')||(s[i] == '.')) {
            isAfterKomma = true;
        }
        else {
            if (num >= 0) {
                num /= 10;
                
                if (digitsAfterKomma) {
                    num /= pow(10, digitsAfterKomma);
                }
                
                Wagon *w = new Wagon();
                w->setNumber(num);
                zug.push_back(w);
                
                num = -1;
                digitsAfterKomma = 0;
                isAfterKomma = false;
            }
            
            if (s[i] == ' ') {
                continue;
            }
            
            Wagon *w = new Wagon();
            w->setOperation(s[i]);
            
            if (zug.size()) {
                Wagon *lastWagon = zug[zug.size()-1];
                if (((lastWagon->isOperation())&&(lastWagon->getOperation() != ')'))) {
                    if ((s[i] == '+')||(s[i] == '-'))
                        w->setOperation(-s[i]);
                }
            }
            else {
                if ((s[i] == '+')||(s[i] == '-'))
                    w->setOperation(-s[i]);
            }
            
            zug.push_back(w);
        }
    }
    
    return zug;
}

//3+4*2/(1-5)^2
//3 4 2 * 1 5 - 2 ^ / +

//(8+2*5)/(1+3*2-4)
//8 2 5 * + 1 3 2 * + 4 - /

//1+2*3*4+(5-6)*(7*8+9)
//1 2 3 * 4 * + 5 6 - 7 8 * 9 + * +

std::vector<Wagon *> shuffleZug(std::vector<Wagon *> zug)
{
    std::vector<Wagon *>endzug;
    std::stack<Wagon *>warte;
    
    for (int i = 0; i < zug.size(); i++) {
        if (zug[i]->isNumber()) {
            endzug.push_back(zug[i]);
        }
        else {
            switch (zug[i]->getOperation()) {
                case '(':
                    warte.push(zug[i]);
                    break;
                    
                case ')':
                    while (warte.size()) {
                        if (warte.top()->getOperation() != '(') {
                            endzug.push_back(warte.top());
                            if ((warte.size() == 1)&&(warte.top()->getOperation() != '(')) {
                                std::cout << "Error! '(' isn't fouded!" << std::endl;
                                exit(1);
                            }
                            warte.pop();
                        }
                        else {
                            warte.pop();
                            break;
                        }
                    }
                    break;
                    
                case '+':
                case '-':
                case '*':
                case '/':
                    while ((warte.size())&&(getPriority(zug[i]->getOperation()) <= getPriority(warte.top()->getOperation()))) {
                        endzug.push_back(warte.top());
                        warte.pop();
                    }
                    
                    warte.push(zug[i]);
                    break;
                    
                case '^':
                case -'+':
                case -'-':
                    while ((warte.size())&&(getPriority(zug[i]->getOperation()) < getPriority(warte.top()->getOperation()))) {
                        endzug.push_back(warte.top());
                        warte.pop();
                    }
                    
                    warte.push(zug[i]);
                    break;
                    
                    break;
                    
                case '|':
                    while (warte.size()) {
                        if (warte.top()->getOperation() != '(')
                            endzug.push_back(warte.top());
                        warte.pop();
                    }
                    break;
                    
                case ' ':
                    break;
                    
                default:
                    std::cout << "Error! Wrong symbol!" << std::endl;
                    exit(1);
                    break;
            }
        }
    }
    return endzug;
}

double calculate(std::vector<Wagon *> zug)
{
    std::stack<Wagon *>stack;
    for (int i = 0; i < zug.size(); i++) {
        stack.push(zug[i]);
        if (stack.top()->isOperation()) {
            char operation = stack.top()->getOperation();
            bool isUnary = stack.top()->isUnary();
            stack.pop();
            
            if (isUnary) {
                double number = stack.top()->getNumber();
                stack.pop();
                
                switch (operation) {
                    case -'+':
                        stack.push(new Wagon(number));
                        break;
                        
                    case -'-':
                        stack.push(new Wagon(-number));
                        break;

                    default:
                        std::cout << "Error! Wrong operation!" << std::endl;
                        exit(1);
                        break;
                }
            }
            else {
                double numberRight = stack.top()->getNumber();
                stack.pop();
                
                double numberLeft = stack.top()->getNumber();
                stack.pop();
                
                switch (operation) {
                    case '+':
                        stack.push(new Wagon(numberLeft + numberRight));
                        break;
                        
                    case '-':
                        stack.push(new Wagon(numberLeft - numberRight));
                        break;
                        
                    case '*':
                        stack.push(new Wagon(numberLeft * numberRight));
                        break;
                        
                    case '/':
                        stack.push(new Wagon(numberLeft / numberRight));
                        break;
                        
                    case '^':
                        stack.push(new Wagon(pow(numberLeft, numberRight)));
                        break;
                        
                    default:
                        std::cout << "Error! Wrong operation!" << std::endl;
                        exit(1);
                        break;
                }
            }
            
        }
    }
    if (stack.size() == 1) {
        return stack.top()->getNumber();
    }
    return 0;
}

int getPriority(char operation)
{
    switch (operation) {
        case '(':
        case ')':
            return 0;
            break;
            
        case '+':
        case '-':
            return 1;
            break;
            
        case '*':
        case '/':
            return 2;
            break;
            
        case '^':
            return 3;
            break;
            
        case -'+':
        case -'-':
            return 4;
            break;
            
        default:
            return 5;
            break;
    }
}