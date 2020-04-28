package main

import (
	"fmt"
	"io"
	"net"
	"os"
	"strings"
	"sync"
)

func main() {
	var clocksRunning sync.WaitGroup
	for i := 1; i < len(os.Args); i++ {
		splitStr := strings.Split(os.Args[i], "=")
		c, err := net.Dial("tcp", splitStr[1])
		println(splitStr[0])
		println(splitStr[1])
		if err != nil {
			fmt.Println(err)
		}
		clocksRunning.Add(1)
		go printTime(c, &clocksRunning, splitStr[0])

	}
	clocksRunning.Wait()
}

func printTime(c net.Conn, clocksRunning *sync.WaitGroup, city string) {
	defer clocksRunning.Done()
	for {
		_, err := io.WriteString(c, city)
		_, err = io.Copy(os.Stdout, c)
		if err != nil {
			return // e.g., client disconnected
		}

	}
}

