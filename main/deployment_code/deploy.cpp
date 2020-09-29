#include <iostream>

using namespace std;

void full_deployment()
{
    cout << "\nFULL DEPLOYMENT SELECTED\n";

    
}

void sim_deployment()
{
    cout << "\nSIMULATION DEPLOYMENT SELECTED\n";

    system("git add ../main_code/*");
    system("git reset ../main_code/hardware_interface.cpp");
    system("git commit -m \"SIM DEPLOYMENT\"");
    system("git push origin simulation");
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


