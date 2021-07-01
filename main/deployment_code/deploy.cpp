#include <iostream>
#include <string>

using namespace std;

// g++ ./main/deployment_code/deploy.cpp -o deploy_windows
// g++ ./main/deployment_code/deploy.cpp -o deploy_rpi

void robot_build()
{
    string version;
    cout << "Robot build version: ";
    cin >> version;
    string command = "g++ ./robot/main/main.cpp ./robot/main/hardware_interface.cpp -o ./robot/builds/build_"+version+" -lpigpio";
    system(command.c_str());
    string command_2 = "g++ ./robot/main/main.cpp ./robot/main/hardware_interface.cpp -o ./robot/builds/current -lpigpio";
    system(command_2.c_str());
}

int main()
{
    cout << "DEPLOYMENT INITIALISED\n";

    system("git pull origin master");

    bool deploying = false;

    // Copy (xcopy and del) (cp and rm)
    // ./
    system("cp -R ./main/main_code/main.cpp ./robot/main");
    system("cp -R ./main/main_code/main.h ./robot/main");
    system("cp -R ./main/main_code/_interface/* ./robot/main");
    system("cp -R ./main/main_code/_modes/* ./robot/main");

    system("cp -R ./main/main_code/main.cpp ./simulation/main");
    system("cp -R ./main/main_code/main.h ./simulation/main");
    system("cp -R ./main/main_code/_interface/* ./simulation/main");
    system("cp -R ./main/main_code/_modes/* ./simulation/main");

    // Del
    system("rm ./robot/main/simulation_interface.cpp");
    system("rm ./simulation/main/hardware_interface.cpp");

    while(!deploying)
    {
        // Full deployment or just sim deployment
        string user_dec;

        cout << "Do you want to build robot code? (y/n) ";
        cin >> user_dec;       

        if(user_dec == "y")
        {
            deploying = true;
            robot_build();
            system("git add .");
            system("git commit -m \"ROBOT BUILD\"");
            system("git push origin master");
        }
        else if (user_dec == "n")
        {
            deploying = true;
            system("git add .");
            system("git commit -m \"DEPLOY\"");
            system("git push origin master");
        }
        else
        {
            cout << "INVALID ANSWER\n";
        }
        
    }   

}


