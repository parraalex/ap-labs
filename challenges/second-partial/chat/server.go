package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"strings"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

type clientData struct {
	ch         chan<- string
	username   string
	ip         string
	permission bool
}

var (
	entering       = make(chan client)
	leaving        = make(chan client)
	messages       = make(chan string) // all incoming client messages
	listUsers      = make(chan string) //when user asks for list of users
	privateMessage = make(chan string) //for sending private messages
	searchUserInfo = make(chan string)
	kickUser       = make(chan string)
)

func broadcaster() {
	clients := make(map[client]bool)     // all connected clients
	users := make(map[string]clientData) //users list
	first := true

	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			if msg == "external" {
				userID := <-messages
				if _, ok := users[userID]; ok {
					messages <- "ok"
					data := <-messages
					for cli := range clients {
						cli <- data
					}
				} else {
					messages <- "banned"
				}
			} else {
				data := <-messages
				for cli := range clients {
					cli <- data
				}
			}

		case cli := <-entering:
			clients[cli] = true
			key := <-listUsers
			if first {
				listUsers <- "first"
				first = false
				users[key] = clientData{cli, key, <-listUsers, true}
			} else {
				listUsers <- "not first"
				users[key] = clientData{cli, key, <-listUsers, false}
			}
		case cli := <-leaving:
			delete(clients, cli)
			delete(users, getUser(cli, users).username)
			close(cli)
		case msg := <-listUsers:
			_ = msg
			listF := ""
			first := true
			for userk, user := range users {
				_ = userk
				if first {
					listF = user.username
					first = false
				} else {
					listF += ", " + user.username
				}
			}
			listUsers <- listF

		case msg := <-privateMessage:
			receiever := msg
			data := <-privateMessage
			sender := <-privateMessage
			if userFound, ok := users[receiever]; ok {
				userFound.ch <- "[private: " + sender + "]" + " > " + data
				privateMessage <- "irc server > Message sent to " + receiever + "!"
			} else {
				privateMessage <- "user not found"
			}

		case msg := <-searchUserInfo:
			if userFound, ok := users[msg]; ok {
				searchUserInfo <- userFound.ip
			} else {
				searchUserInfo <- "user not found"
			}
		case msg := <-kickUser:
			if users[msg].permission == true {
				kickUser <- "true"
				personToKick := <-kickUser
				if userFound, ok := users[personToKick]; ok {
					chanDel := userFound.ch
					chanDel <- "irc server > You're kicked from this channel"
					chanDel <- "irc server > Bad language is not allowed on this channel"
					delete(clients, chanDel)
					delete(users, personToKick)
					close(chanDel)
					kickUser <- "success"
				} else {
					kickUser <- "error"
				}
			} else {
				kickUser <- "false"
			}
		}

	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)
	who := conn.RemoteAddr().String()
	ban := false
	ch <- "irc server > Welcome to simple IRC Server. Press enter to continue."
	input := bufio.NewScanner(conn)
	input.Scan()
	nickname := input.Text()
	messages <- "internal"
	messages <- "irc server > New connected user: " + nickname
	entering <- ch
	listUsers <- nickname
	if <-listUsers == "first" {
		ch <- "irc server > Your user " + nickname + " is successfully logged"
		ch <- "irc-server > Congrats, you were the first user."
		ch <- "irc-server > You're the new IRC Server ADMIN"
	} else {
		ch <- "irc server > Your user " + nickname + " is successfully logged"
	}
	listUsers <- who
	for input.Scan() {
		if ban == false {
			words := strings.Split(input.Text(), " ")
			switch words[0] {
			case "/users":
				if len(words) == 1 {
					listUsers <- who
					usersList := <-listUsers
					ch <- "irc server > " + usersList
				} else {
					ch <- "irc server > Error in /user: You must not add extra arguments. Try Again"
				}

			case "/msg":
				if len(words) >= 3 {
					privateMessage <- words[1]
					indets := 6 + len(words[1]) - 1
					privateMessage <- input.Text()[indets:]
					privateMessage <- nickname
					resultM := <-privateMessage
					if resultM == "user not found" {
						ch <- "irc server > Error in /msg: User not found"
					} else {
						ch <- resultM
					}

				} else {
					ch <- "irc server > Error in /msg: Usage of this command is /msg <user> <msg>"
				}
			case "/time":
				t := time.Now()
				timezone := ""
				off := 0
				if os.Getenv("TZ") != "" {
					timezone = os.Getenv("TZ")
				} else {
					timezone, off = t.Zone()
				}
				_ = off
				ch <- "irc server > Local time: " + timezone + " " + t.Format("15:04")
				_ = t
			case "/user":
				if len(words) == 2 {
					searchUserInfo <- words[1]
					result := <-searchUserInfo
					if result == "user not found" {
						ch <- "irc server > Error in /user: Not found"
					} else {
						ch <- "irc server > Username: " + words[1] + ", IP: " + result
					}
				} else {
					ch <- "irc server > Error in /users: You must select only one user. Try again"
				}
			case "/kick":
				if len(words) == 2 {
					kickUser <- nickname
					response := <-kickUser
					if response == "true" {
						kickUser <- words[1]
						if <-kickUser == "success" {
							messages <- "internal"
							messages <- "irc-server > " + words[1] + " was kicked from channel for bad language policy violation"

						} else {
							ch <- "irc server > Error in /kick: User not found."
						}
					} else {
						ch <- "irc server > You don't have the permissions to kick a user."
					}
				} else {
					ch <- "irc server > Error in /kick: Usage of this command: /kick <user>"
				}

			default:
				messages <- "external"
				messages <- nickname
				if <-messages == "banned" {
					ban = true
				} else {
					messages <- nickname + " > " + input.Text()
				}
			}
			_ = words
		} else {
			ch <- "irc server > Message not sent. You are banned, cannot send messages."
		}
	}
	// NOTE: ignoring potential errors from input.Err()

	leaving <- ch
	messages <- "internal"
	messages <- "irc server > " + nickname + " left channel"
	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {
	args := os.Args
	host := ""
	port := ""
	if len(args) == 5 && (args[1] == "-host" || args[3] == "-host") && (args[1] == "-port" || args[3] == "-port") {
		if args[1] == "-host" {
			host = args[2]
			port = args[4]
		} else {
			host = args[4]
			port = args[2]
		}
		listener, err := net.Listen("tcp", host+":"+port)
		if err != nil {
			log.Fatal(err)
		}
		go broadcaster()
		for {
			conn, err := listener.Accept()
			if err != nil {
				log.Print(err)
				continue
			}
			go handleConn(conn)
		}
	} else {
		println("try again, wrong arguments")
	}
	
}
func getUser(ch chan<- string, list map[string]clientData) clientData {
	for i, j := range list {
		_ = i
		if j.ch == ch {
			return j
		}
	}
	return clientData{make(chan string), "error", "error", false}
}

