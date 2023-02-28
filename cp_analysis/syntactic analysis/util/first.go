package util

import (
	"fmt"

	mapset "github.com/deckarep/golang-set/v2"
)

var first map[rune]mapset.Set[rune]

func (g *Grammar) First(a string) mapset.Set[rune] {
	if first == nil {
		// calculate on first call
		first = make(map[rune]mapset.Set[rune])

		for t := range g.T.Iter() {
			first[t] = mapset.NewSet[rune]()
			first[t].Add(t)
		}
		for n := range g.N.Iter() {
			first[n] = mapset.NewSet[rune]()
			for _, np := range g.P[n] {
				left := rune(np[0])
				if g.T.Contains(left) {
					first[n].Add(left)
				}
			}
		}

		good := false
		for !good {
			good = true
			for n := range g.N.Iter() {
				for _, np := range g.P[n] {		// for each producer
					left := rune(np[0])
					if !g.N.Contains(left) {
						continue
					}

					for e := range first[left].Iter() {
						if !first[n].Contains(e) {
							first[n].Add(e)
							good = false
						}
					}
				}
			}
		}
	}

	return first[rune(a[0])]
}

func (g *Grammar) ShowFirst() {
	if first == nil {
		g.First(" ")
	}
	fmt.Println("First(T omitted):")
	for r, v := range first {
		if !g.N.Contains(r) {
			continue
		}
		fmt.Print("  " + string(r) + ": ")
		ShowRuneSet(v)
		fmt.Println()
	}
}
