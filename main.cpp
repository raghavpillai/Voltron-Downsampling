// Raghav Pillai
// downsampling by deviation?

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

struct vector3 { // 3d vector struture
    float X, Y, Z;
    inline vector3( void ) {}
    inline vector3( const float x, const float y, const float z ) { X = x; Y = y; Z = z; }
};

std::vector<vector3> ingest( std::string inFileName );
std::vector<vector3> process(std::vector<vector3> inPoints, float deviation);
void exportPoints( std::vector<vector3> points, std::string outputFileName );


int main() {
    // get info
    std::string inName, outName;
    float deviation;

    std::cout << "Enter .csv file name" << std::endl;
    std::cin >> inName;

    std::cout << "Enter output file name (followed by .csv)" << std::endl;
    std::cin >> outName;

    std::cout << "Enter deviation value (0-1)" << std::endl;
    std::cin >> deviation;
    
    std::cout << "\nIngesting data" << std::endl;

    std::vector<vector3> inPoints = ingest(inName); // Ingest
    std::cout << "Ingested " << inPoints.size() << " points" << std::endl;
    
    std::vector<vector3> processedPoints = process(inPoints, deviation); // Process
    std::cout << "Removed " << inPoints.size() - processedPoints.size() << " points with a " << deviation * 100 << "% deviation" << std::endl;

    exportPoints(processedPoints, outName); // Export

    return 0;
}

std::vector<vector3> ingest( std::string inFileName ) {
    std::string line;
    std::vector<vector3> points;
    std::ifstream inFile(inFileName);

    if ( inFile.is_open() ){
        while ( getline (inFile,line) ){
            std::stringstream ss(line);
            std::vector<std::string> v;

            while ( ss.good() ) {
                std::string substr;
                getline(ss, substr, ',');
                v.push_back(substr);
            }

            if ( v.at(0) != "x" ) {
                vector3 curr;
                curr.X = stof(v.at(0));
                curr.Y = stof(v.at(1));
                curr.Z = stof(v.at(2));

                points.push_back(curr);
            }
        }
    }
    return points;
}

std::vector<vector3> process(std::vector<vector3> inPoints, float deviation) {
    std::vector<vector3> newPoints;
    vector3 last(0,0,0);

    for ( auto i: inPoints ){
        if( newPoints.size() == 0 ){
            newPoints.push_back(i);
        }else{
            bool pass = true;

            float MAX_DEV_x = last.X + abs(last.X * deviation);
            float MAX_DEV_y = last.Y + abs(last.Y * deviation);
            float MAX_DEV_z = last.Z + abs(last.Z * deviation);

            float MIN_DEV_x = last.X - abs(last.X * deviation);
            float MIN_DEV_y = last.Y - abs(last.Y * deviation);
            float MIN_DEV_z = last.Z - abs(last.Z * deviation);
            

            if (i.X > MAX_DEV_x || i.Y > MAX_DEV_y || i.Z > MAX_DEV_z ) pass = false;
            if (i.X < MIN_DEV_x || i.Y < MIN_DEV_y || i.Z < MIN_DEV_z ) pass = false;
            

            if (pass == false) newPoints.push_back(i);  

        }
        last = i;
    }
    return newPoints;
}

void exportPoints( std::vector<vector3> points, std::string outputFileName ) {
    std::ofstream outFile(outputFileName); // New file (overwrite if already exists)
    outFile << "x,y,z" << std::endl;

    for ( auto i: points ){
        outFile << i.X << "," << i.Y << "," << i.Z << std::endl;
    }
    outFile.close();

    std::cout << "Exported " << points.size() << " points" << std::endl;
}