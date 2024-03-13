#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>


/*Events are in this order on the spreadsheet
50fr, 100 fr, 200fr, 500fr, 100bk, 100br, 100fl, 200IM
*/

class Swimmer{
    public:
        std::string name;
        std::array<double, 8> times;
};

class Lineup{
    public:
        std::vector<Swimmer> swimmers;
        static double convertTime(std::string time);

        void importFile(std::ifstream& csvFile);
        


        Lineup();
        


        

};


//convert imported stuff from the csv into usable doubles

void testExcelImport(Lineup lineup);







#endif // main