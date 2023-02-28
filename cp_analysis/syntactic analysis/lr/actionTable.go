package lr

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
	"synt/util"
)

type pos struct {
	stat int
	t    rune
}

var action map[pos]string

// read from file "action.dat"
func InitAction(fname string) {
	if action != nil {
		return
	}
	fmt.Println("reading action table from in.dat ...")
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
	action = make(map[pos]string)
	// first line is header
	ts := []rune{}
	for _, v := range strings.Fields(raw[0]) {
		ts = append(ts, rune(v[0]))
	}

	// next several lines are status
	for i, v := range raw {
		if i == 0 {
			continue
		}
		vs := strings.Fields(v)
		vs = vs[1:]
		for j := 0; j < len(vs); j++ { // len(vs) == len(ts)
			action[pos{i - 1, ts[j]}] = vs[j]
		}
	}
}

func GetAction() map[pos]string {
	return action
}

func ShowAction(g *util.Grammar) {
	ts := append(append(g.T.ToSlice(), '$'), g.N.ToSlice()...)
	fmt.Print("\t")
	for _, t := range ts {
		fmt.Print(string(t) + "\t")
	}
	fmt.Println()

	for i := 0; action[pos{i, ts[0]}] != ""; i++ {
		fmt.Print("I" + strconv.Itoa(i), "\t")
		for _, t := range ts {
			fmt.Print(action[pos{i, t}] + "\t")
		}
		fmt.Println()
	}
}
