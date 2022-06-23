#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#define EPSILON '#'
#define NEW_STARTER '$'

std::set<char> none_terminate, terminate;
std::map<char, std::set<std::string>> produce;
char starter;

void debug_info(const std::string prompt);
void del_epsilon();
void del_unit();
void del_useless();

int main() {
    std::string tmp;
    std::cout << "None-terminate(begin with S):"
              << std::endl;  // which means S is always mapped as 1 latter
    std::cin >> tmp;
    starter = tmp[0];
    for (auto c : tmp) none_terminate.insert(c);

    std::cout << "Terminate:" << std::endl;
    std::cin >> tmp;
    for (auto c : tmp) terminate.insert(c);

    std::cout << "P(end with" << EPSILON << "):" << std::endl;
    std::getline(std::cin, tmp);  // eat empty line
    while (std::getline(std::cin, tmp)) {
        std::stringstream ss(tmp);
        std::string item;
        char from;  // from a none terminate(single character)
        ss >> item;
        from = item[0];
        if (from == '#') break;
        while (ss >> item)  // to something
            produce[from].insert(item);
    }

    del_epsilon();
    del_unit();
    del_useless();

    debug_info("result:");
    system("pause");

    return 0;
}

// pi中的符号均可致空?
bool is_to_empty(const std::set<char> &n1, const std::string &pi) {
    if (pi == "#") return true;
    for (auto c : pi)
        if (!n1.count(c)) return false;
    return true;
}

// pi中存在可致空的符号?
bool contains_to_empty(const std::set<char> &n1, const std::string &pi) {
    for (auto c : pi)
        if (n1.count(c)) return true;
    return false;
}

void dfs(const std::set<char> &n1, std::vector<std::string> &ret,
         const std::string &pi, std::string cur, int pos) {
    if (pos == pi.size()) {
        if (!cur.empty()) ret.push_back(cur);
        return;
    }

    if (pi[pos] == '#' || n1.count(pi[pos])) dfs(n1, ret, pi, cur, pos + 1);
    cur.push_back(pi[pos]);
    dfs(n1, ret, pi, cur, pos + 1);
}

// 生成p1包含可致空符号的条目
std::vector<std::string> p1_empty_item(const std::set<char> &n1,
                                       const std::string &pi) {
    std::vector<std::string> ret;
    dfs(n1, ret, pi, "", 0);
    return ret;
}

void del_epsilon() {
    // 计算可致空符
    std::set<char> n1;
    while (1) {
        int origin_size = n1.size();
        for (auto nt : none_terminate) {
            if (n1.count(nt)) continue;
            for (auto pi : produce[nt])
                if (is_to_empty(n1, pi)) {
                    n1.insert(nt);
                    break;
                }
        }
        if (origin_size == n1.size()) break;
    }

    // create new P1
    std::map<char, std::set<std::string>> p1;
    for (auto nt : none_terminate) {
        for (auto pi : produce[nt]) {
            if (pi == "#") continue;
            if (contains_to_empty(n1, pi)) {
                auto ret = p1_empty_item(n1, pi);
                for (auto i : ret) p1[nt].insert(i);
            } else
                p1[nt].insert(pi);
        }
    }

    // new starter if needed
    if (n1.count(starter)) {
        p1[NEW_STARTER].insert("#");
        std::string t;
        t.push_back(starter);
        p1[NEW_STARTER].insert(t);
        none_terminate.insert(NEW_STARTER);
        starter = NEW_STARTER;
    }

    produce = p1;
}

// 是单产生式?仅考虑右侧
bool is_unitp(const std::string pi) {
    return pi.size() == 1 && none_terminate.count(pi[0]);
}

void del_unit() {
    // Ns, Na, Nb ...
    std::map<char, std::set<std::string>> p1;
    std::map<char, std::set<char>> ni;
    for (auto nt : none_terminate) {
        std::queue<char> q;
        q.push(nt);
        while (!q.empty()) {
            char cur = q.front();
            q.pop();
            ni[nt].insert(cur);
            for (auto pi : produce[cur])
                if (is_unitp(pi) && !ni[nt].count(pi[0])) q.push(pi[0]);
        }

        // construct new p1
        for (auto ni_nt : ni[nt])
            for (auto pi : produce[ni_nt])
                if (!is_unitp(pi)) p1[nt].insert(pi);
    }

    produce = p1;
}

bool is_summon_able(const std::set<char> &sum, const std::string &pi) {
    for (auto c : pi)
        if (!sum.count(c)) return false;
    return true;
}

void del_useless() {
    // 计算生成符号集合
    std::set<char> summon(terminate);
    while (1) {
        int origin_size = summon.size();
        for (auto nt : none_terminate) {
            if (summon.count(nt)) continue;
            for (auto pi : produce[nt])
                if (is_summon_able(summon, pi)) {
                    summon.insert(nt);
                    break;
                }
        }
        if (origin_size == summon.size()) break;
    }

    // p1是删除非生成符号后的产生式
    std::map<char, std::set<std::string>> p1;
    std::set<char> nt1;
    for (auto nt : none_terminate) {
        if (!summon.count(nt)) continue;
        nt1.insert(nt);
        for (auto pi : produce[nt])
            if (is_summon_able(summon, pi)) p1[nt].insert(pi);
    }

    produce = p1;
    none_terminate = nt1;

    // 计算可达符号集合
    std::set<char> reach;
    std::set<char> t1;
    nt1.clear();

    std::queue<char> q;
    q.push(starter);
    while (!q.empty()) {
        char cur = q.front();
        q.pop();
        reach.insert(cur);
        if (!none_terminate.count(cur)) continue;

        for (auto pi : produce[cur])
            for (auto c : pi)
                if (!reach.count(c)) q.push(c);
    }
    for (auto c : reach)
        if (none_terminate.count(c))
            nt1.insert(c);
        else
            t1.insert(c);

    none_terminate = nt1;
    terminate = t1;

    p1.clear();
    for (auto nt : none_terminate) {
        for (auto pi : produce[nt]) {
            bool good = true;
            for (auto c : pi)
                if (!terminate.count(c) && !none_terminate.count(c)) {
                    good = false;
                    break;
                }
            if (good) p1[nt].insert(pi);
        }
    }

    produce = p1;
    if(starter == NEW_STARTER)
        produce[starter].insert("#");
}

void debug_info(const std::string prompt) {
    // std::cout << "********DEBUG_INFO********" << std::endl;
    std::cout << prompt << std::endl;
    std::cout << "N={";
    for (auto i = none_terminate.begin(); i != none_terminate.end();) {
        std::cout << *i;
        if (++i != none_terminate.end()) std::cout << ',';
    }
    std::cout << '}' << std::endl;

    std::cout << "T={";
    for (auto i = terminate.begin(); i != terminate.end();) {
        std::cout << *i;
        if (++i != terminate.end()) std::cout << ',';
    }
    std::cout << '}' << std::endl;

    std::cout << "P:" << std::endl;
    for (auto p : produce) {
        std::cout << "  ";
        std::cout << p.first;
        std::cout << " -> ";
        for (auto i = p.second.begin(); i != p.second.end();) {
            std::cout << *i;
            if (++i != p.second.end()) std::cout << " | ";
        }
        std::cout << std::endl;
    }

    std::cout << "S:" << starter << std::endl;

    // std::cout << "******DEBUG_INFO END******" << std::endl;
}
