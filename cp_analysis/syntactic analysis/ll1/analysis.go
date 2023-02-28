package ll1

import (
	"fmt"
	"synt/util"
	"unicode"

	mapset "github.com/deckarep/golang-set/v2"
)

var tb map[pos]mapset.Set[string]
var buf string
var ptr int
var st []rune

func reverse(s string) string {
	rns := []rune(s)
	for i, j := 0, len(rns)-1; i < j; i, j = i+1, j-1 {
		rns[i], rns[j] = rns[j], rns[i]
	}
	return string(rns)
}

func top(s []rune) rune {
	return s[len(s)-1]
}

func Analysis(g *util.Grammar, s string) {
	tb = GetTable(g)
	buf = s + "$"
	ptr = 0
	st = make([]rune, 0)
	st = append(st, '$')
	st = append(st, g.S)

	fmt.Println("\nStack\t\t" + "Buffer\t\t" + "Producer")
	fmt.Println("------------------------------------------------")
	for {
		showStack()
		showBuf()

		x := top(st)
		a := rune(buf[ptr])
		if unicode.IsDigit(a) {
			a = 'n'
		}

		if g.T.Contains(x) || x == '$' {
			if x == a {
				st = st[:len(st)-1]
				if a == 'n' {
					for unicode.IsDigit(rune(buf[ptr])) {
						ptr++
					}
				} else {
					ptr++
				}
			} else {
				fmt.Println("Unaccepted")
				return
			}
		} else { // x is in N
			tbItem := tb[pos{x, a}]
			if tbItem.Cardinality() != 0 {
				st = st[:len(st)-1]
				// using x -> i
				for i := range tbItem.Iter() {
					// expect loop only once
					rev := reverse(i)
					rev = rev[:len(rev)-3]
					for _, r := range rev {
						if r != util.Ep {
							st = append(st, r)
						}
					}
					showProduce(i)
				}
			} else {
				fmt.Println("Unaccepted")
				return
			}
		}

		if x == '$' {
			fmt.Println("\nAccepted")
			break
		}
		fmt.Println()
	}

}

func showStack() {
	for _, v := range st {
		fmt.Print(string(v))
	}
	fmt.Print("\t\t")
}

func showBuf() {
	fmt.Print(buf[ptr:])
	fmt.Print("\t\t")
}

func showProduce(p string) {
	fmt.Print(p)
}
