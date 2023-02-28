package util

import (
	"errors"
	"fmt"

	mapset "github.com/deckarep/golang-set/v2"
)

var follow map[rune]mapset.Set[rune]

func (g *Grammar) Follow(a string) (mapset.Set[rune], error) {
	if !g.N.Contains(rune(a[0])) {
		// log.Fatal("querying terminator for Follow set")
		return nil, errors.New("querying terminator for Follow set")
	}

	if first == nil {
		g.First(" ")
	}
	if follow == nil {
		// calculate on first call
		follow = make(map[rune]mapset.Set[rune])

		for n := range g.N.Iter() {
			follow[n] = mapset.NewSet[rune]()
		}
		follow[g.S].Add('$')

		good := false
		for !good {
			good = true
			for n := range g.N.Iter() {
				for _, np := range g.P[n] { // for each producer
					// A -> aBb, add First(b) none Ep into follow[B]
					for i := 1; i < len(np); i++ {
						pre := rune(np[i-1])
						cur := string(np[i])
						if !g.N.Contains(pre) {
							continue
						}

						for e := range g.First(cur).Iter() {
							if e != Ep && !follow[pre].Contains(e) {
								good = false
								follow[pre].Add(e)
							}
						}

						// if Ep in First(b) , add follow[A] into follow[B]
						if g.First(cur).Contains(Ep) {
							for e := range follow[n].Iter() {
								if !follow[pre].Contains(e) {
									follow[pre].Add(e)
								}
							}
						}
					}

					// A -> aB, add follow[A] into follow[B]
					last := rune(np[len(np)-1])
					if g.N.Contains(last) {
						sz := follow[last].Cardinality()
						follow[last] = follow[last].Union(follow[n])
						if follow[last].Cardinality() > sz {
							good = false
						}
					}
				}
			}
		}

	}

	return follow[rune(a[0])], nil
}

func (g *Grammar) ShowFollow() {
	if follow == nil {
		g.Follow(string(g.S))
	}
	fmt.Println("Follow:")
	for r, v := range follow {
		fmt.Print("  " + string(r) + ": ")
		ShowRuneSet(v)
		fmt.Println()
	}
}
