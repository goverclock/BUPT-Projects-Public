package ll1

import (
	"fmt"
	"synt/util"

	mapset "github.com/deckarep/golang-set/v2"
)

type pos struct {
	n rune
	t rune
}

var table map[pos]mapset.Set[string]

func calculateTable(g *util.Grammar) {
	if table == nil {
		table = make(map[pos]mapset.Set[string])
		for n := range g.N.Iter() {
			for t := range g.T.Iter() {
				table[pos{n, t}] = mapset.NewSet[string]()
			}
			table[pos{n, '$'}] = mapset.NewSet[string]()
		}

		for n := range g.N.Iter() {
			for _, np := range g.P[n] { // for each producer
				for r := range g.First(np).Iter() {
					if g.T.Contains(r) {
						table[pos{n, r}].Add(string(n) + "->" + np)
					}
				}
				if g.First(np).Contains(util.Ep) {
					f, _ := g.Follow(string(n))
					for b := range f.Iter() {
						table[pos{n, b}].Add(string(n) + "->" + np)
					}
				}
			}
		}
	}
}

func GetTable(g *util.Grammar) map[pos]mapset.Set[string] {
	calculateTable(g)
	return table
}

func ShowTable(g *util.Grammar) {
	calculateTable(g)
	fmt.Print("Table:\n\t")
	ts := []rune{}
	for t := range g.T.Iter() {
		fmt.Print(string(t), "\t\t")
		ts = append(ts, t)
	}
	ts = append(ts, '$')
	fmt.Println("$")

	for n := range g.N.Iter() {
		fmt.Print(string(n) + "\t")
		for _, t := range ts {
			have := false
			totl := 0
			for p := range table[pos{n, t}].Iter() {
				if have {
					fmt.Print(",")
				}
				have = true
				fmt.Print(p)
				totl += len(p)
			}
			if !have {
				fmt.Print("err\t\t")
			} else {
				fmt.Print("\t\t")
			}
		}
		fmt.Println()
	}
}
