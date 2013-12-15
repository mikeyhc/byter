#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int color = 0;

/* print_usage
 * prints the usage message
 *
 * param name: the name of the application
 * param ret: the value to exit with
 */
void print_usage(char *name, int ret)
{
	fprintf(stderr, "usage:\n");
	fprintf(stderr, "\t%s [args] file1 file2\n\n", name);
	fprintf(stderr, "Will do a bytewise compare between file1 and file2\n");
	fprintf(stderr, "args:\n");
	fprintf(stderr, "\t-h, --help:  show this dialog\n");
	fprintf(stderr, "\t-c, --color: use colored output\n");

	exit(ret);
}/* end: print_usage */	

/* process_cmd_args
 * process the commandline arguments
 *
 * param argc: the number of args
 * param argv: the arguments themselves
 */
void process_cmd_args(int argc, char **argv)
{
	int i;

	for(i=1; i<argc; i++){
		if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
			print_usage(argv[0], EXIT_SUCCESS);
		else if(!strcmp(argv[i], "-c") || !strcmp(argv[i], "--color"))
			color = 1;
		else{
			fprintf(stderr, "unknown option: %s\n", argv[i]);
			print_usage(argv[0], EXIT_FAILURE);
		}
	}
}/* end: process_cmd_args */

/* main
 * do commandline stuff and start the main bulk
 * 
 * param argc: the number of args
 * param argv: the arguments themselves
 */
int main(int argc, char **argv)
{
	process_cmd_args(argc, argv);
	return EXIT_SUCCESS;
}/* end: main */
