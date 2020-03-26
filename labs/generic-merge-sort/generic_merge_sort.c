#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//merge sort in detail function
void mergeN(int completeList[],int left,int middle,int right) {
    int x = 0;
    int y = 0;
    int z = left;
    int lSize = middle-left+1;
    int rSize = right-middle;
    int tmpLeft[lSize];
    int tmpRight[rSize];
    for(int i = 0; i < lSize; i++){
        tmpLeft[i] = completeList[left+i];
    }
    for(int j = 0; j < rSize; j++){
        tmpRight[j] = completeList[middle+1+j];
    }
    while(x<lSize && y<rSize){
        if(tmpLeft[x]>tmpRight[y]){
            completeList[z] = tmpRight[y];
            y++;
        }else{
            completeList[z] = tmpLeft[x];
            x++;
        }
        z++;
    }
    while(x<lSize){
        completeList[z] = tmpLeft[x];
        x++;
        z++;
    }
    while(y<rSize){
        completeList[z] = tmpRight[y];
        y++;
        z++;
    }
}

//merge sort algorithm function
void mergeSort(int completeList[], int left, int right){
    if(left<right){
        int middle = (left+right)/2;
        mergeSort(completeList, left, middle);
        mergeSort(completeList, middle+1, right);
        mergeN(completeList, left, middle, right);
    }
}

int main(int argc, char *argv[]) {
    int completeList[argc-1];
    //defining error in input
    if(argc < 2){
        printf("When you call these program pass numbers to merge. Exiting\n");
        return 0;
    }
    for(int i = 0; i < argc-1; i++){
        completeList[i] = atoi(argv[i+1]);
    }
    
    //calling mergesort
    mergeSort(completeList,0,argc-2);
    printf("List sorted:\n");
    for(int i = 0; i < argc-1; i++){
        printf("%d ", completeList[i]);
    }
    printf("\n");
    return 0;
}
