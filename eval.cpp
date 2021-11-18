//
//  eval.cpp
//  CS 32 Homework 2
//
//  Created by Sophia Liu on 1/28/21.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;

//declarations
int evaluate(string infix, const bool values[], string& postfix, bool& result);
int precedence(char c);
bool validatePrecedence(char a, char b);
bool isOperator(char c);
bool validateSyntax(string infix);
string infixToPostFix(string infix);
int evaluatePostFix(string postfix, const bool values[]);
int performOperation(stack<int> nums, stack<char> ops, const bool values[]);
bool checkParenthesesAgain(string infix, const bool values[]);

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
     //check conditions for valid syntax
     //check 1: closing parehthesis
    if (infixToPostFix(infix) == "BADSYNTAX" || !checkParenthesesAgain(infix,values))
         return 1;
    //no empty parentheses and no unmatched parentheses!!!
     postfix = infixToPostFix(infix);
     cout << postfix << endl;
     //check 2: operators must be preceded by 2 operands
     //check 3: 2 operands must be followed by an operator
     //check : can't use '!' (NOT) if operand 1 & 2 are the same value (CHECKED in validOperation)
     if (evaluatePostFix(postfix,values) == -1)
         return 1;// invalid
     if (evaluatePostFix(postfix,values) == 1) {result = true; return 0;}
     if (evaluatePostFix(postfix,values) == 0) {result = false; return 0;}
  return 1;
}

bool checkParentheses(string infix)
{
    int countParentheses = 0;
    int countInBetween = 0;
    for (int k=0; k < infix.size(); k++)
        {if (infix.at(k)=='(' || infix.at(k)==')') countParentheses++;}
    if (countParentheses != 0 && countParentheses%2 != 0)
        return false;
    if (countParentheses == 0)
        return true;
    for (int k=0; k < infix.size(); k++)
        {if (infix.at(k) == '(')
            {
             int n = 1;
             while (k+n<infix.size() && infix.at(k+n) != ')')
                {if (!isOperator(infix.at(k+n)))
                    countInBetween++;
                 n++;}
             if (countInBetween == 0)
                return false;
            }
        }
    return true;
}

bool checkParenthesesAgain(string infix, const bool values[])
{
    string betweenParentheses;
    for (int k=0; k < infix.size(); k++){
        betweenParentheses.clear();
        if (infix.at(k) == '(' && infix.at(k+1) != '(')
            {for (int n=1; k+n<infix.size(); n++)
                {if (infix.at(k+n) == ')')
                    break;
                betweenParentheses += infix.at(k+n);}
            }
        if (betweenParentheses.size()>0 && evaluatePostFix(infixToPostFix(betweenParentheses), values) == -1)
            return false;
    }
    return true;
}

string infixToPostFix(string infix)
{
 if (infix.empty())
     return "BADSYNTAX";
 if (!checkParentheses(infix))
    return "BADSYNTAX"; //no empty parentheses and no unmatched parentheses!!!
 string postfix;// Initialize postfix to empty
 stack<char> ops; //Initialize the operator stack to empty
 for (int k=0; k < infix.size(); k++){//For each character ch in the infix string
    char Character = infix.at(k);
    bool switchElse = true;
        switch (Character){
            case '0': case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9':
                postfix += Character;//append ch to end of postfix
                switchElse = false;
                break;
            case '(':
                ops.push(Character);//push ch onto the operator stack
                switchElse = false;
                break;
            case ')':
            //pop stack until matching '('
                switchElse = false;
                while (ops.top() != '(')
                    {postfix += ops.top();
                    ops.pop();}
                ops.pop();  // remove the '('
                break;
            case '!': case '|': case '&':
                switchElse = false;
                while (!ops.empty() && ops.top() != '('&& validatePrecedence(Character, ops.top()))
                    //While the stack is not empty and the stack top is not '(' and precedence of ch <= precedence of stack top
                    {postfix += ops.top(); //append the stack top to postfix
                    ops.pop();}//pop the stack
                ops.push(Character); //push ch onto the stack
                break;
            case ' ':
                switchElse = false;
                break;
        }
     if (switchElse)
        return "BADSYNTAX";
    }
    while (!ops.empty())//While the stack is not empty
        {postfix += ops.top(); //append the stack top to postfix
         ops.pop();} //pop the stack
    
    return postfix;
}

bool isOperator(char c)
{return (c=='&'||c=='|'||c=='!');}

int performOperation(int a, int b, char operation) {
        switch (operation) {
            case '|':   // The result is true if either one of the two bits is true, or if both are true. If none is equal to true the result is 0.
                if (a == 1 || b == 1) return 1;
                return 0;
            case '&': // The result of applying this AND operation is true if both a and b are true, and false in all other cases (i.e., if one or both of the variables is 0).
                if (a==1 && b==1) return 1;
                return 0;
            case '!':
                if (a == 1) return 0;
                if (a == 0) return 1;
        }
    return -1;
}


bool validOperation(int a, int b, char operation)
{return performOperation(a, b, operation) != -1;}

int evaluatePostFix(string postfix, const bool values[])
{stack<int> bools; //Initialize the operand stack to empty
 stack<char> ops;
 int invalid = 0;
    
 for (int k=0; k < postfix.size(); k++) // For each character ch in the postfix string
    {
    char Character = postfix.at(k);
    if (!isOperator(Character)){// is an operand
        if(values[Character-'0'] == true)
            bools.push(1);
        if(values[Character-'0'] == false)
            bools.push(0);} //push the value that ch represents onto the operand stack (1 for true, 0 for false)
    
    if (isOperator(Character)){// ch is an operator
        if (Character != '!' && bools.size()<2)
            {invalid++;
             break;}
        if (Character == '!'){
            int operand1 = bools.top(); bools.pop();
            bools.push(performOperation(operand1, 8, '!')); //only use 1 character for !, 8 is arbitrary
                }
        if (Character != '!' && bools.size()>=2){ //otherwise must have 2 operands to work with
            int operand2 = bools.top(); bools.pop(); //set operand2 to the top of the operand stack
            int operand1 = bools.top(); bools.pop();  //set operand1 to the top of the operand stack
            char Operator = postfix.at(k);
            if (!validOperation(operand1, operand2, Operator)) //might need to switch order of operands
                {invalid++; break;}
            if (validOperation(operand1, operand2, Operator))
                bools.push(performOperation(operand1, operand2, Operator));//apply the operation that ch represents to operand1 and operand2, and push the result onto the stack
                }
            }
    //if it's a space, nothing will happen, move to next char in string
        }
    
    if (invalid != 0) return -1;
    
    if (bools.size() ==1)
        return bools.top(); //should be the last value
    
    return -1;
}

int precedence(char c){
        switch (c){
            case '|':
                return 1;
            case '&':
                return 2;
            case '!':
                return 3;
        }
        return -1;
    }

bool validatePrecedence(char a, char b)
{return precedence(a)<=precedence(b);}


//test main
int main()
{
 bool ba[10] = {
//  0      1      2      3      4      5      6      7      8      9
  true,  true,  true,  false, false, false, true,  false, true,  false};
 string pf;
 bool answer;
 assert(ba[2] == true);
 assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  answer);
 assert(evaluate("8|", ba, pf, answer) == 1);
 assert(evaluate ("2", ba, pf, answer) == 0 && answer);
 assert(evaluate(" &6", ba, pf, answer) == 1);
 assert(evaluate("4 5", ba, pf, answer) == 1);
 assert(evaluate("01", ba, pf, answer) == 1);
 assert(evaluate("()", ba, pf, answer) == 1);
 assert(evaluate("()4", ba, pf, answer) == 1);
 assert(evaluate("2(9|8)", ba, pf, answer) == 1);
 assert(checkParenthesesAgain("2(&8)",ba) == false);
 assert(evaluate("2(&8)", ba, pf, answer) == 1);
 assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
 assert(evaluate("x+5", ba, pf, answer) == 1);
 assert(evaluate("", ba, pf, answer) == 1);
 assert(evaluate("2", ba, pf, answer) == 0 && answer);
 assert(evaluate("(3)", ba, pf, answer) == 0 && !answer);
 assert(evaluate("0 & !9", ba, pf, answer) == 0 && answer);
 assert(evaluate("!(7|8)", ba, pf, answer) == 0 &&  !answer);
 assert(evaluate("!7|8", ba, pf, answer) == 0 && answer);
 assert(evaluate("6|4&5", ba, pf, answer) == 0 && answer);
 assert(evaluate(")8(", ba, pf, answer) == 1);
 assert(evaluate("*", ba, pf, answer) == 1);
 assert(evaluate("2|3|4", ba, pf, answer) == 0 &&  pf == "23|4|"  &&  answer);
 assert(evaluate("2|(3|4)", ba, pf, answer) == 0 &&  pf == "234||"  && answer);
 assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0 &&  pf == "43!03&&|"  &&  !answer);
 assert(evaluate(" 9  ", ba, pf, answer) == 0  &&  pf == "9"  &&  !answer);
 assert(evaluate("1&!(9|1&1|9) | !!!(9&1&9)", ba, pf, answer) == 0 && answer);
 ba[2] = false;
 ba[9] = true;
 assert(evaluate("!(99)",ba,pf,answer) == 1);
 assert(evaluate("",ba,pf,answer) ==1);
 assert(evaluate("9", ba, pf, answer) == 0  &&  pf == "9"  &&  answer);
 assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  !answer);
 cout << "Passed all tests" << endl;
        }
