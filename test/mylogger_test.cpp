//
// Created by shaoliming on 19-9-16.
//

#include "log.h"

using namespace slam;
int main(){
    auto p = myLogger::getLogger();
    p->info("shaoliming");
    p->info("shaoliming");
    p->debugf("error  - %s", "shaoliming");
    delete p;
    return 0;
}