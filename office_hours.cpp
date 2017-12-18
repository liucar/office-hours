/*
 * @author Caroline Liu
 * @date 11/24/17
 * @assignment HW 08
 * @course CSCI 61
 * @file office_hours.cpp
 *
 * ClASSES implemented: Meeting, Averager, BoolSource, Student,
 * OfficeHourSim
 *
 * INVARIANT for Meeting class:
 *      1) min_for_meeting variable is the duration of the meeting
 *      2) meet_time_left variable is 0 if meeting is over; otherwise
 *         it's the number of min left until professor is free
 *
 * INVARIANT for Averager class:
 *      1) count variable is the divisor to average the sums
 *      2) sum variable is the sum of all values to be averaged
 *
 * INVARIANT for BoolSource class:
 *      1) probability variable is the probability of query()
 *         returning true
 *
 * INVARIANT for Student class:
 *      1) name variable stores the student's name "StudentX"
 *      2) topic variable stores the student's topic
 *      3) arrival_prob variable is the random arrival probability
 *      4) service_rate variable is the random duration of meeting
 *      5) urgency variable is the question urgency (1-3)
 *      6) arrival time variable is the time student arrives
 *      7) arrival bool_source object determines if a new student
 *         arrives
 *
 * INVARIANT for OfficeHourSim class:
 *      1) Multimap keeps track of student names and their topics
 *      2) Priority queue models students waiting in line and stores
 *         Student objects
 *      3) intended_stay variable is the intended duration of office
 *         hours
 *      4) total_students variable keeps track of total students who
 *         went to office hours
 *      5) wait_time_sum variable is the total sum of all wait times
 *      6) meet_time_sum variable is the total sum of all meet times
 *      7) min_past_hr variable is the total min past the hour the
 *         professor stayed
 *      8) arrival_count keeps track of total arrivals
 *      9) max_arrivals keeps track of maximum arrivals out of all
 *         iterations
 *      10) prof_meeting meeting object determines if professor is in
 *         a meeting
 *
 */

#include "office_hours.h" // Header File
#include <iostream>       // For cout
#include <fstream>        // For ofstream, ifstream
#include <string>         // For append
#include <map>            // For multimap, count, equal_range
#include <queue>          // For priority queue, push, pop, top
#include <cassert>        // For assert
#include <thread>         // For this_thread::sleep_for
#include <chrono>         // For chrono::seconds
#include <cstdlib>        // For rand, RAND_MAX
#include <random>         // For random_device, default_random_engine,
                          // and uniform_real_distribution

using namespace std;

// Meeting CLASS IMPLEMENTATION
Meeting::Meeting()
{
    min_for_meeting = 0;
    meet_time_left = 0;
}

void Meeting::one_minute()
{
    if(is_busy())
        meet_time_left--;
}

void Meeting::start_meeting()
{
    assert(!is_busy());
    meet_time_left = min_for_meeting;
}

void Meeting::set_meet_time(unsigned int duration)
{
    min_for_meeting = duration;
}

bool Meeting::is_busy()
{
    return (meet_time_left > 0);
}




// Averager CLASS IMPLEMENTATION
Averager::Averager()
{
    count = 0;
    sum = 0;
}

void Averager::add_to_sum(double value)
{
    sum += value;
}

void Averager::add_to_count(int value)
{
    count += value;
}

double Averager::average()
{
    return (sum / count);
}




// BoolSource CLASS IMPLEMENTATION
BoolSource::BoolSource()
{
    probability = 0;
}

void BoolSource::set_prob(double p)
{
    probability = p;
}

bool BoolSource::query()
{
    return (rand() < probability * RAND_MAX);
}




// Student CLASS IMPLEMENTATION
Student::Student()
{
    name = "";
    topic = rand_topic();
    arrival_prob = rand_arrival_prob();
    service_rate = rand_service_rate();
    urgency = rand_urgency();
    arrival_time = 0;
    arrival.set_prob(arrival_prob);
}

string Student::rand_name()
{
    const int NAME_LIST_LENGTH = 10;   // Length of names list

    ifstream inFile;
    string student_name;    // Name of student being read in
    random_device seed;

    // Generate random number from 0 to topic list length - 1
    default_random_engine generator(seed());
    uniform_int_distribution<int> topic_distribution(0, NAME_LIST_LENGTH - 1);
    int topic_num = topic_distribution(generator);

    inFile.open("names.txt");
    if (!inFile) { cout << "Error: Could not find file."; }
    else {

        // Skip previously generated random number of lines then
        // get topic line
        for(int i=0; i < topic_num; i++ )
        {
            inFile.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        getline(inFile, student_name);

        inFile.close();
    }

    return student_name;
}

string Student::rand_topic()
{
    const int TOPIC_LIST_LENGTH = 10;   // Length of topics list

    ifstream inFile;
    string topic_name;    // Name of topic being read in
    random_device seed;

    // Generate random number from 0 to topic list length - 1
    default_random_engine generator(seed());
    uniform_int_distribution<int> topic_distribution(0, TOPIC_LIST_LENGTH - 1);
    int topic_num = topic_distribution(generator);

    inFile.open("topics.txt");
    if (!inFile) { cout << "Error: Could not find file."; }
    else {

        // Skip previously generated random number of lines then
        // get topic line
        for(int i=0; i < topic_num; i++ )
        {
            inFile.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        getline(inFile, topic_name);

        inFile.close();
    }

    return topic_name;
}

int Student::rand_service_rate() {

    random_device seed;
    default_random_engine generator(seed());

    // Meet times: random integer between 5 to 20 min
    uniform_int_distribution<int> meet_distribution(5, 20);

    // Meeting duration
    double meeting_time = meet_distribution(generator);

    return meeting_time;

}

double Student::rand_arrival_prob() {

    random_device seed;
    default_random_engine generator(seed());

    // Random decimal between interval [0, .2)
    uniform_real_distribution<double> arrival_distribution(0, .2);

    // Generate random probability of arrival
    double arrival_prob = arrival_distribution(generator);

    return arrival_prob;
}

int Student::rand_urgency() {

    random_device seed;
    default_random_engine generator(seed());

    // Random integer between 1 to 3; 3 being the highest priority
    uniform_int_distribution<int> meet_distribution(1, 3);

    // Generate urgency of question
    double urgency = meet_distribution(generator);

    return urgency;

}




// OfficeHourSim CLASS IMPLEMENTATION
OfficeHourSim::OfficeHourSim(int min)
{
    intended_stay = min;
    arrival_count = 1;
    wait_time_sum = 0;
    meet_time_sum = 0;
    min_past_hr = 0;
    total_students = 0;
    max_arrivals = 0;
    OH_count = 1;
}

void OfficeHourSim::check_for_arrivals(int current_time)
{
    Student new_arrival;

    // If a new student arrives
    if (new_arrival.arrival.query()) {

        // Set the student's name randomly
        new_arrival.name.append(new_arrival.rand_name());

        // Print New Arrival alert
        cout << "\n* New Arrival *";
        cout << "\n\t" << new_arrival.name << "'s service rate: "
             << new_arrival.service_rate
             << " min";
        cout << "\n\t" << new_arrival.name << "'s arrival probability: "
             << (new_arrival.arrival_prob * 100) << "%";
        cout << "\n\t" << new_arrival.name << "'s urgency: "
             << new_arrival.urgency;
        cout << "\n\t" << new_arrival.name << "'s topic: "
             << new_arrival.topic;

        // Set arrival time of new arrival
        new_arrival.arrival_time = current_time;

        // Push new_arrival to priority queue
        students.push(new_arrival);

        // Add student's name and topic to multimap
        student_topics.insert(make_pair(new_arrival.name,
                                        new_arrival.topic + ", OH" +
                                        to_string(OH_count)));

        // Increment num of arrivals
        arrival_count++;

    }
}

void OfficeHourSim::serve_next(int current_time)
{
    int meet_time;          // Meeting duration
    int wait_time;          // Amount of time student waited

    cout << "\n** New Student Meeting **";

    // Set meet_time to service rate of student in the front of line
    meet_time = students.top().service_rate;

    // Compute wait time and add to wait time sum
    wait_time = current_time - students.top().arrival_time;
    wait_time_sum += wait_time;
    cout << "\n\t" << students.top().name
         << "'s wait: " << wait_time << " min";

    // Add meet time to meet time sum
    meet_time_sum += meet_time;
    cout << "\n\t" << students.top().name
         << "'s service rate: " << meet_time << " min";

    // Print question urgency and topic
    cout << "\n\t" << students.top().name << "'s urgency: "
         << students.top().urgency;
    cout << "\n\t" << students.top().name << "'s topic: "
         << students.top().topic;

    // Pop the person in front of the line
    students.pop();

    // Set the meeting duration
    prof_meeting.set_meet_time(meet_time);

    // Professor starts the meeting with student
    prof_meeting.start_meeting();

    // Increment total number of students served
    total_students++;
}

void OfficeHourSim::run()
{
    int current_min;  // Tracks current time

    // CASE 1: Office hours go for the intended duration ( 1 hour ).
    // Students can arrive within this duration.
    for (current_min = 1; current_min <= intended_stay; current_min++)
    {
        cout << "\n\nMeeting in session? "
             << prof_meeting.is_busy();

        // Check for new arrivals during minute
        check_for_arrivals(current_min);

        // If professor is not busy and there are students in the
        // queue, then serve the next student
        if ((!prof_meeting.is_busy()) && (!students.empty()))
        {
            serve_next(current_min);
        }

        cout << "\nStudents in line: " << students.size();
        cout << "\nTimer: " << current_min;

        // Decrement meeting time left by 1 minute
        prof_meeting.one_minute();

        // Model 1 minute as 1 actual second in time
        this_thread::sleep_for(chrono::seconds(1));
    }


    // CASE 2: It's past the hour, and the student queue is not empty.
    // New students can no longer arrive.
    // The professor will stay until there are no students waiting
    while (!students.empty())
    {
        cout << "\n\nMeeting in session (after hour)? "
             << prof_meeting.is_busy();

        // If professor is not busy, then serve the next student
        if (!prof_meeting.is_busy())
        {
            serve_next(current_min);
        }

        cout << "\nStudents in line: " << students.size();
        cout << "\nTimer: " << current_min;

        // Increment current_minute
        current_min++;

        // Decrement meeting time left by 1 minute
        prof_meeting.one_minute();

        // Track minutes the professor stays overtime
        min_past_hr++;

        // Model 1 minute as 1 actual second in time
        this_thread::sleep_for(chrono::seconds(1));
    }


    // CASE 3: It's past the hour, and the professor is meeting with
    // the last student.
    while (prof_meeting.is_busy())
    {
        cout << "\n\nMeeting in session (after hour)? "
             << prof_meeting.is_busy();
        cout << "\nStudents in line: " << students.size();
        cout << "\nTimer: " << current_min;

        // Increment current_minute
        current_min++;

        // Decrement meeting time left by 1 minute
        prof_meeting.one_minute();

        // Track minutes past hour professor stays overtime
        min_past_hr++;

        // Model 1 minute as 1 actual second in time
        this_thread::sleep_for(chrono::seconds(1));
    }

    // Check if total arrival count in current iteration exceeds
    // maximum number of arrivals from past iterations
    if (arrival_count > max_arrivals) {max_arrivals = arrival_count;}

    cout << "\n\nTotal Students: " << total_students;
    cout << "\nWait time sum: " << wait_time_sum;
    cout << "\nMeet time sum: " << meet_time_sum;
    cout << "\nMinutes professor stayed past hour: " << min_past_hr;
    cout << "\n-----------------------------------------------------";

    ++OH_count;

}

void OfficeHourSim::reset_counters()
{
    arrival_count = 1;
    wait_time_sum = 0;
    meet_time_sum = 0;
    min_past_hr = 0;
    total_students = 0;
}

void OfficeHourSim::report()
{
    ofstream outFile;
    outFile.open( "report.txt" );
    bool lastLine = true;

    // Write to outFile student name and topic
    for( it = student_topics.begin(); it != student_topics.end(); it++ )
    {
        if(!lastLine) {
            outFile << endl;
        }
        outFile << it -> first << ", " << it -> second;
        lastLine = false;
    }

    outFile.close();
}

void OfficeHourSim::num_of_times(string file, string names) {
    const int NAME_LIST_LENGTH = 10;
    fstream outFile(file, ios::app);
    ifstream inFile(names);
    string line;
    bool lastLine=true;

    outFile << "\n-----------------------\n";
    outFile << "\nNUMBER OF OFFICE HOUR VISITS\n\n";
    getline(inFile,line);
    for(int i=0; i < NAME_LIST_LENGTH; i++) {
        if (!lastLine) {
            getline(inFile,line);
            outFile << endl;
        }
        outFile << line << ": " << student_topics.count(line);
        lastLine = false;
    }
}

void OfficeHourSim::sort(string file, string column, string direction) {
    ifstream inFile(file);
    ofstream sorted("sorted.txt");
    int num_of_lines = 0;
    int array_size;
    string line;
    bool lastLine = true;

    // Get count of number of lines
    while (getline(inFile, line)) {
        ++num_of_lines;
    }

    inFile.close();
    inFile.open(file);

    array_size = num_of_lines;

    string names[array_size];
    string topics[array_size];

    // Store names and topics in arrays
    for (int i =0; i< array_size; i++) {
        getline(inFile, names[i], ',');
        getline(inFile, topics[i]);

    }

    // If sorting by name
    if (column == "studentName")
    {
        if (direction == "ascending")
        {
            for (int i=0; i < array_size; i++)
            {
                if(!lastLine)
                {
                    sorted << endl;
                }
                sorted << names[i] << topics[i];
                lastLine = false;
            }
        } else if (direction == "descending")
        {
            for (int i=array_size - 1; i >= 0; i--)
            {
                if(!lastLine)
                {
                    sorted << endl;
                }
                sorted << names[i] << topics[i];
                lastLine = false;
            }
        }

    // If sorting by topic
    } else if (column == "questionTopic" )
    {
        // Use selection sort to sort
        int i, j, max_index;
        string largest;

        for (int i = array_size-1; i > 0; i--)
        {
            largest = topics[0];
            max_index = 0;

            for (j = 1; j <= i; j++)
            {
                if (strcmp(topics[j].c_str(),largest.c_str()) > 0)
                {
                    largest = topics[j];
                    max_index = j;
                }
            }
            swap(topics[i], topics[max_index]);
            swap(names[i], names[max_index]);
        }

        if (direction == "ascending")
        {
             // Write to outfile in ascending order
            for(int i=0; i < array_size; i++)
            {
                if(!lastLine)
                {
                    sorted << endl;
                }
                sorted << topics[i] << " " << names[i];
                lastLine = false;
            }

        } else if (direction == "descending") {
            // Write to outfile in descending order
            for(int i=array_size-1; i >= 0; i--)
            {
                if(!lastLine)
                {
                    sorted << endl;
                }
                sorted << topics[i] << " " << names[i];
                lastLine = false;
            }
        }
    }

    sorted.close();
    inFile.close();
}

void OfficeHourSim::search(string file, string category, string search_item)
{
    ifstream inFile(file);
    ofstream results("search.txt");
    int num_of_lines = 0;
    int array_size;
    int entry_count = 0;
    string line;
    bool lastLine = true;

    // Get count of number of lines
    while (getline(inFile, line)) {
        ++num_of_lines;
    }

    inFile.close();
    inFile.open(file);

    array_size = num_of_lines;

    string names[array_size];
    string topics[array_size];

    // Store names and topics in arrays
    for (int i =0; i< array_size; i++) {
        getline(inFile, names[i], ',');
        getline(inFile, topics[i]);

    }

    // If searching by name, serial search
    if (category == "studentName") {
        results << "Searching by studentName for "
                << search_item <<":\n\n";
        for(int i=0; i < array_size; i++) {
            if (search_item == names[i]) {
                if(!lastLine)
                {
                    results << endl;
                }
                results << names[i] << " " << topics[i];
                lastLine=false;
                entry_count++;
            }
        }

        if (entry_count > 0) {
            results << "\n\n" << entry_count << " entries found.";
        } else {
            results << "Item not found.";
        }

    // If searching by topic, serial search
    } else if (category == "questionTopic" ) {
        results << "Searching by questionTopic for "
                << search_item <<":\n\n";
        for(int i=0; i < array_size; i++) {
            if (topics[i].find(search_item) != string::npos) {
                if(!lastLine)
                {
                    results << endl;
                }
                results << topics[i] << " " << names[i];
                lastLine=false;
                entry_count++;
            }
        }

        if (entry_count > 0) {
            results << "\n\n" << entry_count << " entries found.";
        } else {
            results << "Item not found.";
        }
    }

    results.close();
    inFile.close();
}