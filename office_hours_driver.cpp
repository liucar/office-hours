/*
 * @author Caroline Liu
 * @date 11/24/17
 * @assignment HW 08
 * @course CSCI 61
 * @file office_hours_driver.cpp
 *
 * A driver program that runs an office hour simulation from the
 * office_hour_sim class 100 times.
 *
 * Generates:
 * -"report.txt": logs every time a student went to an office hour,
 *                their topics, and total number of times each student
 *                went
 * -"sorted.txt": sort function results sorted by studentName or
 *                questionTopic in ascending or descending order
 * -"search.txt": search function results; either search
 *                for a student name to get topics and office
 *                hours attended, or question topic to get the
 *                student names and office hours who asked it
 *
 * Console output:
 * -timer
 * -if meeting is in session
 * -number of students in line
 * -each student wait time/meet time/probability of arrival/urgency
 * -professor overtime per office hour
 * -average student wait time
 * -average meeting time
 * -average overtime for the professor.
 */

#include <iostream>          // For cout
#include "office_hours.h"    // Header file

using namespace std;

int main()
{
    OfficeHourSim office_hour;    // Office hour simulation
    Averager wait_avg;           // Calculates avg wait time
    Averager meet_avg;        // Calculates avg meeting time
    Averager overtime_avg;   // Calculates avg professor overtime

    // Run office hour simulation 100 times
    for (int i=0; i < 100; i++) {

        // Run office hour simulation
        office_hour.run();

        // Add wait time sum, meet time sum, and minutes past hour
        // from each simulation to their respective averager objects
        wait_avg.add_to_sum(office_hour.get_wait_sum());
        meet_avg.add_to_sum(office_hour.get_meet_sum());
        overtime_avg.add_to_sum(office_hour.get_min_past_hr());

        // Keep count of total students served from each simulation
        // for averaging wait times and meet times; for overtime
        // average, increment count by 1 for each iteration
        wait_avg.add_to_count(office_hour.get_total_students());
        meet_avg.add_to_count(office_hour.get_total_students());
        overtime_avg.add_to_count(1);

        // Restart simulation counters to zero
        office_hour.reset_counters();
    }

    office_hour.report();

    // Search category: studentName or questionTopic
    // Search item: pick a name or topic to search for
    office_hour.search("report.txt", "studentName", "Caroline");

    // Sort category: studentName or questionTopic
    // Sort direction: ascending or descending
    office_hour.sort("report.txt", "studentName", "ascending");

    office_hour.num_of_times("report.txt", "names.txt");

    // Print average times
    cout << "\n\nThe average time a student spends waiting: "
         << wait_avg.average() << " minutes";
    cout << "\nThe average time a student spends with the professor: "
         << meet_avg.average() << " minutes";
    cout << "\nThe average time the professor spends past the hour: "
         << overtime_avg.average() << " minutes";

    return 0;
}