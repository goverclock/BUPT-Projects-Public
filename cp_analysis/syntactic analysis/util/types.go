package util

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strings"

	mapset "github.com/deckarep/golang-set/v2"
)

const Ep = '#'

type Grammar struct {
	N mapset.Set[rune]
	T mapset.Set[rune]
	P map[rune][]string
	S rune
}

func NewGrammar(fname string) *Grammar {
	fmt.Println("reading grammar from " + fname)
	file, err := os.Open(fname)
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()
	raw := []string{}
	scan := bufio.NewScanner(file)
	for scan.Scan() {
		raw = append(raw, scan.Text())
	}

	g := new(Grammar)
	g.N = mapset.NewSet[rune]()
	g.T = mapset.NewSet[rune]()
	g.P = make(map[rune][]string)

	// N, P, S
	for i, s := range raw {
		t := strings.Fields(s)
		n := rune(t[0][0])
		g.N.Add(n)

		if i == 0 {
			g.S = n
		}

		for i, v := range t {
			if i == 0 {
				continue
			}
			s := g.P[n]
			s = append(s, v)
			g.P[n] = s
		}
	}

	// T
	for _, str := range raw {
		for _, c := range str {
			if c == ' ' {
				continue
			}
			if !g.N.Contains(c) {
				g.T.Add(c)
			}
		}
	}

	return g
}

func NewProducers(fname string) []string{
	fmt.Println("reading producers from " + fname)
	file, err := os.Open(fname)
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()
	raw := []string{}
	scan := bufio.NewScanner(file)
	for scan.Scan() {
		raw = append(raw, scan.Text())
	}
	return raw
}

func ShowRuneSet(s mapset.Set[rune]) {
	for e := range s.Iter() {
		fmt.Print(string(e) + " ")
	}
}

func (g *Grammar) ShowNTPS() {
	fmt.Print("N: ")
	ShowRuneSet(g.N)
	fmt.Println()

	fmt.Print("T: ")
	ShowRuneSet(g.T)
	fmt.Println()

	fmt.Print("P: ")
	fir := true
	for i, v := range g.P {
		if fir {
			fir = false
		} else {
			fmt.Print("   ")
		}
		fmt.Print(string(i) + " -> ")
		for _, su := range v {
			fmt.Print(su + " ")
		}
		fmt.Println()
	}

	fmt.Print("S: ")
	fmt.Println(string(g.S))
}
