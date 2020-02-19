#include <stdio.h>
#include <stdlib.h>

#define   LOWER  0       /* lower limit of table */
#define   UPPER  300     /* upper limit */
#define   STEP   20      /* step size */

/* print Fahrenheit-Celsius table */

int main(int argc, char **argv)
{
int fahr;
    if(argc<3){
		printf("fahrenheit: %3d, celsius: %6.1f \n", atoi(argv[1]), (5.0/9.0)*(atoi(argv[1])-32));
	}
	else{
	int l = atoi(argv[1]);
	int u = atoi(argv[2]);
	int s = atoi(argv[3]);	
    for (fahr = l; fahr <= u; fahr = fahr + s)
	printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
}
    return 0;
}
