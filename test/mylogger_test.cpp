//
// Created by shaoliming on 19-9-16.
//

#include "log.h"

using namespace slam;
int main(){
    auto p = myLogger::getLogger();
    p->info("shaoliming");
    p->info("shaoliming");
    p->debugf("ERROR - %s", "exhaused memory");

    // TODO: it's not good to delete the singleton manually
    p->finish();
    return 0;
}
