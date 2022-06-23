#include "cmd.h"

#include <filesystem>
#include <fstream>

#include "admin.h"
#include "express.h"
#include "user.h"

namespace fs = std::filesystem;

extern Account* current_account;

// cmds

// note: admin is register manually through editing a file
void reg(std::string args[]) {
    // pre requires
    if (current_account != nullptr) {
        std::cout << "you must be a guest to register" << std::endl;
        return;
    }
    // syntax
    if (args[1] != "user") {
        panic("usage: reg <user>");
        return;
    }

    // success or die
    if (args[1] == "user") new User();
    std::cout << "register successfully" << std::endl;
}

void log(std::string args[]) {
    // pre requires
    if (current_account != nullptr) {
        std::cout << "you've already logged" << std::endl;
        return;
    }

    std::string fname = cwd() + "/user/" + args[1];
    if (!is_file_exist(fname.c_str())) {
        std::cout << "no such user" << std::endl;
        return;
    }
    // syntax
    if (args[1].empty()) {
        panic("usage: log <username>");
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
    std::cout << "welcome, " << ut + " " << args[1] << std::endl;
    if (usertype == power::user)
        current_account = new User(args[1]);
    else if (usertype == power::admin)
        current_account = new Admin(args[1]);
}

void passwd(std::string args[]) {
    // pre requires
    if (current_account == nullptr) {
        std::cout << "you need to log in first" << std::endl;
        return;
    }

    current_account->passwd(args);
}

void balance(std::string args[]) {
    // pre requires
    if (current_account == nullptr) {
        std::cout << "you need to log in first" << std::endl;
        return;
    }

    current_account->charge(args);
}

void logout(std::string args[]) {
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

void exp(std::string args[]) {
    // pre requires
    if (current_account == nullptr) {
        std::cout << "you need to log in first" << std::endl;
        return;
    }

    if (args[1] == "send") {
        new Express(current_account);
        current_account->change_balance(-EXP_CHARGE);
        add_balance_to("admin", EXP_CHARGE);
        std::cout << "express sent successfully" << std::endl;
        std::cout << "balance -" << EXP_CHARGE
                  << " ,remaining: " << current_account->balance << std::endl;
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

        std::string wd = cwd() + "/express/";
        std::cout << "ID\t\tfrom\t\tto\t\tsend time\t\trecv time\t\tstatus"
                  << std::endl;
        for (const auto& entry : fs::directory_iterator(wd)) {
            std::string entry_path = entry.path();
            Express exp(entry_path);
            exp.ls_info(current_account, ftr);
        }
    } else if (args[1] == "accept") {
        int ind = 2;
        while (!args[ind].empty() && ind < MAX_ARGS) {
            if (is_express_exist(args[ind].c_str())) {
                std::string fpath = cwd() + "/express/" + args[ind];
                Express exp(fpath);
                if (exp.receiver == current_account->nickname) {
                    exp.accept();
                } else
                    std::cout << args[ind] << ": is not to you" << std::endl;

            } else
                std::cout << args[ind] << ": no such express" << std::endl;
            ind++;
        }

    } else
        panic("usage: exp send/ls [filter key]/accept <ID>");
}
