#include "base.h"

using namespace std;

void FileTest(void)
{
    char filepath[100];
    ifstream in_file;
    ofstream out_file;
    char address[21];
    cout << "\nPlease input the path and name that you want to test!" << endl;
    cout << "\n\t C:\\temp\\myfile.trace" << endl;
    cout << "\n\t myfile.trace" << endl;
    cin >> filepath;
    in_file.open(filepath,ios::in);
    out_file.open("out.log",ios::out);
    while(in_file.fail())
    {
        cout << "Open ERROR! Please Check the Path and Name, and Input again!" << endl;
        cin >> filepath;
        in_file.open(filepath,ios::in);
    }


    while(!in_file.eof())
    {
        in_file.getline(address,21);
        bool __attribute__((unused)) is_success = GetHitNum(address, out_file); //in case of the warning of "Wunused-but-set-variable"
        assert(is_success);

    }

    in_file.close();
    out_file.close();
    GetHitRate();
}
