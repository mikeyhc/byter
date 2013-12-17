#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define ROW_LENGTH 20

#define printf_color(x,y,c, z) printf("%s"x"%s", color && c!=z ?	\
			"\033[1;31;40m" : "", y, color && c!=z ? "\033[m" : "")

static int color = 0;
static int right = 0;
static int wrong = 0;

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

	for(i=1; i<argc; i++)
		if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
			print_usage(argv[0], EXIT_SUCCESS);
		else if(!strcmp(argv[i], "-c") || !strcmp(argv[i], "--color"))
			color = 1;
		else if(argv[i][0]=='-'){
			fprintf(stderr, "unknown option: %s\n", argv[i]);
			print_usage(argv[0], EXIT_FAILURE);
		}
}/* end: process_cmd_args */

/* print_hex_row
 * prints out a row in hex format
 *
 * param str: the string to print
 * param other: the string to check against
 * param offset: the offset of str in the total string
 */
void print_hex_row(char *str, char *other, int offset)
{
	int i;

	printf(" %06d", offset);
	for(i=0; str[i]!='\0'; i++){
		printf_color(" %02X", str[i], str[i], other[i]);
		if(str[i]==other[i])
			right++;
		else
			wrong++;
	}
	printf("\n       ");
	for(i=0; other[i]!='\0'; i++)
		printf_color(" %02X", other[i], other[i], str[i]);
	printf("\n");
}/* end: print_hex_row */

/* print_char_row
 * prints out a row in char format
 *
 * param str: the string to print
 * param other: the string to check against
 */
void print_char_row(char *str, char *other)
{
	int i;
	char c;

	printf("       ");
	for(i=0; str[i]!='\0'; i++){
		if(str[i]=='\n')
			printf_color(" %s", "\\n", str[i], other[i]);
		else if(str[i]=='\t')
			printf_color(" %s", "\\t", str[i], other[i]);
		else
			printf_color("  %c", str[i], str[i], other[i]);
	}
	printf("\n       ");
	for(i=0; other[i]!='\0'; i++)
		if(other[i]=='\n')
			printf_color(" %s", "\\n", other[i], str[i]);
		else if(other[i]=='\t')
			printf_color(" %s", "\\t", other[i], str[i]);
		else
			printf_color("  %c", other[i], other[i], str[i]);
	printf("\n");
}/*end: print_char_row */

/* compare_files
 * does the comparision and printing of 2 files
 *
 * param file1: the first file
 * param file2: the second file
 * return: an exit status
 */
int compare_files(char *file1, char *file2)
{
	int fh1, fh2, r1, r2;
	unsigned ur1, ur2;
	char buf1[ROW_LENGTH + 1], buf2[ROW_LENGTH + 1];
	int i, pos;

	fh1 = open(file1, 0, O_RDONLY);
	if(fh1==-1){
		fprintf(stderr,  "could not read %s - %s\n", file1, 
			strerror(errno));
		return EXIT_FAILURE;
	}
	fh2 = open(file2, 0, O_RDONLY);
	if(fh2==-1){
		close(fh1);
		fprintf(stderr, "could not read %s - %s\n", file2,
			strerror(errno));
		return EXIT_FAILURE;
	}

	pos = 0;

	while((r1 = read(fh1, buf1, ROW_LENGTH))>0){
		if((r2 = read(fh2, buf2, ROW_LENGTH))<=0)
			break;
		buf1[r1] = '\0';
		buf2[r2] = '\0';
		print_hex_row(buf1, buf2, pos*ROW_LENGTH);
		print_char_row(buf1, buf2);
		printf("\n");
		pos++;
	}

	if(r1==-1)
		fprintf(stderr, "Error reading from %s - %s\n",
			file1, strerror(errno));
	else if(r2==-1)
		fprintf(stderr, "Error reading from %s - %s\n",
			file2, strerror(errno));

	i = lseek(fh1, 0, SEEK_CUR);
	ur1 = lseek(fh1, 0, SEEK_END) - i;
	i = lseek(fh2, 0, SEEK_CUR);
	ur2 = lseek(fh2, 0, SEEK_END) - i;

	close(fh1);
	close(fh2);

	if(color)
		printf("\t\033[1;36;40m%6d\033[m %s unread\n\t\033[1;36;40m"
			"%6d\033[m %s unread\n\t\033[1;33;40m%6d\033[m "
			"total\n\t\033[1;32;40m%6d\033[m matched\n\t"
			"\033[1;31;40m%6d\033[m no match\n", ur1, file1, ur2,
			file2, right + wrong, right, wrong);
	else
		printf("\t%6d %s unread\n\t%6d %s unread\n\t%6d total\n\t%6d "
			"matched\n\t%6d no match\n", ur1, file1, ur2, file2, 
			right+wrong, right, wrong);
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
	int f1, f2;

	if(argc<3){
		fprintf(stderr, "require at least two files\n");
		print_usage(argv[0], EXIT_FAILURE);
	}

	process_cmd_args(argc, argv);
	for(f1=1; f1<argc; f1++)
		if(argv[f1][0]!='-')
			break;
	for(f2=f1+1; f2<argc; f2++)
		if(argv[f2][0]!='-')
			break;
	if(f2==argc){
		fprintf(stderr, "requires at least 2 files\n");
		print_usage(argv[0], EXIT_FAILURE);
	}
	return compare_files(argv[f1], argv[f2]);
}/* end: main */
