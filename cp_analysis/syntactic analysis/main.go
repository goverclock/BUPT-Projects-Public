package main

import (
	"bufio"
	"fmt"
	"os"
	"synt/ll1"
	"synt/lr"
	"synt/util"
)

func task2() {
	g := util.NewGrammar("in1.dat")
	g.ShowNTPS()
	g.ShowFirst()
	g.ShowFollow()
	ll1.ShowTable(g)

	fmt.Println("string to analysis:")
	scan := bufio.NewScanner(os.Stdin)
	scan.Scan()
	ll1.Analysis(g, scan.Text())
}

func task3() {
	g := util.NewGrammar("in0.dat")
	ps := util.NewProducers("in0.dat")
	lr.InitAction("action.dat")
	lr.ShowAction(g)

	fmt.Println("string to analysis:")
	scan := bufio.NewScanner(os.Stdin)
	scan.Scan()
	lr.Analysis(ps, scan.Text())
}

func main() {
	// task2()
	task3()
}
