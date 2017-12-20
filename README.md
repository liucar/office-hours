# Office Hours
## Contents:

- office_hours_driver.cpp
- office_hours.h
- office_hours.cpp
- topics.txt
- names.txt
- README.txt

## Overview:
A program that models a professor meeting with students during office hours. Students arrive randomly and stand in line to meet with the professor. The professor meets with each student individually while the other students wait. The program uses a priority queue to 
represent students waiting in line, with students having higher question urgency being met with first. A multimap stores the names of the students who have finished meeting with the professor and their respective question topics. 

The driver program will run 100 office hour simulations and generate various analytical reports from the data collected from these office hours. 

## Time Representation:
Office hours run for 1 hour, which is represented by 1 minute in real time. It follows that 1 minute in the simulation is actually
1 second in real time. No new arrivals occur after 1 hour, but the simulation can go over 1 hour if there are 
still students waiting to meet with the professor or a meeting is still in session. The office hour will run until there are no more students waiting in line. 

## Student Representation:
- Every student is randomly assigned a:
	* name from “names.txt” (10 names)
	* topic from “topics.txt” (10 topics) 
	* probability of arrival, which is between [0%, 20%)
	* meeting duration, which is a random integer between 5 to 20 min
	* question urgency between 1 to 3, where 3 is this highest priority and will be met with first
	(However, the student will not interrupt a lower
	   priority meeting already in session.)

## Console Output:

**Every minute (modeled as 1 second in time):**
```
- If a meeting is in session (0 for False, 1 for True)
- Number of students in line
- A timer keeping track of "minutes" elapsed
```

**Alerts:**
```
* New Arrival *
- Student’s service rate (minutes)
- Student’s arrival probability (percentage)
- Student’s urgency (1-3, 3 being top priority)
- Student’s topic (from “topics.txt”)

** New Student Meeting **
- Student’s wait (meeting start time - time of arrival)
- Student’s service rate
- Student’s urgency
- Student’s topic
```

**At the end of every office hour:**
```
-Total number of students served
-Sum of all wait times
-Sum of all meeting times
-Minutes professor stayed past hour
-A dividing line “—————————————“
```

**At the end of 100 simulations:**
```
- Average student wait time
- Average student meet time
- Average professor overtime
```

	   
## Reports Generated:

**“report.txt”** 
- Logs every time a student goes to office hours
and their topic 

- Entry contains: 

			Name, Topic, OH#

- At the end, the 
total number of office hour visits for each student is calculated

**“sorted.txt”** 
- Sorts the report entries by either student name or topic in either ascending or descending alphabetical order. 

- To use sort function:
			  
			  sort(“report.txt”, 
				“studentName” or “questionTopic”, 
				“ascending” or “descending”);

**“search.txt”** 
- Searches the report for all entries containing a desired name or topic. 

- To use search function:
			  
			   search(“report.txt”, 
				  studentName” or “questionTopic”, 
				  desired name or topic as a string);
	   

## Output:
**Console:**
![alt text](https://user-images.githubusercontent.com/34634457/34197209-47c85de0-e51a-11e7-9523-f4af9bcbe7dd.png)

**Report/Averages:**
![alt text](https://user-images.githubusercontent.com/34634457/34197291-a8e22cdc-e51a-11e7-81ff-25f16dbddd0b.png)

**Sorting:**
![alt text](https://user-images.githubusercontent.com/34634457/34197294-ac5d1d04-e51a-11e7-8645-b6d5d9aa4292.png)

**Searching:**
![alt text](https://user-images.githubusercontent.com/34634457/34196115-835be87c-e515-11e7-8125-6a5fcbf1a3e4.png)

