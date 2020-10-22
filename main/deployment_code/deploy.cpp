#include <iostream>
#include <string>

using namespace std;

// g++ ./main/deployment_code/deploy.cpp -o deploy

void sim_build()
{
    string version;
    cout << "Sim build version: ";
    cin >> version;
    string command = "g++ ./simulation/main/main.cpp ./simulation/main/simulation_interface.cpp -o ./simulation/builds/build_"+version+".exe -ljsoncpp";
    system(command.c_str());
}

void git_update()
{
    system("git add .");
    system("git commit -m \"SYNC MAIN FOLDER\"");
    system("git push origin master");
}

void robot_build()
{
    string version;
    cout << "Robot build version: ";
    cin >> version;
    string command = "g++ ./robot/main/main.cpp ./robot/main/hardware_interface.cpp -o ./robot/builds/build_"+version+".exe -lpigpio";
    system(command.c_str());
    string command_2 = "g++ ./robot/main/main.cpp ./robot/main/hardware_interface.cpp -o ./robot/builds/current.exe -lpigpio";
    system(command_2.c_str());
}

void full_deployment()
{
    cout << "\nFULL DEPLOYMENT SELECTED\n";

    system("rsync -av ./main/main_code/ ./robot/main/");
    system("rm ./robot/main/simulation_interface.cpp");

    system("rsync -av ./main/main_code/ ./simulation/main/");
    system("rm ./simulation/main/hardware_interface.cpp");

    sim_build();

    robot_build();

    git_update();
}

void sim_deployment()
{
    cout << "\nSIMULATION DEPLOYMENT SELECTED\n";

    system("rsync -a main/main_code/ simulation/main/");
    system("rm simulation/main/hardware_interface.cpp");

    sim_build();

    git_update();
}

int main()
{
    cout << "DEPLOYMENT INITIALISED\n";

    system("git pull origin master --rebase");

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


