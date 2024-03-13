#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

/*
GLOBAL VARIABLES EXPLANATION
eN (const)-short for event number, meaning amount of events. Its value is 8
swimmers (vector<Swimmer>)- stores information about each swimmer after the info is imported from the csv
eventNames (string array)- for reference to see which events are included.

swimmers is populated at the beginning of the program when the csv is imported
eventNames and eN are not modified in the program ever. They would need to be changed manually in the code
*/

const int eN = 8; 

class Swimmer{ //stores data about each swimmer, which will be imported from the excel file
    public:
        std::string name;
        std::array<double, eN> times;
        std::array<double, eN> normalTimes;
        int eventsIn;

        Swimmer(){
            eventsIn = 0;
        }
};

std::vector<Swimmer> swimmers; //list of the swimmers and times from the excel
std::string eventNames[] = {"50 Free", "100 Free", "200 Free", "500 Free", "100 Back", "100 Breaststroke", "100 Fly", "200 IM"};
/*
Events are in this order on the spreadsheet:
50fr, 100 fr, 200fr, 500fr, 100bk, 100br, 100fl, 200IM
*/

std::vector<std::vector<Swimmer>> sortedSwimmers;
void sortParticipants() {
    int i;
    std::cout<<"111111111111111"<<std::endl;
    for(int k = 0; k< eN; k++){
        std::cout<<"222222222222222222"<<std::endl;
        std::vector<Swimmer> temp;
        for (i = 0; i < swimmers.size(); i++) {
            std::cout<<"33333333333333333333333"<<std::endl;
            if ((swimmers[i].times[k] != -1.0) && (swimmers[i].times[k] != 0)) {
                std::cout<<"444444444444444444"<<std::endl;
                temp.push_back(swimmers[i]);
            }
        }
        std::cout<<"5555555555555555555555"<<std::endl;
        while (temp.size() > 0) {
            int keyInd = 0;
            Swimmer key = temp[keyInd];
            std::cout<<"666666666666666666"<<std::endl;

            for (i = 0; i < temp.size()-1; i++) {
                std::cout<<"777777777777777777777"<<std::endl;
                if (temp.size() == 2) {
                    std::cout<<"8888888888888888888888"<<std::endl;
                    // Code for handling two elements
                    if (key.normalTimes[k] < temp[1].normalTimes[k]) {
                        std::cout<<"999999999999999999999999"<<std::endl;
                        sortedSwimmers[k].push_back(key);
                        sortedSwimmers[k].push_back(temp[1]);
                    } 
                    else {
                        std::cout<<"0000000000000000000"<<std::endl;
                        sortedSwimmers[k].push_back(temp[1]);
                        sortedSwimmers[k].push_back(key);
                    }
                    temp.clear();
                    break;
                } 
                else {
                    std::cout<<"#####################"<<std::endl;
                    // Compare based on normalized times
                    if (temp[i].normalTimes[k] < key.normalTimes[k]) {
                        std::cout<<"$$$$$$$$$$$$$$$$"<<std::endl;
                        key = temp[i];
                        keyInd = i;
                    }
                }
            }
            std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<std::endl;
            sortedSwimmers[k].push_back(key);
            temp.erase(temp.begin() + keyInd);
        }
    }
    
};


class Event{
    public:
        int eventNo;
        std::vector<Swimmer> orderedSwimmers;
        std::vector<Swimmer> participants;
        Event(int eNo){
            eventNo = eNo;
            sortParticipants();
        }
        populateEvent(){
            int i = 0;
            while(participants.size()<3 && i<orderedSwimmers.size()){
                if(orderedSwimmers[i].eventsIn<2){
                    participants.push_back(orderedSwimmers[i]);
                }
                else{
                }
                i++;
            }
            for (int k = 0; k < participants.size(); k++) {
                for (int j = 0; j < swimmers.size(); j++) {
                    if (swimmers[j].name == participants[k].name) {
                        swimmers[j].eventsIn++;
                        break;
                    }
                }
            }
        }
        createBaselineEvent(){
            for(int j = 0; j<3; j++){
                participants.push_back(orderedSwimmers[j]);
                
            }
        }
};


//stores the information about an individual lineup to be compared to others
class Lineup{
    public:
        std::vector<Event> events;
        double score;
        Lineup(){
        }
        createLineup(){
            for(int i = 0; i<eN; i++){
                Event event(i);
                events.push_back(event);
                events[i].populateEvent();
            }
        }
        createBaseLineup(){
            int i = 0;
            while(i<eN){
                Event event(i);
                events.push_back(event);
                events[i].createBaselineEvent();
                i++;
            }
        }
        int calculateScore(){
            int ret;
            for(int i = 0; i< events.size(); i++){
                for (int j = 0; j< events[j].participants.size(); j++){
                    ret += events[i].participants[j].normalTimes[i];
                }
            }
            std::cout<<ret<<std::endl;
            return ret;
        }
};


//Convert Strings imported from excel to usable doubles
static double convertTime(std::string time){ 
    //find the position of x (if it exists) and remove it
    size_t xPos = time.find("x");
    if (xPos != std::string::npos){
        time = time.substr(1);
    }

    //find the colon (if it exists) and convert it to a regular double
    size_t pos = time.find(":");
    if(pos != std::string::npos){
        std::string mins = time.substr(0, pos);
        std::string secs = time.substr(pos+1);
        double x;
        double y;
        try{
            x = std::stod(mins);
            x *= 60;
            y = std::stod(secs);
            return x+y;
        }
        catch(const std::exception& e){
            std::cerr << "Erorr: " << e.what() << std::endl;
            return -1.0;
        }
    }
    else{
        //if there is not colon, just return the number
        try{
            return std::stod(time);
        }
        //catch the error if it breaks. Also, if the cell is blank, return -1.
        catch(const std::exception& e){
            return -1.0;
        }
    }
}


//convert imported stuff from the csv into usable doubles and store it in the swimmers vector
void importFile(std::ifstream& csvFile){
    //make sure the file is open
    if (!csvFile.is_open()) {
        std::cerr << "Error: Unable to open the CSV file :(" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(csvFile, line)) {
    // Remove any carriage return ('\r') characters
    line.erase(std::remove_if(line.begin(), line.end(), [](char c) { return c == '\r'; }), line.end());

    Swimmer swimmer;
    std::istringstream iss(line);

    // Read the name
    if (!(std::getline(iss, swimmer.name, ','))) {
        std::cerr << "Error: Unable to extract name from the line." << std::endl;
        return;
    }

    // Read and convert times
    for (int j = 0; j < swimmer.times.size(); ++j) {
        std::string time;
        if (!(std::getline(iss, time, ','))) {
            std::cerr << "Error: Unable to extract times from the line." << std::endl;
            return;
        }
        swimmer.times[j] = convertTime(time);
    }

    swimmers.push_back(swimmer);
    }
    csvFile.close();
}


//Loop through and print out the values from the swimmers vector to test if it successfull imported
void testExcelImport(){
    for(int i = 0; i<swimmers.size(); i++){
        std::cout << swimmers[i].name;
        for(int j = 0; j<swimmers[i].times.size(); j++){
            std::cout << " / ";
            std::cout << swimmers[i].times[j];
        }
        std::cout << "" << std::endl;
    }
    std::cout << " " <<std::endl;
    std::cout << " " <<std::endl;
    std::cout << " " <<std::endl;
    std::cout << "----------------------------------------------- " <<std::endl;
    std::cout << " " <<std::endl;
}

//Print our the contents of each event in the Lineup class
void printEvents(Lineup lineup){
    for(int i = 0; i<eN; i++){
        std::cout << " "<< std::endl;
        std::cout << "Event " << (lineup.events[i].eventNo)+1 << ": " << eventNames[i] <<std::endl;
        std::cout << lineup.events[i].participants[0].name << ": " << lineup.events[i].participants[0].times[lineup.events[i].eventNo] << std::endl;
        std::cout << lineup.events[i].participants[1].name << ": " << lineup.events[i].participants[1].times[lineup.events[i].eventNo] << std::endl;
        std::cout << lineup.events[i].participants[2].name << ": " << lineup.events[i].participants[2].times[lineup.events[i].eventNo] << std::endl;
    }
    std::cout << " " << std::endl;
    std::cout << " " << std::endl;
}

//normalizes the times to make the more comparable
void normalizeTimes(Lineup lineup) {
    for (int i = 0; i < swimmers.size(); i++) {
        for (int j = 0; j < swimmers[i].normalTimes.size(); j++) {
            if (swimmers[i].times[j] == -1) {
                swimmers[i].normalTimes[j] = -1;
            } 
            else {
                std::cout << "swimmers[i].times[j]: " << swimmers[i].times[j] << std::endl;
                std::cout << "lineup.events[j].orderedSwimmers[0].times[j]: " << lineup.events[j].orderedSwimmers[0].times[j] << std::endl;

                swimmers[i].normalTimes[j] = ((swimmers[i].times[j] / lineup.events[j].orderedSwimmers[0].times[j]));

                std::cout << "Result: " << swimmers[i].normalTimes[j] << std::endl;
            }
        }
    }
}



//Reset the events each swimmer is in
void resetSwimmers(){
    for(int i = 0; i<swimmers.size(); i++){
        swimmers[i].eventsIn = 0;
    }
}

void printNormalTimes(){
    for(Swimmer swimmer : swimmers){
        int i = 0;
        std::cout<<swimmer.name<<std::endl;
        while(i < eN){
            std::cout<< "Event " << i<< "-" << eventNames[i]<<": "<<swimmer.normalTimes[i]<< std::endl;
            i++;
        }
        std::cout<<std::endl;
    }
}


Lineup compareLineups(Lineup base, Lineup L1, Lineup L2){

}


int main(){
    std::ifstream csvFile("Top_times_4_17_22.csv");
    importFile(csvFile);
    testExcelImport();
    sortParticipants();
    Lineup baseLine;
    baseLine.createBaseLineup();
    resetSwimmers();
    Lineup lineup;
    lineup.createLineup();
    normalizeTimes(lineup);
    printNormalTimes();
    std::cout<<"Score: ";
    lineup.calculateScore();
    printEvents(baseLine);
    printEvents(lineup);
    
    std::cout << "Press Enter to quit" << std::endl;
    std::cin.get();
    return 0;
}
