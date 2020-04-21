#include <iostream>
#include "Program.h";

int main()
{
    Program* pProgram = new Program(10);
    int dummy = 0;
    std::cout << "Press Enter to continue\n";
    std::cin >> dummy;
    std::cout << "Hello World!\n";
    std::cout << pProgram->GetId();
    return 0;
}