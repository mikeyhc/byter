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

	for(i=1; i<argc-2; i++){
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

/* compare_files
 * does the comparision and printing of 2 files
 *
 * param file1: the first file
 * param file2: the second file
 * return: an exit status
 */
int compare_files(char *file1, char *file2)
{
	FILE *fh1, *fh2;
	char buf1[21], buf2[21];
	int i;

	fh1 = fopen(file1, "rb");
	if(!fh1){
		fprintf(stderr,  "could not read %s\n", file1);
		return EXIT_FAILURE;
	}
	fh2 = fopen(file2, "rb");
	if(!fh2){
		close(fh1);
		fprintf(stderr, "could not read %s\n", file2);
		return EXIT_FAILURE;
	}

	while(fgets(buf1, 21, fh1)!=NULL){
		if(fgets(buf2, 21, fh2)==NULL)
			break;
		for(i=0; buf1[i]!='\0'; i++)
			printf(" %02X", buf1[i]);
		printf("\n");
		for(i=0; buf2[i]!='\0'; i++)
			printf(" %02X", buf2[i]);
		printf("\n");
		for(i=0; buf1[i]!='\0'; i++){
			if(buf1[i]=='\t')
				printf(" \\t");
			else if(buf1[i]=='\n')
				printf(" \\n");
			else
				printf("  %c", buf1[i]);
		}
		printf("\n");
		for(i=0; buf2[i]!='\0'; i++){
			if(buf2[i]=='\t')
				printf(" \\t");
			else if(buf2[i]=='\n')
				printf(" \\n");
			else
				printf("  %c", buf2[i]);

		}
		printf("\n");
		printf("\n");
	}




	close(fh1);
	close(fh2);
	return EXIT_SUCCESS;
}/* end: compare_files */

/* main
 * do commandline stuff and start the main bulk
 * 
 * param argc: the number of args
 * param argv: the arguments themselves
 */
int main(int argc, char **argv)
{
	if(argc<3){
		fprintf(stderr, "require at least two file names\n");
		print_usage(argv[0], EXIT_FAILURE);
	}
	process_cmd_args(argc, argv);
	return compare_files(argv[argc-2], argv[argc-1]);
}/* end: main */
