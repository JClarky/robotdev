#include <iostream>
#include <string>

using namespace std;

void sim_build()
{
    string version;
    cout << "Build Version: ";
    cin >> version;
    string command = "g++ ./simulation/main/main.cpp ./simulation/main/simulation_interface.cpp -o ./simulation/builds/build_"+version+" -ljsoncpp";
    system(command.c_str());
}

void git_update()
{
    system("git add .");
    system("git commit -m \"SYNC MAIN FOLDER\"");
    system("git push origin master");
}

void full_deployment()
{
    cout << "\nFULL DEPLOYMENT SELECTED\n";

    system("rsync -av ./main/main_code/ ./robot/main/");
    system("rm ./robot/main/simulation_interface.cpp");

    system("rsync -av ./main/main_code/ ./simulation/main/");
    system("rm ./simulation/main/hardware_interface.cpp");

    sim_build();

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


