#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#define STRLEN  50

struct time
{
    int days;
    int months;
    int years;
};
int main (void)
{
    void outputResult (struct tm P5Time);
    void createBatchfiles (void);
    struct tm DesiredDate;

    DesiredDate = (struct tm) {.tm_mday = 4, .tm_mon = 4, .tm_year = 117};  //Defining the desired date. Could also be done by the user. (By having outputResult take an argument)
    outputResult (DesiredDate);

    #ifndef BATCREATED
        #define BATCREATED
        createBatchfiles();
    #endif // BATCREATED

    return 0;
}

struct time howLongUntil (struct tm *now, struct tm *future)     //Calculates how much time separates two dates.
{
    struct time elapsedTime;
    int daysPerMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool moreThanOneYear = false, sameMonth = false;

    if ((future->tm_year < now->tm_year) || (future->tm_year == now->tm_year && future->tm_mon < now->tm_mon) ||           //Error and exact date check.
    (future->tm_year == now->tm_year && future->tm_mon == now->tm_mon && future->tm_mday < now->tm_mday)){
        printf ("An error has ocurred or the desired date precedes the current date.\n");
        exit (EXIT_SUCCESS);
    }
    if ((future->tm_year < now->tm_year) || (future->tm_year == now->tm_year && future->tm_mon < now->tm_mon) ||
    (future->tm_year == now->tm_year && future->tm_mon == now->tm_mon && future->tm_mday == now->tm_mday)){
        printf ("The day has come! Persona 5! Yay!\n");
        exit (EXIT_SUCCESS);
    }


    if (future->tm_year > now->tm_year){                            //Calculate the difference in years.
        elapsedTime.years = future->tm_year - now->tm_year;
        moreThanOneYear = true;
    }
    else if (future->tm_year == now->tm_year)
        elapsedTime.years = 0;

    if (future->tm_mon > now->tm_mon)                               //Calculate the difference in months.
        elapsedTime.months = future->tm_mon - now->tm_mon;

    else if (future->tm_mon < now->tm_mon && moreThanOneYear == true){
        elapsedTime.years -= 1;
        elapsedTime.months = 12 - (now->tm_mon + 1) + (future->tm_mon + 1);
    }
    else if (future->tm_mon == now->tm_mon){
        elapsedTime.months = 0;
    }

    if (future->tm_mday > now->tm_mday)                             //Calculate the difference in days.
        elapsedTime.days = future->tm_mday - now->tm_mday;

    else if (future->tm_mday < now->tm_mday){
        elapsedTime.months -= 1;
        elapsedTime.days = daysPerMonth[now->tm_mon] - now->tm_mday + future->tm_mday;
    }
    else
        elapsedTime.days = 0;

    return elapsedTime;
}

void outputResult (struct tm P5Time)
{
    FILE *source, *potato;
    source = fopen ("P5texto.txt", "w");
    int counting = 0, currentchar;
    if (source == NULL){
        fprintf (stderr, "Couldn't open file 1.\n");
        exit(EXIT_SUCCESS);
    }
    char messageBoxMessage[STRLEN], *headofwindow = "Remember!";
    struct time howLongUntil (struct tm *now, struct tm *future);
    time_t rawtime;
    struct tm *currTime;
    struct time result;

    time (&rawtime);
    currTime = localtime (&rawtime);

    result = howLongUntil (currTime, &P5Time);


    if (result.years)
        fprintf (source, "%i years, ", result.years);
    if (result.months == 1)
        result.months = 0;
    if (result.months)
        fprintf (source, "%i months, ", result.months - 1);
    if (result.days)
        fprintf (source, "%i days ", result.days);
    fprintf (source, "for Persona 5!\n");
    fclose (source);
    potato = fopen ("P5texto.txt", "r");
    if (potato == NULL){
        fprintf (stderr, "Couldn't open file 1.\n");
        exit(EXIT_SUCCESS);
    }

    ShowWindow (GetConsoleWindow(), SW_MINIMIZE);
    fgets (messageBoxMessage, STRLEN, potato);
    MessageBox (0, messageBoxMessage, headofwindow, MB_ICONEXCLAMATION);
    fclose (potato);

}

void createStartupBat (void)  //Determines the path of the startup folder so the batch files can be written in them. (In W7)
{
    FILE *buffile, *batfile;
    buffile = fopen ("buffile.txt", "w");
    DWORD buff = 20;
    int stringlength = 0, i;
    char username[buff], defUsername[buff], pathbuf[120], *chequeo;
    bool check;

    //Get User and get rid of null character
    check = GetUserName(username, &buff);
    if (!check)
        printf ("Erorr. Couldn't get username.\n");
    else
        while (username[stringlength] != '\0'){
            defUsername[stringlength] = username[stringlength];
            ++stringlength;
        }

        //Print path in file to get rid of double backslash
        fprintf( buffile, "C:\\Users\\%.*s\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup", stringlength, defUsername);
        fclose(buffile);

        //Get path from file and set it
        buffile = fopen ("buffile.txt", "r");
        fgets (pathbuf, 120, buffile);
        chdir (pathbuf);

        //Create batch in startup
        batfile = fopen ("P5-2.bat", "w");
        fprintf (batfile, "start /D \"C:\\Users\\Jack\\\" /MIN P5-1.bat");

        fclose (batfile);
        fclose (buffile);
}

void createBatchfiles (void)    //Create both batch files.
{
    void createStartupBat (void);
    FILE *batfile;

    batfile = fopen ("P5-1.bat", "w");
    fprintf (batfile, "@echo off\nTIMEOUT /t 30 /nobreak\nstart /D \"C:\\Users\\Jack\\\" /B P5.exe");
    fclose (batfile);
    createStartupBat();
}





