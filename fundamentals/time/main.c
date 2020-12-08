#include <stdio.h>
#define __USE_XOPEN // needed by strptime
#include <time.h>

int main()
{

    // get current time
    time_t current_time = time(NULL);
    // convert a time value to a broken-down UTC time
    struct tm *tm_current_time = gmtime(&current_time);
    // convert struct time to string
    char current_time_string[256];
    strftime(current_time_string, sizeof(current_time_string), "%F %T", tm_current_time);
    puts(current_time_string);

    // Converting string to time_t
    char *time_string = "2015-05-15 22:55:13";
    // convert time_string in the broken-down UTC time structure, using the format specified
    struct tm tm_time;
    strptime(time_string, "%F %T", &tm_time);
    // converts a broken-down time structure to time_t structure
    time_t t = mktime(&tm_time);
    // convert back to string
    char new_time_string[256];
    strftime(new_time_string, sizeof(new_time_string), "%F %T", &tm_time);
    puts(new_time_string);



}
