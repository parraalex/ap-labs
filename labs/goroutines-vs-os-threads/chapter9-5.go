package main

import (
	"fmt"
	"time"
)

var communications int //this global variable registers total number of communications.

func sendOne(inbox chan bool, output chan bool) {
	for {
		output <- true
		<-inbox
		communications++
	}
}
func sendTwo(inbox chan bool, output chan bool) {
	for {
		<-inbox
		communications++
		output <- false
	}
}

func main() {
	one := make(chan bool)
	two := make(chan bool)
	go sendOne(one, two)
	go sendTwo(two, one)
	for {
		time.Sleep(time.Second)
		fmt.Println("Comunications: ", communications)
		communications = 0
	}
}

