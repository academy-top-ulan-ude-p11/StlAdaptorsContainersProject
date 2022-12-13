#include <iostream>
#include <stack>
#include <string>
#include <map>

using namespace std;

int BracesCheck(string inStr)
{
    stack<char> braces;
    string brOpen = "([{";
    string brClose = ")]}";

    int pos{};
    bool isError = false;

    while (pos < inStr.length())
    {
        if (!(strchr(brOpen.c_str(), inStr[pos]) || strchr(brClose.c_str(), inStr[pos])))
        {
            pos++;
            continue;
        }

        if (strchr(brOpen.c_str(), inStr[pos]))
        {
            braces.push(inStr[pos]);
            pos++;
            continue;
        }

        int indexClose = brClose.find(inStr[pos]);
        if (braces.empty())
        {
            isError = true;
            break;
        }
        char braceTop = braces.top();
        int indexTop = brOpen.find(braceTop);

        if (indexClose != indexTop)
        {
            isError = true;
            break;
        }

        braces.pop();
        pos++;
    }

    if (!isError)
        isError = !braces.empty();

    return isError ? pos : -1;
}

string OpzCreate(string infExpresion, map<string, double> vars)
{
    stack<char> signsStack;
    int pos{};

    const string brOpen = "({[";
    const string brClose = ")}]";
    const string operators = "-+/*";
    const string opersMultDiv = "/*";
    const string opersPlusMinus = "-+";
    string opzExpresion;

    while (pos < infExpresion.length())
    {
        // пропускаем пробелы
        if (infExpresion[pos] == ' ')
        {
            pos++;
            continue;
        }

        // вычленяем числа и переносим в выходную строку
        if (isdigit(infExpresion[pos]))
        {
            string numsymbols = ".e";
            string number;
            while (pos < infExpresion.length() &&
                (isdigit(infExpresion[pos]) ||
                    (numsymbols.find_first_of(tolower(infExpresion[pos])) != string::npos)))
            {
                number.push_back(infExpresion[pos]);
                if (tolower(infExpresion[pos]) == 'e' && infExpresion[pos + 1] == '-')
                    number.push_back(infExpresion[++pos]);
                if (tolower(infExpresion[pos]) == 'e' && infExpresion[pos + 1] == '+')
                    pos++;
                pos++;
            }
            opzExpresion.append(number);
            opzExpresion.push_back('#');
            continue;
        }

        // вычлиняем переменные и подставляем в выходную строку их значения
        if (isalpha(infExpresion[pos]))
        {
            string varName;
            while (pos < infExpresion.length() &&
                (isalnum(infExpresion[pos]) || infExpresion[pos] == '_'))
            {
                varName.push_back(infExpresion[pos++]);
            }
            double value = vars[varName];
            opzExpresion.append(to_string(value));
            opzExpresion.push_back('#');
        }
        


        // отправляем в стек открывающуюся скобку
        if (brOpen.find_first_of(infExpresion[pos]) != string::npos)
        {
            signsStack.push(infExpresion[pos]);
            pos++;
            continue;
        }

        // прочитали закрывающую скобку - выталкиваем из стека оперторы в выходную строку
        if (brClose.find_first_of(infExpresion[pos]) != string::npos)
        {
            while (!signsStack.empty() && brOpen.find_first_of(signsStack.top()) == string::npos)
            {
                opzExpresion.push_back(signsStack.top());
                signsStack.pop();
            }
            signsStack.pop();
        }

        // встретили умножение или деление - выталкиваем (если есть) все умножения и деления в выходную строку
        if (opersMultDiv.find_first_of(infExpresion[pos]) != string::npos)
        {
            while (!signsStack.empty() && opersMultDiv.find_first_of(signsStack.top()) != string::npos)
            {
                opzExpresion.push_back(signsStack.top());
                signsStack.pop();
            }
            signsStack.push(infExpresion[pos]);
        }

        // встретили сложение или вычитание - выталкиваем (если есть) все операции кроме скобки в выходную строку
        if (opersPlusMinus.find_first_of(infExpresion[pos]) != string::npos)
        {
            while (!signsStack.empty() && operators.find_first_of(signsStack.top()) != string::npos)
            {
                opzExpresion.push_back(signsStack.top());
                signsStack.pop();
            }
            signsStack.push(infExpresion[pos]);
        }

        pos++;
    }

    while (!signsStack.empty())
    {
        opzExpresion.push_back(signsStack.top());
        signsStack.pop();
    }

    return opzExpresion;
}

double OpzCalculate(string opzExpresion)
{
    stack<double> numbersStack;
    const string operators = "-+/*";

    int pos = 0;

    while (pos < opzExpresion.length())
    {
        // читаю из опз строки число и кладу в стек
        if (isdigit(opzExpresion[pos]))
        {
            string number;
            while (pos < opzExpresion.length() && opzExpresion[pos] != '#')
                number.push_back(opzExpresion[pos++]);
            pos++;
            numbersStack.push(stod(number));
            continue;
        }

        // читаю знак оператора и вычисляю, а результат кладу в стек.
        if (operators.find_first_of(opzExpresion[pos]) != string::npos)
        {
            double operandRight = numbersStack.top();
            numbersStack.pop();
            double operandLeft = numbersStack.top();
            numbersStack.pop();

            double result;

            switch (opzExpresion[pos])
            {
            case '+': result = operandLeft + operandRight; break;
            case '-': result = operandLeft - operandRight; break;
            case '*': result = operandLeft * operandRight; break;
            case '/': result = operandLeft / operandRight; break;
            default:
                break;
            }

            numbersStack.push(result);
        }
        pos++;
    }
    return numbersStack.top();
}

int main()
{
    map<string, double> vars;
    vars.insert(make_pair("a", 24));
    vars.emplace("b", 5);

    string infExpresion = "a / (b + 7)";
    
    
    string opz = OpzCreate(infExpresion, vars);
    cout << opz << "\n";
    cout << OpzCalculate(opz) << "\n";



}
