package main

import (
	"os"
	"strconv"
	"time"
)

func buildPipeline(n int) (chan<- int, <-chan int) {
	inbox := make(chan int)
	output := inbox
	for i := 0; i < n; i++ {
		prevChan := output
		nextChan := make(chan int)
		go func() {
			for data := range prevChan {
				nextChan <- data
			}
			close(nextChan)
		}()
		output = nextChan
	}
	return inbox, output
}

func main() {
	if len(os.Args) != 2 {
		println("Usage: ./chapter9-4 numRoutinesCreated")
		os.Exit(1)
	}
	numRoutines := 0
	if numR, err := strconv.Atoi(os.Args[1]); err == nil {
		numRoutines = numR
	} else {
		println("Error, please input an integer for the parameter")
		os.Exit(2)
	}
	timeWatch := time.Now()
	inbox, output := buildPipeline(numRoutines)
	inbox <- numRoutines
	<-output
	//in this moment the output chan recieves the whole traversed message from the inbox
	endTime := time.Now()
	result := endTime.Sub(timeWatch)
	println("Total time to traverse the entire pipeline: ", result)
}

