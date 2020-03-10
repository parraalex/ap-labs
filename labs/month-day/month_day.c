#include <stdio.h>
#include <stdlib.h>

void month_day(int year, int yearday, int* pyear, int* pday){
    int mDay;
	int tmpYDay = *pday;
	int count = 0;

	int monthTable[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	char *monthNames[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dic"};

	if (year % 4 == 0){
        monthTable[1] = 29;
    }
	for(int i = 0; i < 12; i++){
		if (tmpYDay > monthTable[i]){
			tmpYDay = tmpYDay - monthTable[i];
			count ++;
		}else{
			break;
		}
	}

	printf("%s %d, %d\n", monthNames[count], tmpYDay, *pyear);
}

int main(int argc, char* argv[]) {

	int year = atoi(argv[1]);
	int day = atoi(argv[2]);
	int* aYear = &year;
	int* aDay = &day;
	mDay(year, day, aYear, aDay);


	return 0;
}
