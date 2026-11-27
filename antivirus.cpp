#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    string filename;
    string line;

    cout << "Enter file name to restore: ";
    getline(cin, filename);

    if (filename.empty())
    {
        cout << "Invalid file name!" << endl;
        return 1;
    }

    ifstream backup("backup.txt");

    if (!backup)
    {
        cout << "No backup found!" << endl;
        return 1;
    }

    // Do not overwrite an existing file
    ifstream checkFile(filename);

    if (checkFile)
    {
        cout << "Destination file already exists!" << endl;
        return 1;
    }

    checkFile.close();

    ofstream restoredFile(filename);

    if (!restoredFile)
    {
        cout << "Could not create restored file!" << endl;
        return 1;
    }

    while (getline(backup, line))
    {
        restoredFile << line << '\n';
    }

    backup.close();
    restoredFile.close();

    cout << "File restored successfully!" << endl;

    return 0;
}
