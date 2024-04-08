#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

/*
GLOBAL VARIABLES EXPLANATION
eN (const)-short for event number, meaning amount of events. Its value is 8. When looping through it I decided to use k as the counter most of the time so if you see k it probably runs the loop 8 times
prtcp (const) -short for participants, meaning allowed number of participants per event. This is 3 but can be modified in the code manually
swimmers (vector<Swimmer>)- stores information about each swimmer after the info is imported from the csv
eventNames (string array)- for reference to see which events are included.
shouldExit - for user to indicate they want to quit the program

swimmers is populated at the beginning of the program when the csv is imported
eventNames and eN are not modified in the program ever. They would need to be changed manually in the code
*/

const int prtcp = 3; 
const int eN = 8; 
bool shouldExit = false;
const int maxEventsAllowed = 2; //maximum events that a swimmer is allowed to be in

class Swimmer{ //stores data about each swimmer, which will be imported from the excel file
    public:
        std::string name;
        std::array<double, eN> times;
        std::array<double, eN> normalTimes; //times relative to fastest time
        int eventsIn; //how many events they are already in (maximum 2)

        bool isTrash; //needed for default constructor, not useful in actual values

        //swimmer constructor
        Swimmer(){
            eventsIn = 0;
            isTrash = false;
        }

        //default swimmer to be replaced (trust me its necessary)
        Swimmer(int tra, int sh){
            name = "trashswimmerplaceholder";
            isTrash = true;
        }
};

std::vector<Swimmer> swimmers; //list of the swimmers and times from the excel
std::array<Swimmer, eN> fastestSwimmers; //list of swimmers with fastest time in each event.
std::vector<std::vector<Swimmer>> sortedSwimmers; //vector storing swimmers sorted for each event
std::string eventNames[] = {"50 Free", "100 Free", "200 Free", "500 Free", "100 Back", "100 Breaststroke", "100 Fly", "200 IM"};
/*
Events are in this order on the spreadsheet:
50fr, 100 fr, 200fr, 500fr, 100bk, 100br, 100fl, 200IM
*/



int helperInt = 0;//makes it easier to specify the event when comparing for sorting
//helper to compare based on time for each event for each swimmer
bool compareByTimes(const Swimmer& a, const Swimmer& b) {
    return a.times[helperInt] < b.times[helperInt];
}

//uses std::sort
void sortParticipants() {
    int i;
    //loop the function 8 times so that it does it for each event.
    int k;
    for(k = 0; k< eN; k++){
        helperInt = k;
        std::vector<Swimmer> temp = {};
        sortedSwimmers.push_back(temp);  
        //add swimmers to sortedSwimmers and remove all swimmers who have a time of -1 (i.e. don't have a time)
        for (i = 0; i < swimmers.size(); i++) {
            if ((swimmers[i].times[k] != -1.0) && (swimmers[i].times[k] != 0)) {
                sortedSwimmers[k].push_back(swimmers[i]);
            }
        }
        //sort
        std::sort(sortedSwimmers[k].begin(), sortedSwimmers[k].end(), compareByTimes);
        
    }
}

//find fastest swimmer in each event and populate fastestSwimmers
void findFastestTimes(){
    Swimmer key;
    for(int k = 0; k< eN; k++){ //loop through each event
        std::vector<Swimmer> temp;
        for(int j = 0; j< swimmers.size(); j++){
            if(swimmers[j].times[k]>=0){
                temp.push_back(swimmers[j]);
            }
        }
        if(temp.size() == 0){
            continue;
        }
        else if (temp.size() == 1){
            fastestSwimmers[k] = (temp[0]);
            continue;
        }
        key = temp[0];
        for(int i = 1; i< temp.size(); i++){
            if(temp[i].times[k]<key.times[k]){
                key = temp[i];
            }
        }
        fastestSwimmers[k] = (key);
    }
}

//normalizes the times to make the more comparable
//divides the swimmer's time by the best time in the set to determine the normalTime
void normalizeTimes() {
    findFastestTimes();
    for (int i = 0; i < swimmers.size(); i++) {
        for (int j = 0; j < eN; j++) {
            //set normalTimes to -1 if time is -1 (doesn't exist)
            if (swimmers[i].times[j] == -1) {
                swimmers[i].normalTimes[j] = -1;
            }
            else {
                //find the normalTime if it exists
                swimmers[i].normalTimes[j] = ((swimmers[i].times[j] / fastestSwimmers[j].times[j]));
            }
        }
    }
}


//Stores data about each individual event, of which there are 8 in a Lineup
class Event{
    public:
        int eventNo; //number 0-7 to distinguish which event it is
        std::array<Swimmer, prtcp> participants;
        Event(int eNo){ //constructor
            eventNo = eNo;
        }

        //adds best swimmers to event
        void populateEvent(){
            //make sure the event is not completely full
            bool temp = false;
            for(int y = 0; y<prtcp; y++){
                if(participants[y].isTrash){
                    temp = true;
                }
            }
            if(!temp){
                return;
            }


            int i = 0;
            while(i<sortedSwimmers[eventNo].size()){ 
                if(sortedSwimmers[eventNo][i].eventsIn<maxEventsAllowed){
                    for(int c = 0; c<prtcp; c++){
                        if(participants[c].isTrash){
                            participants[c] = (sortedSwimmers[eventNo][i]);
                            break;
                        }
                    }
                    
                }
                i++;
            }

            //adds 1 to eventsIn for each swimmer to ensure they are not placed in other events
            for (int k = 0; k < participants.size(); k++) {
                for(int u = 0; u < swimmers.size(); u++){
                    if(swimmers[u].name == participants[k].name){
                        swimmers[u].eventsIn++;
                        break;
                    }
                }
                for(int l = 0; l<sortedSwimmers.size(); l++){
                    for (int j = 0; j < sortedSwimmers[l].size(); j++) {
                        if (sortedSwimmers[l][j].name == participants[k].name) {
                            sortedSwimmers[l][j].eventsIn++;
                            break;
                        }
                    }
                }
            }
        }

        //fills event with default stuff
        fillEventWithTrash(){
            Swimmer trashSwimmer(0, 0);
            int i = 0;
            while(i<prtcp){
                participants[i] = (trashSwimmer);
                i++;
            }
        }
};


//stores the information about an individual lineup to be compared to others
class Lineup{
    public:
        std::string name;
        std::vector<Event> events;
        double score;

        bool isTrash; //not useful in actual lineups, only for placeholders
        Lineup(){
            isTrash = false;
            fillWithTrash();
        }


        //another completely necessary trash constructor
        Lineup(int tr, int ash){
            isTrash = true;
        }
        //creates 8 events
        createLineup(){
            for(int k = 0; k<eN; k++){
                Event event(k);
                events.push_back(event);
                events[k].populateEvent();
            }
        }
        //fills lineup with default swimmers 
        fillWithTrash(){
            for(int k = 0; k<eN; k++){
                Event event(k);
                events.push_back(event);
                events[k].fillEventWithTrash();

            }
        }
        //scores the value of the lineup to be compared with others based on normalTimes of swimmers
        void calculateScore(){
            double ret =0.0;
            for(int i = 0; i< eN; i++){
                for (int j = 0; j< events[j].participants.size(); j++){
                    ret = ret + (events[i].participants[j].normalTimes[i]);
                }
            }
            score = ret;
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

//IMPORT DATA FROM CSV FILE (important)
//convert imported stuff from the csv into usable doubles and store it in the swimmers vector
void importFile(std::ifstream& csvFile){
    std::cout<<"Importing times..."<<std::endl;
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
    //close the csv file 
    csvFile.close();
}


//Loop through and print out the values from the swimmers vector to test if it successfully imported
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


//self explanitory if you don't understand what this does based on the name I don't have anything to say to you
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













/*
User-related functions
*/

//stores the different lineups the user may make
std::vector<Lineup> lineups;

//Main option 1- prints out all of the swimmers' names as well as the number they are related to (helpful for adding contraints to the lineup)
void printSwimmers(){
    std::cout<<""<<std::endl;
    std::cout<<"Swimmers list: "<<std::endl;
    for(int i = 0; i< swimmers.size(); i++){
        std::cout<<(i+1)<<": "<<swimmers[i].name<<std::endl;
    }
}

//Main option 3- prints out the values in all stored lineups
void printLineupsHelper(){
    if(lineups.size()<1){ //checks to see if there are any lineups in the vector
        std::cout<<"No lineups to print"<<std::endl;
        std::cout<<""<<std::endl;
        return;
    }
    for(int i = 0; i<lineups.size(); i++){
        std::cout<<"--------------------------------------"<<std::endl;
        std::cout<<lineups[i].name<<std::endl;
        printEvents(lineups[i]);
        std::cout<<""<<std::endl;
        std::cout<<"Lineup Score: "<<lineups[i].score<<std::endl;
        std::cout<<""<<std::endl;
    }
    std::cout<<"--------------------------------------"<<std::endl;
}


/*
Main option 2 (creating lineups) is big so it gets a big section
*/
//print statements for options menu for building a lineup

//Build Option 1: add a swimmer to an event
Lineup manualAdd(Lineup lineup1){
    Lineup lineup = lineup1;
    std::cout<<"Select a swimmer to add to an event, and choose that swimmer's index"<<std::endl;
    int a;
    int b;
    std::string ans0;
    std::string ans1;
    //get the input for swimmer index
    while(true){
        //input the index of removed swimmer
        std::cin >> ans0; 
        if(ans0 == "q"){
            return lineup1;
        }
        try{
            a = stoi(ans0); //convert inputted string to integer
        }
        catch(const std::invalid_argument&){
            std::cout<<"Invalid Input. Please try again: "<<std::endl;
            continue;
        }

        //check to make sure the index fits the vector size
        if(a<1){
            std::cout<<"Number is too small. Please try again: "<<std::endl;
            continue;
        }
        if(a>swimmers.size()){
            std::cout<<"Number is too big. Please try again: "<<std::endl;
            continue;
        }
        break;
    }

    //check if swimmer has maxed out events
    if(swimmers[a-1].eventsIn>=maxEventsAllowed){
        std::cout<<"Swimmer is already in "<<maxEventsAllowed<<" events"<<std::endl;
        return lineup1;
    }




    std::cout<<"Which event number would you like the assign?"<<std::endl;
    while(true){
        //input the index of swimmer
        std::cin >> ans1; 
        if(ans0 == "q"){
            return lineup1;
        }
        try{
            b = stoi(ans1); //convert inputted string to integer
        }
        catch(const std::invalid_argument&){
            std::cout<<"Invalid Input. Please try again: "<<std::endl;
            continue;
        }

        //check to make sure the index fits the vector size
        if(b<1){
            std::cout<<"Number is too small. Please try again: "<<std::endl;
            continue;
        }
        if(b>eN){
            std::cout<<"Number is too big. Please try again: "<<std::endl;
            continue;
        }
        break;
    }

    //check if swimmer is already in the event
    for(int i = 0; i<prtcp; i++){
        if(swimmers[a-1].name == lineup.events[b-1].participants[i].name){
            std::cout<<"Swimmer is already in this event."<<std::endl;
            return lineup1;
        }
    }


    //assign the swimmer to the event and make sure it is not already full
    for(int i = 0; i<prtcp; i++){
        if(lineup.events[b-1].participants[i].isTrash){
            lineup.events[b-1].participants[i] = swimmers[a-1];//subtract 1 because user list starts at 1 but computer list starts at 0
            //add to eventsIn for the swimmer
            std::string tempName = swimmers[a-1].name;
            swimmers[a-1].eventsIn++;
            //loop through each array in the 2d array sortedSwimmers and add to eventsIn
            for(int k = 0; k< eN; k++){
                for(int i = 0; i<sortedSwimmers[k].size(); i++){
                    if(sortedSwimmers[k][i].name == tempName){
                        sortedSwimmers[k][i].eventsIn++;
                        break;
                    }
                }
            }
            return lineup;
        }
    }
    std::cout<<"Event is already full. Please choose a different event"<<std::endl;
    return lineup1;



}


//Build Option 3: Erase the previously added constraints
void eraseConstraints(){
    //reset eventsIn in swimmers vector
    for(int i = 0; i<swimmers.size(); i++){
        swimmers[i].eventsIn = 0;
    }
    //loop through each array in the 2d array sortedSwimmers and reset the swimmer from each one
    for(int k = 0; k< eN; k++){
        for(int i = 0; i<sortedSwimmers[k].size(); i++){
            sortedSwimmers[k][i].eventsIn = 0;
        }
    }
}

void lineupBuildOptions(){
    std::cout<<""<<std::endl;
    std::cout<<"Options: "<<std::endl;
    std::cout<<"1. Manually add a swimmer to an event"<<std::endl;
    std::cout<<"2. Finish building lineup based on current parameters"<<std::endl;
    std::cout<<"3. Quit lineup building and erase constraints" << std::endl; //done!
    std::cout<<""<<std::endl;
}

//processes input for lineup creation
Lineup creationUserInput(Lineup lineup){
    std::string ans;
    while(true){
        lineupBuildOptions();
        std::cin>> ans;
        if(ans == "1"){
            //manually add swimmer
            lineup = manualAdd(lineup);
        }
        else if (ans == "2"){
            //finish building the lineup
            lineup.createLineup();
            lineup.calculateScore();
            eraseConstraints();
            return lineup;
        }
        else if (ans == "3" || ans == "q"){
            //quit and destroy constraints
            eraseConstraints();
            Lineup lineup(0, 0); //creates a placeholder so it is not added to the lineups vector
            return lineup;

        }
        else{
            std::cout<<"Invalid Input. Please try again";
            std::cout<<""<<std::endl;
        }
    }

}

//Main option 2- called when the user chooses to build a new lineup
Lineup userCreateLineup(){
    Lineup lineup; //lineup constructor

    //name the lineup
    std::cout<<"Name your lineup"<<std::endl;
    std::string ans1;
    std::cin >> ans1;
    lineup.name = ans1;

    //this is where majority of lineup building happens
    lineup = creationUserInput(lineup);
    lineup.name = ans1;
    bool tempBool = lineup.isTrash;
    if(tempBool){
    }
    else{
        lineups.push_back(lineup);
    }
    return lineup;
}

//helper method to actually remove the swimmer
void removeSwimmerHelper(int a){
    std::string tempName = swimmers[a].name;
    swimmers.erase(swimmers.begin()+a);
    //loop through each array in the 2d array sortedSwimmers and remove the swimmer from each one
    for(int k = 0; k< eN; k++){
        for(int i = 0; i<sortedSwimmers[k].size(); i++){
            if(sortedSwimmers[k][i].name == tempName){
                sortedSwimmers[k].erase(sortedSwimmers[k].begin()+i);
                break;
            }
        }
    }
}

//Main option 4- obtain index for swimmer to remove and call the helper method to remove it from swimmers and sortedSwimmers
void removeSwimmer(){
    int a;
    std::string u;
    std::cout<<"Enter the index of the swimmer you would like to remove: ";
    //input the index of removed swimmer
    std::cin >> u; 
    try{
        a = stoi(u); //convert inputted string to integer
    }
    catch(int e){
        std::cout<<"Invalid Input. Please try again"<<std::endl;
        return;
    }

    //check to make sure the index fits the vector size
    if(a<1){
        std::cout<<"Number is too small. Please try again"<<std::endl;
        return;
    }
    if(a>swimmers.size()){
        std::cout<<"Number is too big. Please try again"<<std::endl;
        return;
    }

    //must subtract 1 because inputted number is from a list starting at 1 while the vectors it will be used in start at 0
    removeSwimmerHelper(a-1);
}




















/*
Main User functions
*/
//print user options
void printOptions(){
    std::cout<<"Options"<<std::endl;
    std::cout<<"1. List the swimmers and their indexes"<<std::endl; //done!
    std::cout<<"2. Create new lineup"<<std::endl;
    std::cout<<"3. Print out all lineups"<<std::endl; //done!
    std::cout<<"4. Remove a swimmer (note: this will remove the swimmer from future lineups, but the swimmer will remain if already in a lineup)"<<std::endl; //done!
    std::cout<<"5. Quit (q)"<<std::endl; //done!
    std::cout<< "" <<std::endl;
}

//get option input from user in main menu
void mainUserInput(){
    std::string ans;
    while(true){
        std::cin>> ans;
        if(ans == "1"){
            //print the swimmers
            printSwimmers();
            break;
        }
        else if(ans == "2"){
            //create new lineup
            userCreateLineup();
            break;
        }
        else if (ans == "3"){
            //print out all lineups
            printLineupsHelper();
            break;
        }
        else if (ans == "4"){
            //remove a swimmer
            removeSwimmer();
            break;
        }
        else if (ans == "5" || ans == "q" || ans == "Q"){
            //quit
            shouldExit = true;
            break;
        }
        else{
            std::cout<<"Invalid Input. Please try again: ";
        }
    }
}


//call this method in main to run the program
void run(){
    /*
    set up infrastructure before user interactions
    */
    std::ifstream csvFile("SwimmerTimes.csv"); //MODIFY THIS TO DETERMINE WHICH FILE YOU ARE USING!!!!!!
    //import data from the file
    importFile(csvFile);
    std::cout<<""<<std::endl;

    std::cout<<"Swimmers' stats:"<<std::endl;
    testExcelImport();

    //normalize times and sort swimmers for easier processing
    normalizeTimes();
    sortParticipants();
    /*
    End of Infrastructure stuff
    */


    //MAIN LOOP for user interactions
    while(true){
        printOptions();
        std::cout<<"Choose an option: ";
        mainUserInput();
        if(shouldExit){
            break;
        }
    }

}

//calls run
int main(){
    run();

    // Clear the input buffer before waiting for user input
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    //This part exists to that the window does not close automatically when you are done.
    std::cout << "Press Enter to quit and close the window" << std::endl;
    std::cin.get();
    return 0;
}
