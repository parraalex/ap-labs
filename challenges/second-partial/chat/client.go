package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"os"
)

//!+
func main() {
	args := os.Args
	server := ""
	username := ""
	if args[1] == "-server" && args[3] == "-user" {
		username = args[4]
		server = args[2]
	} else if args[3] == "-server" && args[1] == "-user" {
		username = args[2]
		server = args[4]
	} else {
		println("Error in arguments. Usage: ./client -server [localhost] -user [nickname]")
		os.Exit(3)
	}

	conn, err := net.Dial("tcp", server)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Fprint(conn, username)
	done := make(chan struct{})
	go func() {
		io.Copy(os.Stdout, conn) // NOTE: ignoring errors
		log.Println("done")
		done <- struct{}{} // signal the main goroutine
	}()
	mustCopy(conn, os.Stdin)
	conn.Close()
	<-done // wait for background goroutine to finish
}

//!-

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}

