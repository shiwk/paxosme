//
// Created by shiwk on 2021/3/6.
//

#ifndef PAXOSME_DATABASE_HPP
#define PAXOSME_DATABASE_HPP

#include <string>

class Database {
    int Get(const std::string &key, std::string & sValue);

    int Put(const std::string &key, const std::string & sValue);
};


#endif //PAXOSME_DATABASE_HPP
