<h1 align="center">LineupMaker</h1>
<p align="center"><strong>Optimize lineup for a swim meet to score the most points</strong>
<br/>

<h2>About</h2>
<strong>Allows the user to create multiple lineups and compare them to determine which is the strongest and will score the most points. It is for swim meets (specifically high school meets), allowing you to manually assign some swimmers and automatically assign others.<br/> 
The program is based on a few values that are important for it to work. First, there is a global variable called eN which is set to 8, as this is the amount of individual events at a meet. This would never need to be changed unless the meet had different events. However, that has never happened to me so I just set it as 8. Similarly, the variable prtcp (short for participants) is set to 3, as that is the maximum amount of swimmers you can add to an event. Same with maxEventsAllowed (set to 2)<br/>
The events that I included are listed at the top of the source code in main.cpp. When you run the program, you will be prompted with a number of options that allow you to build a lineup and figure out which is best. Once the lineup is built, it will be given a score, which can be compared to other lineups you build.</strong><br/>


<h2>Goal and requirements</h2>
 DESIGN DOC: https://docs.google.com/document/d/16pJ8yu7457kh0xkWjlEdQsvarU4t2Rwh1TBplXZ-m9U/edit?usp=sharing <br/>
 The goals and non-goals are listed in this doc, but to summarize, my intial goals were to allow the user to create for a swim meet, follow the constraints (listed in the design doc), and also just learn c++ because I did not know if before doing this project.<br/>
 I decided not to include relays because they would be very difficult to implement in the program and are super easy to set up manually anyway because swimmers can swim more of them compared to individual events.<br/>
 
 <h3>Sprint Goal</h3>
<strong></strong><br/>
1. Debug the infinite loop (very annoying) <br/>    
2. Add restrictions including choose swimmer (manually place swimmer in event), prevent swimmer (block from putting in specific event), remove swimmer(entirely prevent from participating. This is last priority because this can be done manually by removing the swimmer from spreadsheet if necesary)<br/>     
3. Remove any bugs that are remaining  <br/>  

<h2>Key learnings</h2>
<strong></strong><br/>
C++ language <br/>
How to debug (I did a LOT of this) <br/>
Planning and defining criteria and constraints for the project <br/><br/>
SPRINT GOALS:<br/>
I completed all of the sprint goals except for prevent swimmer, because I realized that it is a mostly useless feature. I couldn't think of a single situation you would need to do that instead of just removing the swimmer altogether, so I decided not to add that. <br/>
Also, the features that I defined in my sprint goal were not in my initial plan for the project, but they make it significantly better. This happened many times throughout making the project where I changed it from its initial plan to either add a feature or remove and unnecessary one.<br/>


<h2>Running the project</h2>
<strong></strong><br/>
The most important thing for running the project is to have the csv file in the exact same format as the one I provided, and make sure it is named "SwimmerTimes.csv". Unless you want to manually change the code and compile it yourself to change the file name. The first column is the names (which, in the provided file, are just Swimmer 1, Swimmer 2, etc), and the next 8 are the times for each event, correlated to the eventNames[] in main. The program will still work if the events are out of order, but it won't list the correct event name over the swimmers it chose for that event.<br/>
Also, for the spreadsheet, ensure there is some value in the first cell in column J (like -1 in the sample file). This needs to be there because there was some bug that I did not want to fix in the code so I just put that there and it worked.<br/>
Along with the excel file name, you can also modify eN (the amount of events), maxEventsAllowed (most event a swimmer can be in) and prtcp(the amount of participants allowed per event) in the code if you really need to, but make sure those changes are reflected in your csv file.<br/><br/>
To run it, just run main.exe assuming your machine is a windows and can run exe files. Make sure that the spreadsheet with the correct name is in the folder with it.<br/>
Otherwise, you need to compile it yourself on whatever machine you are using so find a compiler that works<br/>

<!-- <h2>Misc.</h2>
Feel free to add anything else you want here :) -->

<!-- <h2>Copyright</h2>
This project is licensed under the terms of the MIT license and protected by Udacity Honor Code and Community Code of Conduct. See <a href="LICENSE.md">license</a> and <a href="LICENSE.DISCLAIMER.md">disclaimer</a>. -->