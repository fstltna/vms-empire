/*
 *    Copyright (C) 1987, 1988 Chuck Simmons
 * 
 * See the file COPYING, distributed with empire, for restriction
 * and warranty information.
 */

/*
main.c -- parse command line for empire

options:

    -w water: percentage of map that is water.  Must be in the range
              10..90.  Default is 70.
	      
    -s smooth: amount of smoothing performed to generate map.  Must
	       be a nonnegative integer.  Default is 5.
	       
    -d delay:  number of milliseconds to delay between output.
               default is 2000 (2 seconds).

    -S saveinterval: sets turn interval between saves.
	       default is 10
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "empire.h"
#include "extern.h"

#define OPTFLAGS "w:s:d:S:f:b"

char* RemoveSpaces(char* source)
{
    char *output=source;
    for (int i = 0, j = 0; i<strlen(source); i++,j++)                        /* Evaluate each character in the source */
    {
        if (((source[i] >= 48) && (source[i] <= 57)) || ((source[i] >= 65) && (source[i] <= 90)) || ((source[i] >= 97) && (source[i] <= 122)) ) /* If the character is allowed */
	{
            output[j]=source[i]; /* Copy that character to the output char[] */
	}
        else
	{
            output[j]='_'; /* Copy the replacement character to the output char[] */
	}
    }
    /*output[j] = '\0';*/
    return output;
}
char PlayerName[256] = "empsave.dat"; /* Used to keep the string safe */
int main(argc, argv)
int argc;
char *argv[];
{
    int c;
    extern char *optarg;
    extern int optind;
    int errflg = 0;
    int wflg, sflg, dflg, Sflg;
    int land;
	
    wflg = 70; /* set defaults */
    sflg = 5;
    dflg = 2000;
    Sflg = 10;
    savefile = PlayerName;
    bbsmode = 0;

    /*
     * extract command line options
     */

    while ((c = getopt (argc, argv, OPTFLAGS)) != EOF) {
	switch (c) {
	case 'w':
	    wflg = atoi (optarg);
	    break;
	case 's':
	    sflg = atoi (optarg);
	    break;
	case 'd':
	    dflg = atoi (optarg);
	    break;
	case 'S':
	    Sflg = atoi (optarg);
	    break;
	case 'f':
	    savefile = optarg;
	    break;
	case 'b':
	    bbsmode = 1;
	    break;
	case '?': /* illegal option? */
	    errflg++;
	    printf("Unknown option: '%s'\n", optarg); 
	    break;
	}
    }
    savefile = RemoveSpaces(savefile);
    printf("Playing as Player Name = '%s'\n", savefile);
    if (errflg || (argc-optind) != 0) {
	(void) printf ("empire: usage: empire [-w water] [-s smooth] [-d delay] [-f savefile] [-b]\n");
	exit (1);
    }

    if (wflg < 10 || wflg > 90) {
	(void) printf ("empire: -w argument must be in the range 0..90.\n");
	exit (1);
    }
    if (sflg < 0) {
	(void) printf ("empire: -s argument must be greater or equal to zero.\n");
	exit (1);
    }
	
    if (dflg < 0 || dflg > 30000) {
	(void) printf ("empire: -d argument must be in the range 0..30000.\n");
	exit (1);
    }

    SMOOTH = sflg;
    WATER_RATIO = wflg;
    delay_time = dflg;
    save_interval = Sflg;

    /* compute min distance between cities */
    land = MAP_SIZE * (100 - WATER_RATIO) / 100; /* available land */
    land /= NUM_CITY; /* land per city */
    MIN_CITY_DIST = isqrt (land); /* distance between cities */

    empire (); /* call main routine */
    return (0);
}
