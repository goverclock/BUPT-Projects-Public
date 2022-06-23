#include "cmd.h"

#include <filesystem>
#include <fstream>

#include "admin.h"
#include "average.h"
#include "book.h"
#include "courier.h"
#include "fragile.h"
#include "user.h"

namespace fs = std::filesystem;

extern Account* current_account;
extern ToolLib tl;

// cmds

// note: admin is register manually through editing a file
void Command::reg(std::string args[]) {
    // pre requires
    if (current_account != nullptr) {
        std::cout << "you must be a guest to register" << std::endl;
        return;
    }

    // success or die
    if (args[1] == "user")
        new User();
    else if (args[1] == "courier")
        new Courier();
    else {
        tl.panic("usage: reg <user/courier>");
        return;
    }
    std::cout << "register successfully" << std::endl;
}

void Command::log(std::string args[]) {
    // pre requires
    if (current_account != nullptr) {
        std::cout << "you've already logged" << std::endl;
        return;
    }

    std::string fname = tl.cwd() + "/user/" + args[1];
    if (!tl.is_file_exist(fname.c_str())) {
        std::cout << "no such user" << std::endl;
        return;
    }
    // syntax
    if (args[1].empty()) {
        tl.panic("usage: log <username>");
        return;
    }

    // read from file for password
    std::string pd;  // real password
    std::ifstream userfile(fname);
    getline(userfile, pd);
    int usertype = atoi(pd.c_str());  // user type
    getline(userfile, pd);
    userfile.close();
    std::cout << "password for " << args[1] << ":";
    std::string in;  // entered password
    std::cin >> in;
    // eat empty line
    std::string t;
    getline(std::cin, t);

    if (pd != in) {
        std::cout << "wrong password" << std::endl;
        return;
    }

    std::string ut;
    if (usertype == user)
        ut = "user";
    else if (usertype == admin)
        ut = "adminstrator";
    else if (usertype == courier)
        ut = "courier";
    std::cout << "welcome, " << ut + " " << args[1] << std::endl;
    if (usertype == user)
        current_account = new User(args[1]);
    else if (usertype == admin)
        current_account = new Admin(args[1]);
    else if (usertype == courier)
        current_account = new Courier(args[1]);
}

void Command::passwd(std::string args[]) {
    // pre requires
    if (current_account == nullptr) {
        std::cout << "you need to log in first" << std::endl;
        return;
    }

    current_account->passwd(args);
}

void Command::balance(std::string args[]) {
    // pre requires
    if (current_account == nullptr) {
        std::cout << "you need to log in first" << std::endl;
        return;
    }

    current_account->charge(args);
}

void Command::logout(std::string args[]) {
    // pre requires
    if (current_account == nullptr) {
        std::cout << "you've already logged out" << std::endl;
        return;
    } else {
        delete current_account;
        current_account = nullptr;
        std::cout << "you've logged out" << std::endl;
    }
}

void Command::exp(std::string args[]) {
    // pre requires
    if (current_account == nullptr) {
        std::cout << "you need to log in first" << std::endl;
        return;
    }

    if (args[1] == "send") {
        std::cout << "type: ";
        std::string in;
        Express* exp;
        while (1) {
            std::cin >> in;
            if (in == "fragile") {
                exp = new Fragile(current_account);
                break;
            } else if (in == "book") {
                exp = new Book(current_account);
                break;
            } else if (in == "average") {
                exp = new Average(current_account);
                break;
            }
            std::cout << "no such type" << std::endl;
            std::cout << "type: ";
        }

        int fee = exp->getPrice();
        if (!current_account->change_balance(-fee, true)) {
            std::cout << "failed to send express, need more balance"
                      << std::endl;
            tl.rm_express(exp->ID.c_str());
            return;
        }
        Admin adm("admin");
        adm.change_balance(fee, false);
        std::cout << "express sent successfully" << std::endl;
    } else if (args[1] == "ls") {
        Filter ftr;
        for (int i = 2; !args[i].empty() && i < MAX_ARGS; i++) {
            if (args[i] == "-ID") ftr.id = args[++i];
            if (args[i] == "-from") ftr.snd_user = args[++i];
            if (args[i] == "-to") ftr.rec_user = args[++i];
            if (args[i] == "-stime") ftr.stime = args[++i];
            if (args[i] == "-rtime") ftr.rtime = args[++i];
            if (args[i] == "-sta") ftr.sta = atoi(args[++i].c_str());
        }

        std::string wd = tl.cwd() + "/express/";
        std::cout << "ID\t\tfrom\t\tto\t\tsend time\t\t\trecv time\t\t\tstatus"
                  << std::endl;
        for (const auto& entry : fs::directory_iterator(wd)) {
            std::string entry_path = entry.path();
            // type doesn't matter here
            Fragile exp(entry_path);
            exp.ls_info(current_account, ftr);
        }
    } else if (args[1] == "accept") {
        int ind = 2;
        while (!args[ind].empty() && ind < MAX_ARGS) {
            if (tl.is_express_exist(args[ind].c_str())) {
                std::string fpath = tl.cwd() + "/express/" + args[ind];
                // type doesn't matter here
                Fragile exp(fpath);
                if (exp.receiver == current_account->nickname)
                    exp.accept();
                else
                    std::cout << args[ind] << ": is not to you" << std::endl;

            } else
                std::cout << args[ind] << ": no such express" << std::endl;
            ind++;
        }
    } else if (args[1] == "assign") {
        if (current_account->type != admin) {
            std::cout << "this command is for adminstrator" << std::endl;
            return;
        }
        if (args[2].empty() || args[3].empty()) {
            std::cout << "usage: exp assign <express ID> <courier nick>"
                      << std::endl;
            return;
        }

        std::string exp_ID = args[2];
        std::string cou_nick = args[3];
        if (!tl.is_express_exist(exp_ID.c_str())) {
            std::cout << "no such express" << std::endl;
            return;
        }
        if (!tl.is_account_exist(cou_nick.c_str())) {
            std::cout << "no such courier" << std::endl;
            return;
        }
        if (tl.account_type(cou_nick.c_str()) != user_type::courier) {
            std::cout << cou_nick << ": not a courier" << std::endl;
            return;
        }
        
        // balance: admin to courier
        express_type t = (express_type)tl.exp_type(exp_ID);
        Express *exp;
        std::string fpath = tl.cwd() + "/express/" + exp_ID;
        if(t == fragile) exp = new Fragile(fpath);
        else if(t == book) exp = new Book(fpath);
        else if(t == average) exp = new Average(fpath);
        int fee = exp->getPrice() / 2;
        if (!current_account->change_balance(-fee, true)) {
            std::cout << "failed to assign express, need more balance"
                      << std::endl;
            return;
        }
        if (!exp->assign_to(cou_nick)) {
            current_account->change_balance(fee, false);
            return;
        }
        delete exp;
        Courier cou(cou_nick);
        cou.change_balance(fee, false);
    } else if (args[1] == "dlry") {
        if (current_account->type != courier) {
            std::cout << "this command is for courier" << std::endl;
            return;
        }

        int ind = 2;
        while (!args[ind].empty()) {
            std::string exp_ID = args[ind];
            if (!tl.is_express_exist(exp_ID.c_str())) {
                std::cout << "no such express" << std::endl;
                return;
            }
            // type doesn't matter here
            Fragile exp(tl.cwd() + "/express/" + exp_ID);
            exp.delivery();
            ind++;
        }

    } else
        tl.panic("usage: exp send/ls [filter key]/accept <ID>");
}

void Command::accmgr(std::string args[]) {
    // pre requires
    if (current_account == nullptr) {
        std::cout << "you need to log in first" << std::endl;
        return;
    }
    if (current_account->type != admin) {
        std::cout << "this command is for administrator" << std::endl;
        return;
    }

    if (args[1] == "rm") {
        if (!tl.is_account_exist(args[2].c_str())) {
            std::cout << "no such accoune" << std::endl;
            return;
        } else {
            std::string fpath = tl.cwd() + "/user/" + args[2];
            remove(fpath.c_str());
            std::cout << "deleted account: " + args[2] << std::endl;
        }
    } else
        std::cout << "usage: coumgr rm <nick>" << std::endl;
}
