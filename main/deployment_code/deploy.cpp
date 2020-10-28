#include <iostream>
#include <string>

using namespace std;

// g++ ./main/deployment_code/deploy.cpp -o deploy

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
    string command = "g++ ./robot/main/main.cpp ./robot/main/hardware_interface.cpp -o ./robot/builds/build_"+version+" -lpigpio";
    system(command.c_str());
    string command_2 = "g++ ./robot/main/main.cpp ./robot/main/hardware_interface.cpp -o ./robot/builds/current -lpigpio";
    system(command_2.c_str());
}

void run()
{
    cout << "run stuff lmao";
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

        cout << "Do you want to build? (y/n) ";
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
            system("git commit -m \"ROBOT BUILD\"");
            system("git push origin master");
            run();
        }
        else
        {
            cout << "INVALID ANSWER\n";
        }
        
    }   

}


