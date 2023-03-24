#include <iostream>

int main(int argc, char *argv[])
{
    if (argc == 4) // passman-cli example.com{website/service} johndoe{account} password{master password} 0{reset counter}
    {
        // one-time mode
    }
    else if (argc != 1)
    {
        cout << "Invalid number of arguments." << endl;
    }
    // interactive mode

    return 0;
}