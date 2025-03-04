#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "database.h"
#include <functional>
#include <map>
#include <string>
#include <vector>

class CommandHandler
{
public:
    CommandHandler(Database &db);

    void handleCommand(const std::string &userCommand);

private:
    Database &db;
    std::map<std::string, std::function<void(const std::string &, const std::string &)>> actions;
    std::vector<std::string> stopWords;

    std::vector<std::string> splitText(const std::string &text);
    std::string findBestMatch(const std::string &input, const std::vector<std::string> &keywords);
    std::tuple<std::string, std::string, std::string> extractCommand(const std::string &userCommand);
    void openLink(const std::string &keyword);
    void searchInLink(const std::string &keyword, const std::string &query);
    void executeCommand(const std::string &command);
};

#endif // COMMANDHANDLER_H
