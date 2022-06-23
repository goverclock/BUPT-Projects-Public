#pragma once
#include <cstdlib>
#include <map>
#include <string>

#include "cmd.h"

#define MAX_ARGS 16

enum user_type { admin, user, courier, guest };
enum express_type { fragile, book, average };
// pending_assign -> pending_delivery -> pending_receive -> received
enum express_status {
    received,
    pending_receive,
    pending_assign,
    pending_delivery
};
typedef void (Command::*f)(std::string[]);

struct Filter {
    std::string id;
    std::string snd_user;
    std::string rec_user;
    std::string stime;
    std::string rtime;
    int sta = -1;
};
