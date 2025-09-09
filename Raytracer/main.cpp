#include "RayTracer.h"
#include <iostream>
#include <string>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "json.hpp"
using namespace std;


int main()
{
    
    string choice;
    cout << "Choose a scene:" << endl;
    cin >> choice;
    string newStr = "assets\\" + choice + ".json";
    string ppmStr = "assets\\" + choice + ".ppm";
    char* ppmChar;
    char* strChar;

    strChar = &newStr[0];
    ppmChar = &ppmStr[0];
    //system(ppmChar);
    //--------------------------------------------------------------
    

        cout << "Scene: " << strChar << endl;

        std::ifstream t(strChar);
        if (!t) {
            cout << "File " << strChar << " does not exist!" << endl;
            return -1;
        }

        std::stringstream buffer;
        buffer << t.rdbuf();

        nlohmann::json j = nlohmann::json::parse(buffer.str());
        cout << "Parsed successfully" << endl;


        RayTracer rt(j);
        rt.run();




    return 0;
}

