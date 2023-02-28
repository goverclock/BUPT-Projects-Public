package lr

import (
	"fmt"
	"strconv"
	"unicode"
)

var act map[pos]string
var buf string
var ptr int
var statStack []int
var symbolStack []rune

func top(s []int) int {
	return s[len(s)-1]
}

func Analysis(ps []string, s string) {
	act = GetAction()
	statStack = make([]int, 0)
	symbolStack = make([]rune, 0)

	statStack = append(statStack, 0)
	buf = s + "$"
	ptr = 0

	fmt.Println("\nStack\t\t\tBuffer\t\t\tAction")
	fmt.Println("--------------------------------------------------------------------")
	for {
		for _, v := range statStack {
			fmt.Print("I" + strconv.Itoa(v) + " ")
		}
		fmt.Print("\t\t\t")
		fmt.Print(buf[ptr:])
		fmt.Print("\t\t\t")

		stat := top(statStack)
		a := rune(buf[ptr])
		if unicode.IsDigit(a) {
			a = 'n'
		}

		item := action[pos{stat, a}]
		if item[0] == 'S' {
			to, _ := strconv.Atoi(item[1:])
			statStack = append(statStack, to)
			symbolStack = append(symbolStack, a)
			if a == 'n' {
				for unicode.IsDigit(rune(buf[ptr])) {
					ptr++
				}
			} else {
				ptr++
			}
			fmt.Print("Shift ", to)
		} else if item[0] == 'R' {
			x, _ := strconv.Atoi(item[1:]) // Rx
			p := ps[x-1]                   // produce A -> ...
			A := rune(p[0])
			b := len(p[2:])
			statStack = statStack[:len(statStack)-b] // pop b elements
			symbolStack = symbolStack[:len(symbolStack)-b]

			stat = top(statStack)
			to, _ := strconv.Atoi(act[pos{stat, A}])
			statStack = append(statStack, to)
			symbolStack = append(symbolStack, A)
			fmt.Print("reduce by " + p)
		} else if item == "ACC" {
			fmt.Print("Accepted")
			return
		} else {
			fmt.Print("error")
			return
		}

		fmt.Println()
		for _, v := range symbolStack {
			fmt.Print(string(v))
		}
		fmt.Println()
	}
}
