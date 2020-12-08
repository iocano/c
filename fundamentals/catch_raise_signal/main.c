/*
    Program:
        Asks the user to inputs random words and keeps track of how many words he got right.
        The program will keep running forever, unless user presses Ctrl-C, or user takes more than 2 seconds to input the word.
        When the program ends, it will display the final score.
        Example of alarm, getchar, raise, srandom, sigaction, SIGALRM, sigemptyset, SIGINT, STDIN_FILENO, strcmp, struct sigaction, tcgetattr, TCSANOW, time, tcsetattr, termios
    Compile:
        gcc main.c -o words
    -------------------------------------
    Run:
        ./words
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <termio.h>
#include <stdbool.h>

int score = 0;
struct termios old_term_input;

int catch_signal(int sig, void (*handler)(int))
{
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction(sig, &action, NULL);
}

// SIGALARM routine
void times_up(int sig)
{
    puts("\nTIME'S UP!");
    raise(SIGINT);
}

// SIGINT routine
void end_game(int sig)
{
    // restore terminal input parameters
    tcsetattr(STDIN_FILENO, TCSANOW, &old_term_input);
    printf("Final score: %i\n", score);
    exit(0);
}

char get_character()
{
    char character;
    struct termios new_term_input;

    // Get file descriptor parameters and put on old_term
    tcgetattr(STDIN_FILENO, &old_term_input);

    // Copy settings
    new_term_input = old_term_input;

    /*
       Disable bits
       Canonical input processing mode, terminal input is processed in lines terminated by newline ('\n'), EOF, or EOL characters
ICANON: enables canonical input processing mode
ECHO: enables input characters echo
*/
    new_term_input.c_lflag &= ~(ICANON | ECHO);

    /*
       Set parameters to file descriptor immediately from new_term
        TCSANOW: change occurs immediately.
        TCSADRAIN: change occurs after all output written to fd has been transmitted.
        TCSAFLUSH: change occurs after all output written to fd has been transmitted, and all input that has been received but not read will be discarded before the change is made.
    */
    tcsetattr( STDIN_FILENO, TCSANOW, &new_term_input);

    character = getchar();

    // restore the old settings
    tcsetattr( STDIN_FILENO, TCSANOW, &old_term_input);

    return character;
}

bool check_input_string(const char *word)
{
    char character;
    char *input_word = malloc(strlen(word) + 1);
    char *input_buffer = input_word;
    while (strcmp(word, input_word) != 0)
    {
        // Ignore arrows sequence Esc [ (A|B|C|D)
        if((character = get_character()) == 27 && (character = get_character()) == 91 && (character = get_character()) >= 65 && character <= 68)
        {
            continue;
        }
        // If backspace delete character
        if(character == 0x7f && input_buffer != input_word)
        {
            *--input_buffer = '\0';
            // go one char left
            printf("\b");
            // overwrite the char with whitespace
            printf(" ");
            // go back to "now removed char position"
            printf("\b");
        }
        else
        {
            *input_buffer++ = character;
            printf("%c", character);
        }
    }
    return true;
}

int main()
{
    char *words[] = {
        "the","of","and","a","to","in","is","you","that","it","he","was","for","on","are","as","with","his","they","I",
        "at","be","this","have","from","or","one","had","by","word","but","not","what","all","were","we","when","your","can","said",
        "there","use","an","each","which","she","do","how","their","if","will","up","other","about","out","many","then","them","these","so",
        "some","her","would","make","like","him","into","time","has","look","two","more","write","go","see","number","no","way","could","people",
        "my","than","first","water","been","call","who","oil","its","now","find","long","down","day","did","get","come","made","may","part"
    };
    int word_count = sizeof(words) / sizeof(*words);
    int timer = 2;

    // set signals routines
    catch_signal(SIGALRM, times_up);
    catch_signal(SIGINT, end_game);

    // makes sure get different random numbers each time
    srandom(time(0));

    printf("Write words in less of %i", timer);
    while (1)
    {
        // random number between 0 and 10
        int random_index = random() % word_count;

        printf("\nWrite: %s\n", words[random_index]);

        // alarm routine (times_up) will be raise on 5
        alarm(timer);

        if(check_input_string(words[random_index]))
        {
            score++;
        }
    }
    return 0;
}
