package main

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"
)

var files []string

// scanDir stands for the directory scanning implementation
func scanDir(dir string) error {
	err := filepath.Walk(dir, func(path string, info os.FileInfo, err error) error {

		files = append(files, os.FileMode.String(info.Mode()))
		return nil
	})
	if err != nil {
		//fmt.Println("second")
		panic(err)
	}
	/*for i := 0; i < len(files); i++ {

		fmt.Println(files[i])
	}*/

	return nil
}
func scanFiles(list []string) {
	dirCount := 0
	sockCount := 0
	devCount := 0
	otherCount := 0
	symCount := 0

	for i := 0; i < len(files); i++ {
		//fmt.Println(files[i])
		if strings.ToLower(string(files[i][0])) == "l" {
			//fmt.Println("this is a symbolic link")
			symCount++
		} else if strings.ToLower(string(files[i][0])) == "d" {
			//fmt.Println("this is a directory")
			dirCount++
		} else if strings.ToLower(string(files[i][0])) == "s" {
			//fmt.Println("this is a socket")
			sockCount++
		} else if strings.ToLower(string(files[i][0])) == "c" || strings.ToLower(string(files[i][0])) == "b" {
			//fmt.Println("this is a device ")
			devCount++
		} else {
			//fmt.Println("this is a other file")
			otherCount++
		}

	}
	fmt.Printf("Directory Scanner Tool\n")
	fmt.Printf("+-------------------------+------+\n")
	fmt.Printf("| Path                    | %v |\n", os.Args[1])
	fmt.Printf("| Directories             | %v   |\n", dirCount)
	fmt.Printf("| Symbolic Links          | %v    |\n", symCount)
	fmt.Printf("| Devices                 | %v   |\n", devCount)
	fmt.Printf("| Sockets                 | %v    |\n", sockCount)
	fmt.Printf("| Other files             | %v   |\n", otherCount)
	fmt.Printf("+-------------------------+------+\n")
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}
	scanDir(os.Args[1])
	scanFiles(files)

}

