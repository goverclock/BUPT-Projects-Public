#include "express.h"

#include <fstream>

extern ToolLib tl;

// create a new express
// ask for type before this
Express::Express(Account* sender_account) {
    sender = sender_account->nickname;
    status = pending_assign;

    std::cout << "to user: ";
    std::string t;
    std::cin >> t;
    while (!tl.is_account_exist(t.c_str())) {
        std::cout << "no such user" << std::endl;
        std::cout << "to user: ";
        std::cin >> t;
    }
    receiver = t;

    std::cout << "weight:";
    std::cin >> t;
    weight = atoi(t.c_str());

    std::cout << "description: ";
    std::cin >> description;
    send_time = tl.current_time();
    receive_time = "null\t\t";
    ID = generate_ID();

    save_to_file();

    // eat empty line
    getline(std::cin, t);
}

// read from filr for an existing express
Express::Express(std::string path_id) {
    ID = path_id.substr(path_id.find_last_of("/") + 1);

    std::ifstream expfile(path_id);
    std::string line;
    getline(expfile, line);
    type = (express_type)atoi(line.c_str());
    getline(expfile, line);
    weight = atoi(line.c_str());
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
    getline(expfile, line);
    courier_nick = line;

    expfile.close();
}

// argument: filter mask
void Express::ls_info(Account* cmd_user, Filter ftr) {
    // cmd user can only see express about himself(as sender or receiver)
    // except for admin
    bool good = false;
    if (cmd_user->type == admin) good = true;
    if (cmd_user->nickname == sender || cmd_user->nickname == receiver)
        good = true;
    if (cmd_user->nickname == courier_nick && status == pending_delivery)
        good = true;
    if (!good) return;

    // if (cmd_user->type != admin && cmd_user->nickname != sender &&
    //     cmd_user->nickname != receiver && cmd_user->nickname != courier_nick)
    //     return;

    // if (cmd_user->type != admin && cmd_user->nickname != sender &&
    //     cmd_user->nickname != receiver && status != pending_delivery)
    //     return;

    // filter
    if (!ftr.id.empty() && ftr.id != ID) return;
    if (!ftr.snd_user.empty() && ftr.snd_user != sender) return;
    if (!ftr.rec_user.empty() && ftr.rec_user != receiver) return;
    if (!ftr.stime.empty() && ftr.stime != send_time) return;
    if (!ftr.rtime.empty() && ftr.rtime != receive_time) return;
    if (ftr.sta != -1 && ftr.sta != status) return;

    std::string st = "unknown";
    if (status == received)
        st = "received";
    else if (status == pending_delivery) {
        st = "pending delivery";
        if (!courier_nick.empty())
            if (courier_nick == cmd_user->nickname)
                st += "(courier: YOU)";
            else
                st += "(courier: " + courier_nick + ")";
    } else if (status == pending_receive)
        st = "pending receive";
    else if (status == pending_assign)
        st = "pending assign";
    std::cout << ID << '\t' << sender << "\t\t" << receiver << "\t\t"
              << send_time << "\t\t" << receive_time << "\t\t" << st
              << std::endl;
}

void Express::accept() {
    if (status == received) {
        std::cout << ID << ": has alreay been received" << std::endl;
        return;
    } else if (status == pending_delivery) {
        std::cout << ID << ": hasn't been deliveried yet" << std::endl;
        return;
    } else if (status == pending_assign) {
        std::cout << ID << ": hasn't been assigned to a courier yet"
                  << std::endl;
        return;
    }

    // status == pending_receive here
    std::cout << ID << ": successfully accepted" << std::endl;
    status = received;
    receive_time = tl.current_time();
    save_to_file();
}

// doesn't change balance
// return true if succed
bool Express::assign_to(std::string c_n) {
    if (status != pending_assign) {
        std::cout << ID << ": need not to be assigned now" << std::endl;
        return false;
    }
    status = pending_delivery;
    courier_nick = c_n;
    save_to_file();
    std::cout << "successfully assigned " << ID << " to " << courier_nick
              << std::endl;
    return true;
}

void Express::delivery() {
    if (status != pending_delivery) {
        std::cout << ID << ": need not to be deliveried now" << std::endl;
        return;
    }
    status = pending_receive;
    save_to_file();
    std::cout << "successfully deliveried " << ID << std::endl;
}

std::string Express::generate_ID() {
    std::string ct = tl.current_time();
    int tm[6];
    sscanf(ct.c_str(), "%d-%d-%d.%d:%d:%d", &tm[0], &tm[1], &tm[2], &tm[3],
           &tm[4], &tm[5]);
    char buf[64];
    sprintf(buf, "%x%x%x%x%x%x", tm[0], tm[1], tm[2], tm[3], tm[4], tm[5]);
    return buf;
}

void Express::save_to_file() {
    std::string fname = tl.cwd() + "/express/" + ID;
    std::ofstream outfile(fname);
    outfile << type << std::endl;
    outfile << weight << std::endl;
    outfile << sender << std::endl;
    outfile << receiver << std::endl;
    outfile << status << std::endl;
    outfile << send_time << std::endl;
    outfile << receive_time << std::endl;
    outfile << description << std::endl;
    outfile << courier_nick << std::endl;
}
