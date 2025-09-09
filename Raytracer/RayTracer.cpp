#include "RayTracer.h"
#include <stdlib.h>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
using namespace std;

/*
Roadmap Completed:
    week 1
    week 2
    week 3
    week 4
    week 5 (official date Feb 16th)

*/
extern bool helper::globalillum = false;
extern bool helper::antialiasing = false;
extern double helper::max_bounce = 1.0;
extern Eigen::Vector2d helper::raysperpixel(1, 1);
extern double helper::probterminate = 1;
extern Eigen::Vector2f** helper::grid = new Eigen::Vector2f*[1];
extern int helper::speedup = 1;


// Global queue and synchronization primitives
std::queue<Node<int>**> taskQueue;   // queue of partitions
std::mutex queueMutex;
std::condition_variable queueCV;
std::atomic<bool> found(false);      // signal for early cancellation



RayTracer::RayTracer(json& j)
{
  
    parseGeo(j);

    // 2 - parse lights
    parseLight(j);

    // 3 - parse lights
    parseOutput(j);
    cout << "\nFinished parsing\n";
    //system("PAUSE");
}
RayTracer::RayTracer()
{

}
RayTracer::~RayTracer()
{

}


void RayTracer::run()
{

  
    delete[] helper::grid;
   
   

    float maxX = 0, maxY = 0, maxZ = 0;
    float minX = 0, minY = 0, minZ = 0;

    vector<Geometry*> g; //vector for all objects
    vector<Light*> l;    //vector for all lights
    

    for (int i = 0; i < myJ.geo.size; i++)
    {
        if (myJ.geo[i]->type == "sphere")
            g.push_back(new Sphere(myJ.geo[i]->comment,myJ.geo[i]->centre, myJ.geo[i]->radius, myJ.geo[i]->ka, myJ.geo[i]->kd, myJ.geo[i]->ks, myJ.geo[i]->ac, myJ.geo[i]->dc, myJ.geo[i]->sc, myJ.geo[i]->pc));
        if (myJ.geo[i]->type == "rectangle")
            g.push_back(new Rectangle(myJ.geo[i]->comment,myJ.geo[i]->P1, myJ.geo[i]->P2, myJ.geo[i]->P3, myJ.geo[i]->P4, myJ.geo[i]->ka, myJ.geo[i]->kd, myJ.geo[i]->ks, myJ.geo[i]->ac, myJ.geo[i]->dc, myJ.geo[i]->sc, myJ.geo[i]->pc));


        if (g.back()->xInterval[0] < minX)
            minX = g.back()->xInterval[0];
        if (g.back()->xInterval[1] > maxX)
            maxX = g.back()->xInterval[1];

        if (g.back()->yInterval[0] < minY)
            minY = g.back()->yInterval[0];
        if (g.back()->yInterval[1] > maxY)
            maxY = g.back()->yInterval[1];

        if (g.back()->zInterval[0] < minZ)
            minZ = g.back()->zInterval[0];
        if (g.back()->zInterval[1] > maxZ)
            maxZ = g.back()->zInterval[1];

    }
    minX = floorf(minX * 1000) / 1000;
    maxX = floorf(maxX * 1000) / 1000;

    minY = floorf(minY * 1000) / 1000;
    maxY = floorf(maxY * 1000) / 1000;

    minZ = floorf(minZ * 1000) / 1000;
    maxZ = floorf(maxZ * 1000) / 1000;





    for (int i = 0; i < myJ.light.size; i++)
    {
        if (myJ.light[i]->type == "point")
            l.push_back(new Light(myJ.light[i]->type, myJ.light[i]->centre, myJ.light[i]->id, myJ.light[i]->is, myJ.outP[0]->ai,myJ.light[i]->usecenter));
        else
            l.push_back(new Light(myJ.light[i]->type, myJ.light[i]->P1, myJ.light[i]->P2, myJ.light[i]->P3, myJ.light[i]->P4, myJ.light[i]->id, myJ.light[i]->is, myJ.outP[0]->ai, myJ.light[i]->usecenter));
    }



    for (int outP = 0; outP < myJ.outP.size; outP++)
    {
        double imgWidth = myJ.outP[outP]->size[0];
        double imgHeight = myJ.outP[outP]->size[1];


        Eigen::Vector3f bkc = myJ.outP[outP]->bkc;
        vector<double> buffer(3 * imgWidth * imgHeight); //vector that will hold the color information for each pixel
        



        /*

        My Camera

        */
        helper::max_bounce = myJ.outP[outP]->maxbounces;
        helper::globalillum = myJ.outP[outP]->globalillum;
        helper::probterminate = myJ.outP[outP]->probterminate;
        helper::antialiasing = myJ.outP[outP]->antialiasing;
        helper::speedup = myJ.outP[outP]->speedup;
        helper::raysperpixel = myJ.outP[outP]->raysperpixel;
        


        string filename = myJ.outP[outP]->filename;
        

        Camera cam(myJ.outP[outP]->centre, myJ.outP[outP]->lookat, myJ.outP[outP]->fov, imgWidth / imgHeight, myJ.outP[outP]->up);
        cam.bkc = myJ.outP[outP]->bkc;
        cam.imgWidth = imgWidth;
        cam.imgHeight = imgHeight;
        /*
        Frustum
        */


        double near = 1, far = 2;


        /*
        Camera Information
        */
        double angle = (cam.fov) * helper::pi / (double)180.0;
        double h = tan((angle / (double)2.0));
        double F = 1.0 / h;


        Eigen::Matrix4d projection;
        projection(0, 0) = F / (cam.aspectRatio);
        projection(1, 0) = 0;
        projection(2, 0) = 0;
        projection(3, 0) = 0;

        projection(0, 1) = 0;
        projection(1, 1) = F;
        projection(2, 1) = 0;
        projection(3, 1) = 0;

        projection(0, 2) = 0;
        projection(1, 2) = 0;
        projection(2, 2) = (near + far) / (near - far);
        projection(3, 2) = -1;

        projection(0, 3) = 0;
        projection(1, 3) = 0;
        projection(2, 3) = (2 * near * far) / (near - far);
        projection(3, 3) = 0;

        cam.projectionMatrix = projection;
        cam.inverseProjMatrix = projection.inverse();
        cam.mvpMatrix = cam.inverseViewMatrix * cam.inverseProjMatrix;
        Eigen::Vector4d d;
        //------------------------------------------




        int closestIndex = 0;


        //The origin of the ray should be the camera's position
        //Direction vector will be changed later
        Ray r(Eigen::Vector3d(cam.position[0], cam.position[1], cam.position[2]), Eigen::Vector3d(0, 0, 1));
        Eigen::Vector3f color;
        double u;
        double v;
        
        g = zSort(g);
        
        

        /*Stratified Sampling Grid*/


        if (helper::raysperpixel[0] > 1)
        {
	    int rs = helper::raysperpixel[0];
            helper::grid = new Eigen::Vector2f*[rs];

            for (int i = 0; i < helper::raysperpixel[0]; i++)
            {
                helper::grid[i] = new Eigen::Vector2f[rs];
                for (int j = 0; j < helper::raysperpixel[0]; j++)
                {
      
                    float x = (realUniRand(0.0, 0.9));
                    float y = (realUniRand(0.0, 0.9));
                    helper::grid[i][j] = Eigen::Vector2f(x, y);
                }

            }
        }

        /* THREADED SOLUTION */

        int choice;
        cout << "How many threads should be created?" << endl;
        cin >> choice;
        cout << endl;

       /* THREADED SOLUTION */


        cout << "Running RayTracer..." << endl;
        auto start = chrono::system_clock::now();

        /* THREADED SOLUTION */

        makeThreads(choice, (int)(3 * imgWidth * imgHeight), &buffer, &g, &l, cam);

        /* THREADED SOLUTION*/



         /* NON-THREADED SOLUTION 
        Eigen::Vector4d t;
        Eigen::VectorXd p;
        bool sceneHit = false;
        Eigen::Vector4d transform;


        for (double i = 0; i < imgHeight; i++)
        {
            for (double j = 0; j < imgWidth; j++)
            {

                color = Eigen::Vector3f(0, 0, 0);

                if (i == imgHeight/2.0 && j == imgWidth/2.0)
			        cout <<"\n\n50% Done...\n\n";
                


				if (helper::globalillum)
				{
                    

                
                        for (int s = 0; s < helper::raysperpixel[0]; s++)
                        {
                            for (int ss = 0; ss < helper::raysperpixel[0]; ss++)
                            {
                                for (int sss = 0; sss < helper::raysperpixel[1]; sss++)
                                {
                                    u = ((j + helper::grid[s][ss][0]) / ((double)(imgWidth) / 2.0)) - 1.0;   //x position 
                                    v = 1.0 - (((i + helper::grid[s][ss][1]) / ((double)(imgHeight) / 2.0)));  //y position


                                    transform = Eigen::Vector4d(u, v, 1, 1);

                                    //Matrix multiplication to find the direction vector
                                    d = cam.mvpMatrix * transform;

                                    r.directionV = Eigen::Vector3d(d.x(), d.y(), d.z());

                                    //HitContainer hit = tree.findIntersect(r, NULL, false);
                                    HitContainer hit = loopForHits(r, g, false, NULL);



                                    if (hit.g == NULL)
                                    {
                                        color += cam.bkc;
                                    }
                                    else
                                    {
                                        color += vecMult(hit.g->ka * l[0]->ai, hit.g->ac);
                                        for (int m = 0; m < l.size(); m++)
                                        {
                                            Eigen::Vector3f newColor = l[m]->getColor(hit.g, cam, g, hit.hitPixel);
                                            color += newColor;
                                        }

                                    }
                                }
                                

                            }

                        }
                        color /= (helper::raysperpixel[0]* helper::raysperpixel[0] * helper::raysperpixel[1]);
                    //}
					

				}
                else if (!helper::antialiasing)
                {
                    u = ((j) / ((double)(imgWidth) / 2.0)) - 1.0;   //x position 
                    v = 1.0 - (((i) / ((double)(imgHeight) / 2.0)));  //y position



                    transform = Eigen::Vector4d(u, v, 1, 1);

                    //Matrix multiplication to find the direction vector
                    d = cam.mvpMatrix * transform;

                    r.directionV = Eigen::Vector3d(d.x(), d.y(), d.z());

                    HitContainer hit = loopForHits(r, g, false, NULL);




                    if (hit.g == NULL)
                        color = cam.bkc;
                    else
                    {

                        color += vecMult(hit.g->ka * l[0]->ai, hit.g->ac);
                        for (int m = 0; m < l.size(); m++)
                        {
                            Eigen::Vector3f newColor = l[m]->getColor(hit.g, cam, g, hit.hitPixel);
                            color += newColor;
                        }



                    }
                }
                else if (helper::antialiasing)
                {

                    double sampleSize = helper::raysperpixel[0];


                    for (int s = 0; s < sampleSize; s++)
                    {


                        u = ((j + helper::grid[0][s][0]) / ((double)(imgWidth) / (double)2.0)) - 1.0;   //x position 
                        v = 1 - (((i + helper::grid[0][s][1]) / ((double)(imgHeight) / (double)2.0)));  //y position

                        transform = Eigen::Vector4d(u, v, 1, 1);
                        d = cam.mvpMatrix * transform;

                        r.directionV = Eigen::Vector3d(d.x(), d.y(), d.z());


                        HitContainer hit = loopForHits(r, g, false, NULL);




                        if (hit.g == NULL)
                            color += cam.bkc;
                        else
                        {
                            color += vecMult(hit.g->ka * l[0]->ai, hit.g->ac);
                            for (int m = 0; m < l.size(); m++)
                            {
                                Eigen::Vector3f newColor = l[m]->getColor(hit.g, cam, g, hit.hitPixel);
                                color += newColor;
                            }


                        }
   

                    }
                    color /= sampleSize;
                }
                 




                
                color = colorOverflow(color);

                //Write the color to the buffer
                buffer[3 * i * imgWidth + 3 * j + 0] = color[0];
                buffer[3 * i * imgWidth + 3 * j + 1] = color[1];
                buffer[3 * i * imgWidth + 3 * j + 2] = color[2];



                closestIndex = 0;


            }
        }

       /*  NON - THREADED SOLUTION */

         //--------------
        auto end = chrono::system_clock::now();

        chrono::duration<double> elapsed_seconds = end - start;
        cout << "\n\nTime Elapsed: " << elapsed_seconds.count() << endl;


        saveppm(filename, buffer, imgWidth, imgHeight);

        if (helper::raysperpixel[0] > 1)
        {

            for (int i = 0; i < helper::raysperpixel[0]; i++)
            {
                delete[] helper::grid[i];
                helper::grid[i] = NULL;
            }
            delete[] helper::grid;


        }


        string s = filename;
        char* ppmChar;
        ppmChar = &s[0];

        system(&s[0]);
            

        
    }


    myJ.geo.destroy();
    myJ.light.destroy();
    myJ.outP.destroy();

    for (auto p : g)
        delete p;
    for (auto p : l)
        delete p;

}


bool RayTracer::parseGeo(nlohmann::json& j)
{
    cout << "Geometry: " << endl;
    Eigen::Vector3f temp;
    int gc = 0;
    string theStr[7] = { "P1","P2","P3","P4","ac","dc","sc" };

    for (auto itr = j["geometry"].begin(); itr != j["geometry"].end(); itr++) {
        myJ.geo.addNode(make_shared<GeometryJ>());

        std::string type;
        if (itr->contains("type")) {
            myJ.geo[gc]->type = (*itr)["type"].get<std::string>();
        }
        else {
            cout << "Fatal error: geometry shoudl always contain a type!!!" << endl;
            return false;
        }
        if (itr->contains("comment")) {
            myJ.geo[gc]->comment = (*itr)["comment"].get<std::string>();
        }
        else
            myJ.geo[gc]->comment = "";
        if (myJ.geo[gc]->type == "rectangle")
        {
            cout << "Rectangle: " << endl;
            for (int i = 0; i < 4; i++)
            {
                if (itr->contains(lowercase(theStr[i])))
                {
                    getElements(itr, lowercase(theStr[i]), 3, temp);
                    *myJ.geo[gc]->FloatMemberMap[theStr[i]](*myJ.geo[gc]) = temp;
                    cout << theStr[i] << ": ";
                    vPrint(theStr[i], myJ.geo[gc]);
                }
                else if (itr->contains(uppercase(theStr[i])))
                {
                    getElements(itr, uppercase(theStr[i]), 3, temp);
                    *myJ.geo[gc]->FloatMemberMap[theStr[i]](*myJ.geo[gc]) = temp;
                    cout << theStr[i] << ": ";
                    vPrint(theStr[i], myJ.geo[gc]);
                }
            }

        }
        if (myJ.geo[gc]->type == "sphere") {
            cout << "Sphere: " << endl;

            //centre     
            getElements(itr, "centre", 3, temp);
            myJ.geo[gc]->centre = temp;


            cout << "Centre: ";
            vPrint("centre", myJ.geo[gc]);

            //radius
            if (itr->contains("radius")) {
                myJ.geo[gc]->radius = (*itr)["radius"].get<double>();
            }
            else {
                cout << "Fatal error: sphere shoudl always have a radius!!!" << endl;
                return false;
            }
            cout << "Radius: " << myJ.geo[gc]->radius << endl;


        }


        for (int i = 4; i < 7; i++)
        {
            if (itr->contains(theStr[i]))
            {
                getElements(itr, theStr[i], 3, temp);
                for (int j = 0; j < 3; j++)
                    *myJ.geo[gc]->FloatMemberMap[theStr[i]](*myJ.geo[gc]) = temp;
                cout << theStr[i] << ": ";
                vPrint(theStr[i], myJ.geo[gc]);
            }
        }



        if (itr->contains("ka"))
            myJ.geo[gc]->ka = (*itr)["ka"].get<double>();

        if (itr->contains("kd"))
            myJ.geo[gc]->kd = (*itr)["kd"].get<double>();

        if (itr->contains("ks"))
            myJ.geo[gc]->ks = (*itr)["ks"].get<double>();

        if (itr->contains("pc"))
            myJ.geo[gc]->pc = (*itr)["pc"].get<double>();

        ++gc;


    }

    cout << "We have: " << gc << " objects!" << endl;
    return true;
}
bool RayTracer::parseLight(nlohmann::json& j)
{
    cout << "Light: " << endl;
    int lc = 0;
    Eigen::Vector3f temp;
    string theStr[6] = { "P1","P2","P3","P4","id","is" };
    // use iterators to read-in array types
    for (auto itr = j["light"].begin(); itr != j["light"].end(); itr++) {
        myJ.light.addNode(make_shared<LightJ>());
        std::string type;
        if (itr->contains("type")) {
            myJ.light[lc]->type = (*itr)["type"].get<std::string>();
        }
        else {
            cout << "Fatal error: light shoudl always contain a type!!!" << endl;
            return false;
        }
        if (itr->contains("comment")) {
            myJ.light[lc]->comment = (*itr)["comment"].get<std::string>();
        }
        else
            myJ.light[lc]->comment = "";
        if (myJ.light[lc]->type == "area")
        {
            cout << "Area based light: " << endl;

            for (int i = 0; i < 4; i++)
            {
                if (itr->contains(lowercase(theStr[i])))
                {
                    getElements(itr, lowercase(theStr[i]), 3, temp);
                    *myJ.light[lc]->FloatMemberMap[theStr[i]](*myJ.light[lc]) = temp;

                    cout << theStr[i] << ": ";
                    vPrint(theStr[i], myJ.light[lc]);
                }
                else if (itr->contains(uppercase(theStr[i])))
                {
                    getElements(itr, uppercase(theStr[i]), 3, temp);
                    *myJ.light[lc]->FloatMemberMap[theStr[i]](*myJ.light[lc]) = temp;

                    cout << theStr[i] << ": ";
                    vPrint(theStr[i], myJ.light[lc]);
                }

            }

        }
        if (myJ.light[lc]->type == "point") {
            cout << "Point based light: " << endl;

            getElements(itr, "centre", 3, temp);
            myJ.light[lc]->centre = temp;
            cout << "Centre: ";
            vPrint("centre", myJ.light[lc]);
        }


        for (int i = 4; i < 6; i++)
        {
            if (itr->contains(theStr[i]))
            {
                getElements(itr, theStr[i], 3, temp);
                *myJ.light[lc]->FloatMemberMap[theStr[i]](*myJ.light[lc]) = temp;

                cout << theStr[i] << ": ";
                vPrint(theStr[i], myJ.light[lc]);
            }
        }
        if (itr->contains("usecenter"))
            myJ.light[lc]->usecenter = (*itr)["usecenter"].get<bool>();

        ++lc;

        cout << "\n\n";
    }
    cout << "We have: " << lc << " objects!" << endl;


    return true;
}
bool RayTracer::parseOutput(nlohmann::json& j)
{
    cout << "Outputs: " << endl;
    int lc = 0;
    string theStr[5] = { "centre","lookat","up","ai","bkc" };
    Eigen::Vector2d tempI;
    Eigen::Vector3f temp;


    // use iterators to read-in array types
    for (auto itr = j["output"].begin(); itr != j["output"].end(); itr++) {
        myJ.outP.addNode(make_shared<OutputJ>());
        std::string filename;
        if (itr->contains("filename")) {
            myJ.outP[lc]->filename = (*itr)["filename"].get<std::string>();
        }
        else {
            cout << "Fatal error: output shoudl always contain a filename!!!" << endl;
            return false;
        }

        if (itr->contains("globalillum"))
            myJ.outP[lc]->globalillum = (*itr)["globalillum"].get<bool>();
        if (itr->contains("antialiasing"))
            myJ.outP[lc]->antialiasing = (*itr)["antialiasing"].get<bool>();
        if (itr->contains("speedup"))
            myJ.outP[lc]->speedup = (*itr)["speedup"].get<int>();


        if (itr->contains("maxbounces"))
            myJ.outP[lc]->maxbounces = (*itr)["maxbounces"].get<double>();


        if (itr->contains("probterminate"))
            myJ.outP[lc]->probterminate = (*itr)["probterminate"].get<double>();

        


        if (getElements(itr, "raysperpixel", 2, tempI))
            myJ.outP[lc]->raysperpixel = tempI;

        cout << "Rays per Pixel: [ " << myJ.outP[lc]->raysperpixel[0] << ", " << myJ.outP[lc]->raysperpixel[1] << " ]\n";


        if (getElements(itr, "size", 2, tempI))
            myJ.outP[lc]->size = tempI;
        cout << "Size: [ " << myJ.outP[lc]->size[0] << ", " << myJ.outP[lc]->size[1] << " ]\n";



        for (int i = 0; i < 5; i++)
        {
            if (itr->contains(theStr[i]))
            {
                getElements(itr, theStr[i], 3, temp);
                *myJ.outP[lc]->FloatMemberMap[theStr[i]](*myJ.outP[lc]) = temp;
                cout << theStr[i] << ": ";
                vPrint(theStr[i], myJ.outP[lc]);
            }
        }


        myJ.outP[lc]->fov = (*itr)["fov"].get<double>();
        cout << "Filename: " << myJ.outP[lc]->filename << endl;
        cout << "Camera centre: ";
        vPrint("centre", myJ.outP[lc]);
        cout << "FOV: " << myJ.outP[lc]->fov << endl;

        ++lc;

    }

    cout << "We have: " << lc << " objects!" << endl;
    return true;
}

//Helper function to shorten code
template<typename T>
bool RayTracer::getElements(nlohmann::detail::iter_impl<nlohmann::json> itr, std::string theStr, int arrSize, T& arr)
{
    int i = 0;
    bool received = false;
    for (auto itr2 = (*itr)[theStr].begin(); itr2 != (*itr)[theStr].end(); itr2++) {
        if (i < arrSize)
        {
            arr[i++] = (*itr2).get<double>();
            received = true;
        }
        else
            cout << "Warning: Too many entries in " << theStr << endl;
    }
    return received;
}




int saveppm(std::string file_name, vector<double>& buffer, double dimx, double dimy) {

    ofstream ofs(file_name, ios_base::out | ios_base::binary);
    ofs << "P6" << endl << dimx << ' ' << dimy << endl << "255" << endl;

    for (int j = 0; j < dimy; ++j)
        for (int i = 0; i < dimx; ++i)
            ofs << (char)(255.0 * buffer[3 * j * dimx + 3 * i + 0]) << (char)(255.0 * buffer[3 * j * dimx + 3 * i + 1]) << (char)(255.0 * buffer[3 * j * dimx + 3 * i + 2]);

    ofs.close();

    return 0;
}


void RayTracer::makeThreads(int threadCount, int size, vector<double>* buffer, vector<Geometry*>* objects, vector<Light*>* lights, Camera& cam)
{
    int imgWidth = myJ.outP[0]->size[0];
    int imgHeight = myJ.outP[0]->size[1];

    int* adjWidths = new int[threadCount];
    int* adjHeights = new int[threadCount];
    int* cumulWidths = new int[threadCount];
    int* cumulHeights = new int[threadCount];


    int* heights = new int[threadCount];
    int* widths = new int[threadCount];

    for (int i = 0; i < threadCount; i++)
    {
        adjWidths[i] = 0;

        adjHeights[i] = 0;
        cumulWidths[i] = 0;
        cumulHeights[i] = 0;


        heights[i] = 0;
        widths[i] = 0;
    }

    checkDiv(myJ.outP[0]->size[1], threadCount, heights);
    checkDiv(myJ.outP[0]->size[0], threadCount, widths);


    for (int i = 0; i < threadCount; i++)
    {
        adjWidths[i] = (imgWidth / threadCount) + widths[i];
        adjHeights[i] = (imgHeight / threadCount) + heights[i];
        if (i > 0)
        {
            cumulWidths[i] = cumulWidths[i - 1] + adjWidths[i - 1];
            cumulHeights[i] = cumulHeights[i - 1] + adjHeights[i - 1];
        }
    }


    thread* threads = new thread[threadCount];

    for (int i = 0; i < threadCount; i++)
        threads[i] = thread(startTrace, i, imgWidth, imgHeight, buffer, objects, lights, std::ref(cam), cumulWidths[i], cumulHeights[i], adjWidths[i], adjHeights[i]);


    //auto start = chrono::system_clock::now();
    for (int i = 0; i < threadCount; i++)
        threads[i].join();
    //auto end = chrono::system_clock::now();

    //chrono::duration<double> elapsed_seconds = end - start;
    //cout << "\n\nTime Elapsed: " << elapsed_seconds.count() << endl;



    delete[] threads;
    delete[] heights;
    delete[] widths;
    delete[] adjWidths;
    delete[] adjHeights;
    delete[] cumulWidths;
    delete[] cumulHeights;
}
void startTrace(int id, int totalWidth, int totalHeight, vector<double>* buffer,  vector<Geometry*>* objects, vector<Light*>* lights, Camera& cam, int baseWidth, int baseHeight, int imgWidth, int imgHeight)
{

    Eigen::Vector4f t;
    int closestIndex = 0;
    Eigen::Vector3f color;
    Eigen::Vector4d d;
    double u, v;
    Eigen::Vector4d transform;
    Ray r(Eigen::Vector3d(cam.position[0],cam.position[1],cam.position[2]), Eigen::Vector3d(0, 0, 1));
    bool sceneHit = false;




    for (double i = baseHeight; i < imgHeight + baseHeight; i++)
    {
        cam.i = i;
        for (double j = 0; j < totalWidth; j++)
        {
            color = Eigen::Vector3f(0, 0, 0);

            if (helper::globalillum)
            {
              
                    for (int s = 0; s < helper::raysperpixel[0]; s++)
                    {
                        for (int ss = 0; ss < helper::raysperpixel[0]; ss++)
                        {
                            for (int sss = 0; sss < helper::raysperpixel[1]; sss++)
                            {
                                u = ((j + helper::grid[s][ss][0]) / ((double)(totalWidth) / (double)2.0)) - 1.0;   //x position 
                                v = 1 - (((i + helper::grid[s][ss][1]) / ((double)(totalHeight) / (double)2.0)));  //y position


                                transform = Eigen::Vector4d(u, v, 1, 1);

                                //Matrix multiplication to find the direction vector
                                d = cam.mvpMatrix * transform;

                                r.directionV = Eigen::Vector3d(d.x(), d.y(), d.z());


                                HitContainer hit = loopForHits(r, (*objects), false, NULL);



                                if (hit.g == NULL)
                                    color += cam.bkc;
                                else
                                {
                                    color += vecMult(hit.g->ka * (*lights)[0]->ai, hit.g->ac);
                                    for (int m = 0; m < (*lights).size(); m++)
                                    {
                                        Eigen::Vector3f newColor = (*lights)[m]->getColor(hit.g, cam, (*objects), hit.hitPixel);
                                        color += newColor;
                                    }

                                }
                            }
                            
                        }

                    }
                    color /= (helper::raysperpixel[0] * helper::raysperpixel[0] * helper::raysperpixel[1]);
                


            }
            else if (!helper::antialiasing)
            {
                u = ((j) / ((double)(totalWidth) / (double)2.0)) - 1.0;   //x position 
                v = 1 - (((i) / ((double)(totalHeight) / (double)2.0)));  //y position



                transform = Eigen::Vector4d(u, v, 1, 1);

                //Matrix multiplication to find the direction vector
                d = cam.mvpMatrix * transform;

                r.directionV = Eigen::Vector3d(d.x(), d.y(), d.z());

             
                HitContainer hit = loopForHits(r, (*objects), false, NULL);


                //HitContainer hit = tree.findIntersect(r, NULL, false);


                if (hit.g == NULL)
                    color = cam.bkc;
                else
                {
                    color += vecMult(hit.g->ka * (*lights)[0]->ai, hit.g->ac);
                    for (int m = 0; m < (*lights).size(); m++)
                    {
                        Eigen::Vector3f newColor = (*lights)[m]->getColor(hit.g, cam, (*objects), hit.hitPixel);
                        color += newColor;
                    }
                }
            }
            else if (helper::antialiasing)
            {
                vector<Geometry*> sampleHits;

                vector<Eigen::Vector4d> samplePixel;
                double sampleSize = helper::raysperpixel[0];
                int index1 = 0, index2 = 0;


                for (int s = 0; s < sampleSize; s++)
                {


                    u = ((j + helper::grid[0][s][0]) / ((double)(totalWidth) / (double)2.0)) - 1.0;   //x position 
                    v = 1 - (((i + helper::grid[0][s][1]) / ((double)(totalHeight) / (double)2.0)));  //y position

                    transform = Eigen::Vector4d(u, v, 1, 1);
                    d = cam.mvpMatrix * transform;

                    r.directionV = Eigen::Vector3d(d.x(), d.y(), d.z());

                    //HitContainer hitS = tree.findIntersect(r, NULL, false);

                   
                    HitContainer hit = loopForHits(r, (*objects), false, NULL);
                 

                    

                    if (hit.g == NULL)
                        color += cam.bkc;
                    else
                    {
                        color += vecMult(hit.g->ka * (*lights)[0]->ai, hit.g->ac);
                        for (int m = 0; m < (*lights).size(); m++)
                        {
                            Eigen::Vector3f newColor = (*lights)[m]->getColor(hit.g, cam, (*objects), hit.hitPixel);
                            color += newColor;
                        }


                    }


                    index2++;



                }
                color /= sampleSize;
            }






            color = colorOverflow(color);



            //Write the color to the buffer
            (*buffer)[3 * i * (totalWidth)+3 * j + 0] = color[0];
            (*buffer)[3 * i * (totalWidth)+3 * j + 1] = color[1];
            (*buffer)[3 * i * (totalWidth)+3 * j + 2] = color[2];


            this_thread::yield();
            this_thread::yield();




            
        }
    }
}



string lowercase(string s)
{
    for_each(s.begin(), s.end(), [](char& c)
        {
            c = ::tolower(c);
        });
    return s;

}

string uppercase(string s)
{
    for_each(s.begin(), s.end(), [](char& c)
        {
            c = ::toupper(c);
        });

    return s;

}