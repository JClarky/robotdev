#include <iostream>

using namespace std;

void full_deployment()
{
    cout << "\nFULL DEPLOYMENT SELECTED\n";

    system("\"copy .\\main\\main_code .\\robot\\main\"");
    system("\"del .\\robot\\main\\simulation_interface.cpp\"");

    system("git_update.bat");
}

void sim_deployment()
{
    cout << "\nSIMULATION DEPLOYMENT SELECTED\n";

    system("\"copy .\\main\\main_code .\\simulation\\main\"");
    system("\"del .\\simulation\\main\\hardware_interface.cpp\"");

    system("sim_build.bat");

    system("git_update.bat");
}

int main()
{
    cout << "DEPLOYMENT INITIALISED\n";

    bool deploying = false;

    while(!deploying)
    {
        // Full deployment or just sim deployment
        string user_dec;

        cout << "Is this a full deployment? (y/n) ";
        cin >> user_dec;

        if(user_dec == "y")
        {
            deploying = true;
            full_deployment();
        }
        else if (user_dec == "n")
        {
            deploying = true;
            sim_deployment();
        }
        else
        {
            cout << "INVALID ANSWER\n";
        }
        
    }   

}


