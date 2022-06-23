#pragma once
#include <map>
#include <string>
#include <cstdlib>

#define MAX_ARGS 16
#define EXP_CHARGE 15

enum power {admin, user, guest};
enum express_status {received, pending_receive};
typedef void (*f)(std::string[]);

struct Filter {
    std::string id;
    std::string snd_user;
    std::string rec_user;
    std::string stime;
    std::string rtime;
    int sta = -1;
};
