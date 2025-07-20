#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

const char* get_home_directory() {
    const char* homedir = getenv("HOME");
    if (homedir == NULL) {
        homedir = getenv("USERPROFILE");
    }
    return homedir;
}