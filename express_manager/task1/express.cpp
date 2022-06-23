#include "express.h"

#include <fstream>

Express::Express(Account* sender_account) {
    sender = sender_account->nickname;
    status = pending_receive;

    std::cout << "to user: ";
    std::string t;
    std::cin >> t;
    while (!is_account_exist(t.c_str())) {
        std::cout << "no such user" << std::endl;
        std::cout << "to user: ";
        std::cin >> t;
    }
    receiver = t;

    std::cout << "description: ";
    std::cin >> description;
    send_time = current_time();
    receive_time = "null";
    ID = generate_ID();

    save_to_file();

    // eat empty line
    getline(std::cin, t);
}

Express::Express(std::string id) {
    ID = id.substr(id.find_last_of("/") + 1);

    std::ifstream expfile(id);
    std::string line;
    getline(expfile, line);
    sender = line;
    getline(expfile, line);
    receiver = line;
    getline(expfile, line);
    status = (express_status)atoi(line.c_str());
    getline(expfile, line);
    send_time = line;
    getline(expfile, line);
    receive_time = line;
    getline(expfile, line);
    description = line;

    expfile.close();
}

// argument: filter mask
void Express::ls_info(Account* cmd_user, Filter ftr) {
    // cmd user can only see express about himself(as sender or receiver)
    // except for admin
    if (cmd_user->type != admin && cmd_user->nickname != sender &&
        cmd_user->nickname != receiver)
        return;
    // filter
    if (!ftr.id.empty() && ftr.id != ID) return;
    if (!ftr.snd_user.empty() && ftr.snd_user != sender) return;
    if (!ftr.rec_user.empty() && ftr.rec_user != receiver) return;
    if (!ftr.stime.empty() && ftr.stime != send_time) return;
    if (!ftr.rtime.empty() && ftr.rtime != receive_time) return;
    if (ftr.sta != -1 && ftr.sta != status) return;

    std::cout << ID << '\t' << sender << "\t\t" << receiver << "\t\t"
              << send_time << "\t\t" << receive_time << "\t\t" << status
              << std::endl;
}

void Express::accept() {
    if (status == received) {
        std::cout << ID << ": has alreay been received" << std::endl;
        return;
    }
    std::cout << ID << ": successfully accepted" << std::endl;
    status = received;
    receive_time = current_time();
    save_to_file();
}

std::string Express::generate_ID() {
    std::string ct = current_time();
    int tm[6];
    sscanf(ct.c_str(), "%d-%d-%d.%d:%d:%d", &tm[0], &tm[1], &tm[2], &tm[3],
           &tm[4], &tm[5]);
    char buf[64];
    sprintf(buf, "%x%x%x%x%x%x", tm[0], tm[1], tm[2], tm[3], tm[4], tm[5]);
    return buf;
}

void Express::save_to_file() {
    // save to file cwd/user/nickname
    // format: see handin/report.md
    std::string fname = cwd() + "/express/" + ID;
    std::ofstream outfile(fname);
    outfile << sender << std::endl;        // sender
    outfile << receiver << std::endl;      // receiver
    outfile << status << std::endl;        // status
    outfile << send_time << std::endl;     // send_time
    outfile << receive_time << std::endl;  // receive_time
    outfile << description << std::endl;   // description
    outfile.close();
}