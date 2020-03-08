package main

import (
	"golang.org/x/tour/wc"
	"strings"
)

func WordCount(s string) map[string]int {
	mapa := make(map[string]int)
	words := strings.Fields(s)
	for i := 0; i < len(words); i++ {
		if elem, ok := mapa[words[i]]; !ok {
			mapa[words[i]] = 1
			_ = elem
		} else {
			mapa[words[i]] = mapa[words[i]] + 1
			_ = elem
		}
		


	}
	return mapa
}

func main() {
	wc.Test(WordCount)
}
