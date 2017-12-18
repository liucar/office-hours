/*
 * @author Caroline Liu
 * @date 11/24/17
 * @assignment HW 08
 * @course CSCI 61
 * @file office_hours.h
 *
 * ClASSES provided: Meeting, Averager, BoolSource, Student,
 * OfficeHourSim
 *
 * CONSTRUCTOR for Meeting class:
 *      Meeting()
 *          Postcondition: min_for_meeting and meet_time_left
 *          initialized to 0
 * MEMBER FUNCTIONS for Meeting class:
 *      void one_minute()
 *          Precondition: Meeting in session
 *          Postcondition: Meeting time left has decremented 1 minute
 *      void start_meeting()
 *          Precondition: Professor is not busy
 *          Postcondition: Meeting starts; meet_time_left is set to
 *          min_for_meeting
 *      void set_meet_time(unsigned int duration)
 *          Postcondition: min_for_meeting set to duration
 *      bool is_busy()
 *          Postcondition: Returns true if there is meeting time left
 *
 *
 * CONSTRUCTOR for Averager class:
 *      Averager()
 *          Postcondition: sum and count are initialized to 0
 * MEMBER FUNCTIONS for Averager class:
 *      void add_to_sum(double value)
 *          Postcondition: value is added to sum total
 *      void add_to_count(int value)
 *          Postcondition: value is added to count total
 *      double average()
 *          Postcondition: Returns the average (sum divided by count)
 *
 *
 * CONSTRUCTOR for BoolSource class:
 *      BoolSource()
 *          Precondition: 0 <= p <= 1
 *          Postcondition: probability is initialized to 0
 * MEMBER FUNCTIONS for BoolSource class:
 *      void set_prob(double p)
 *           Precondition: 0 <= p <= 1
 *          Postcondition: probability is set to p
 *      bool query()
 *          Postcondition: Returns true or false, with the probability
 *          of a true value being p (from set_prob)
 *
 *
 * CONSTRUCTOR for Student class:
 *      Student()
 *          Postcondition: Creates student object that stores the
 *          student's name, topic, arrival probability, service rate,
 *          urgency, and arrival time
 * MEMBER FUNCTIONS for Student class:
 *     int rand_name()
 *          Postcondition: Picks random int to set student name
 *     string rand_topic()
 *          Postcondition: Picks random topic from topics.txt list
 *     int rand_service_rate()
 *          Postcondition: Generates a random int meeting time between
 *          5 and 10 minutes
 *      double rand_arrival_prob()
 *          Postcondition: Generates a random arrival decimal
 *          probability between [0,1)
 *      int rand_urgency()
 *          Postcondition: Generates a random int urgency between
 *          1-3, with 3 being highest priority
 *
 *
 * STRUCT StudentCompare:
 *        operator()
 *          Postcondition: Less-than comparison based on urgency
 *          of question
 *
 *
 * CONSTRUCTOR for OfficeHourSim class:
 *      OfficeHourSim(int min = 60)
 *          Precondition: min is the inteded duration of office hours
 *          which is 60 minutes
 *          Postcondition: Counters initialized to 0, simulation is
 *          ready to run
 * MEMBER FUNCTIONS for OfficeHourSim class:
 *      void check_for_arrivals(int current_time)
 *          Precondition: Simulation is running
 *          Postcondition: Create new Student object, new arrival
 *          alert is printed, arrival_time is set to current_time,
 *          student name and topic are added to the multimap,
 *          and new student is pushed to priority queue
 *      void serve_next(int current_time)
 *          Precondition: Simulation is running and the professor can
 *          start a new meeting
 *          Postcondition: New student meeting alert printed, wait
 *          time is computed, add meet time to meet time sum, pop the
 *          Student from the priority queue, set the meeting
 *          duration, start meeting, and increment total number of
 *          students served
 *      void run()
 *          Postcondition: Office hour simulation that models 1 minute
 *          as 1 actual second in time and also tracks total students
 *          served, wait time sum, meet time sum, and minutes the
 *          professor stayed overtime
 *      void reset_counters()
 *          Precondition: Simulation has already run
 *          Postcondition: Reset all counters to 0
 *      void report()
 *          Precondition: Simulation has run
 *          Postcondition: Generates output report.txt file with
 *          number of times a student appeared in office hours and
 *          their topics
 *      void sort()
 *          Precondition: Report has been generated
 *          Postcondition: Sorts by column and direction in sorted.txt
 *      void search()
 *          Precondition: Report has been generated
 *          Postcondition: Searches for student name or topic and
 *          writes results to search.txt
 *      int get_wait_sum()
 *          Postcondition: Returns wait sum
 *      int get_meet_sum()
 *          Postcondition: Returns meet sum
 *      int get_min_past_hr()
 *          Postcondition: Returns minutes professor stayed past hour
 *      int get_total_students()
 *          Postcondition: Returns total students served
 */

#ifndef OFFICE_HOURS_H
#define OFFICE_HOURS_H
#include <queue>     // For priority queue
#include <map>       // For multimap
#include <string>    // For string

using namespace std;

// MEETING CLASS
class Meeting
{
public:
    Meeting();
    void one_minute();
    void start_meeting();
    void set_meet_time(unsigned int duration);
    bool is_busy();

private:
    unsigned int min_for_meeting; // Minutes for a meeting
    unsigned int meet_time_left;  // Time until prof is not busy
};

// AVERAGER CLASS
class Averager
{
public:
    // Constructor
    Averager();

    // Member functions
    void add_to_sum(double value);
    void add_to_count(int value);
    double average();

private:
    double count;  // The divisor to average the sums
    double sum;    // Sum of all values to be averaged
};

// BOOLSOURCE CLASS
class BoolSource
{
public:
    // Constructor
    BoolSource();

    // Member Functions
    void set_prob(double p);
    bool query();

private:
    double probability;  // Probability of query() returning true
};

// STUDENT CLASS
class Student
{
public:
    // Constructor:
    Student();

    // Member functions
    string rand_name();
    string rand_topic();
    int rand_service_rate();
    double rand_arrival_prob();
    int rand_urgency();

    // Public variables
    string name;           // Random name of student, "StudentX"
    string topic;          // Student topic
    double arrival_prob;   // Random arrival probability
    int service_rate;      // Random duration of meeting per student
    int urgency;           // Question urgency 1-3 (3 is top priority)
    int arrival_time;      // Time the student arrives

    BoolSource arrival;    // Determines if a new student arrives
};

// STUDENT COMPARE STRUCT
struct StudentCompare
{
    bool operator()(Student &student1, Student &student2)
    {
        int first_value = student1.urgency;
        int sec_value = student2.urgency;
        return first_value < sec_value;
    }
};

// OFFICE HOUR SIMULATION CLASS
class OfficeHourSim
{
public:
    // Constructor:
    OfficeHourSim(int min = 60);

    // Member Functions
    void check_for_arrivals(int current_time);
    void serve_next(int current_time);
    void run();
    void reset_counters();
    void report();
    void num_of_times(string file, string names);
    void sort(string file, string column, string direction);
    void search(string file, string category, string search_item);

    int get_wait_sum() {return wait_time_sum;}
    int get_meet_sum() {return meet_time_sum;}
    int get_min_past_hr() {return min_past_hr;}
    int get_total_students() {return total_students;}

private:

    // Multimap with student names and their topics
    multimap<string, string> student_topics;
    multimap<string, string>::iterator it;

    // Priority queue for Student objects, uses StudentCompare
    priority_queue<Student, vector<Student>,
            StudentCompare> students;

    int intended_stay;     // Intended duration of office hours
    int total_students;    // Total students who went to office hours
    int wait_time_sum;     // Total sum of all wait times
    int meet_time_sum;     // Total sum of all meeting durations
    int min_past_hr;       // Total min past hour professor stayed
    int arrival_count;     // Keeps count of arrivals
    int max_arrivals;      // Maximum num of arrivals from all iterations
    int OH_count;          // Office hour count

    Meeting prof_meeting;  // Determines if professor is in a meeting
};

#endif