package main

import "golang.org/x/tour/pic"

func Pic(dx, dy int) [][]uint8 {
	a := make([][]uint8, dy)
	for i:=0; i<dy; i++ {
		tmp := make([]uint8, dx)
		a[i] = tmp
	}
	for i:=0; i<dy; i++{
		for j:=0; j<dx; j++{
			a[i][j] = uint8(i^j)
		}
	}
	return a
	
}

func main() {
	pic.Show(Pic)
}


