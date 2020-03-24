#include <stdio.h>
#include <time.h>

/* filecopy:  copy file ifp to file ofp */
void filecopy(FILE *ifp, FILE *ofp)
{
    int c;

    while ((c = getc(ifp)) != EOF)
        putc(c, ofp);

}

/* cat:  concatenate files, version 2 */
int main(int argc, char *argv[])
{   
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    FILE *fp;
    void filecopy(FILE *, FILE *);
    char *prog = argv[0];   /* program name for errors */

    if (argc == 1)  /* no args; copy standard input */
        filecopy(stdin, stdout);
    else
        while (--argc > 0)
            if ((fp = fopen(*++argv, "r")) == NULL) {
                fprintf(stderr, "%s: can′t open %s\n",
			prog, *argv);
                return 1;
            } else {
                filecopy(fp, stdout);
                fclose(fp);
            }

    if (ferror(stdout)) {
        fprintf(stderr, "%s: error writing stdout\n", prog);
        return 2;
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;  
    printf("%f",cpu_time_used);
    return 0;
}
