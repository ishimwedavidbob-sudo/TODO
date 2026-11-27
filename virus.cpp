#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

int main()
{
    string filename;
    string line;

    cout << "Enter file name to delete: ";
    cin >> filename;

    ifstream input(filename);

    if (!input)
    {
        cout << "File not found!" << endl;
        return 1;
    }

    ofstream backup("backup.txt");

    while (getline(input, line))
    {
        backup << line << endl;
    }

    input.close();
    backup.close();

    if (remove(filename.c_str()) == 0)
    {
        cout << "File deleted successfully." << endl;
        cout << "Backup saved in backup.txt" << endl;
    }
    else
    {
        cout << "Error deleting file." << endl;
    }

    return 0;
}
