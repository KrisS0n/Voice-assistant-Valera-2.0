#include <iostream>
#include <sstream>

#include "commandhandler.h"
#include "constants.h"
#include "levenshtein.h"

CommandHandler::CommandHandler(Database &db) : db(db)
{
    actions = {
        {"открыть", [this](const std::string &keyword, const std::string &query = "") { this->openLink(keyword); }},
        {"найти", [this](const std::string &keyword, const std::string &query) { this->searchInLink(keyword, query); }},
        {"найди",
         [this](const std::string &keyword, const std::string &query) { this->searchInLink(keyword, query); }}};

    stopWords = {"в", "на", "по", "за", "под", "от"};
}

void CommandHandler::handleCommand(const std::string &userCommand)
{
    auto [action, keyword, query] = extractCommand(userCommand);

    if (keyword.empty())
    {
        std::cerr << "Не удалось определить ключевое слово для команды." << std::endl;
        return;
    }

    auto it = actions.find(action);
    if (it != actions.end())
    {
        it->second(keyword, query);
    }
    else
    {
        std::cerr << "Неизвестная команда: " << action << std::endl;
    }
}

std::vector<std::string> CommandHandler::splitText(const std::string &text)
{
    std::vector<std::string> words;
    std::istringstream iss(text);
    std::string word;
    while (iss >> word)
    {
        if (!word.empty())
        {
            words.push_back(word);
        }
    }
    return words;
}

std::string CommandHandler::findBestMatch(const std::string &input, const std::vector<std::string> &keywords)
{
    std::string bestMatch;
    double bestPercentage = 0.0;

    for (const auto &keyword : keywords)
    {
        double percentage = calculateMatchPercentage(input, keyword);
        if (percentage > bestPercentage && percentage >= 50.0)
        {
            bestPercentage = percentage;
            bestMatch = keyword;
        }
    }

    if (!bestMatch.empty())
    {
        std::cout << "Найдено совпадение: " << bestMatch << " (совпадение: " << bestPercentage << "%)" << std::endl;
    }

    return bestMatch;
}

std::tuple<std::string, std::string, std::string> CommandHandler::extractCommand(const std::string &userCommand)
{
    std::vector<std::string> words = splitText(userCommand);

    std::vector<std::string> logicKeywords = db.getLogicKeywords();

    std::string command;
    std::string keyword;
    std::string query;

    for (int i = words.size() - 1; i >= 0; --i)
    {
        std::string bestLogicMatch = findBestMatch(words[i], logicKeywords);
        if (!bestLogicMatch.empty())
        {
            command = bestLogicMatch;

            if (i < words.size() - 1)
            {
                std::vector<std::string> allKeywords = db.getAllKeywords();
                keyword = findBestMatch(words[i + 1], allKeywords);
            }

            for (int j = i + 2; j < words.size(); ++j)
            {
                query += words[j] + " ";
            }
            if (!query.empty())
            {
                query.pop_back();
            }
            break;
        }
    }

    if (keyword.empty() && !query.empty())
    {
        std::vector<std::string> allKeywords = db.getAllKeywords();
        std::vector<std::string> queryWords = splitText(query);
        for (size_t i = 0; i < queryWords.size(); ++i)
        {
            std::string potentialKeyword = findBestMatch(queryWords[i], allKeywords);
            if (!potentialKeyword.empty())
            {
                keyword = potentialKeyword;
                queryWords.erase(queryWords.begin() + i);
                query.clear();
                for (const auto &word : queryWords)
                {
                    query += word + " ";
                }
                if (!query.empty())
                {
                    query.pop_back();
                }
                break;
            }
        }
    }

    if (command.empty())
    {
        std::vector<std::string> allKeywords = db.getAllKeywords();
        keyword = findBestMatch(words[0], allKeywords);
        command = "открыть";
    }

    return {command, keyword, query};
}

void CommandHandler::openLink(const std::string &keyword)
{
    std::string openLink, searchInLink;
    if (db.getLinkData(keyword, openLink, searchInLink))
    {
        executeCommand(openLink);
    }
    else
    {
        std::string command = db.getProgramCommand(keyword);
        if (!command.empty())
        {
            executeCommand(command);
        }
    }
}

void CommandHandler::searchInLink(const std::string &keyword, const std::string &query)
{
    std::string openLink, searchInLink;
    if (db.getLinkData(keyword, openLink, searchInLink))
    {
        std::string url = searchInLink + query;
        executeCommand(url);
    }
}

void CommandHandler::executeCommand(const std::string &command)
{
    if (command.substr(0, 5) == "https")
    {
        std::string fullCommand = XDG + "'" + command + "'";
        system(fullCommand.c_str());
    }
    else
    {
        system((command).c_str());
    }
}
