#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include <vector>

class Database
{
public:
    Database(const std::string &dbName);
    ~Database();

    bool getLinkData(const std::string &keyword, std::string &openLink, std::string &searchInLink);
    std::string getProgramCommand(const std::string &keyword);
    std::vector<std::string> getLogicKeywords();
    std::vector<std::string> getAllKeywords();

private:
    sqlite3 *db;
};

#endif // DATABASE_H
