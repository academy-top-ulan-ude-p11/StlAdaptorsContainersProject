#include <iostream>
#include <stack>

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

int main()
{
    string inStr;
    string opzStr;

    stack<char> signs;

    int pos{};

    while (pos < inStr.length())
    {
        if (isdigit(inStr[pos]))
        {
            string number;
            number.push_back(inStr[pos]);
        }
    }
    
    


}
