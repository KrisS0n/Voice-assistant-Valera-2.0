#include "database.h"
#include <iostream>

Database::Database(const std::string &dbName)
{
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK)
    {
        std::cerr << "Ошибка открытия базы данных: " << sqlite3_errmsg(db) << "\n";
    }
}

Database::~Database()
{
    sqlite3_close(db);
}

bool Database::getLinkData(const std::string &keyword, std::string &openLink, std::string &searchInLink)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT open_link, search_in_link FROM links WHERE keyword = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, keyword.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        openLink = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        searchInLink = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        sqlite3_finalize(stmt);
        return true;
    }

    sqlite3_finalize(stmt);
    return false;
}

std::string Database::getProgramCommand(const std::string &keyword)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT command FROM programs WHERE keyword = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << std::endl;
        return "";
    }

    sqlite3_bind_text(stmt, 1, keyword.c_str(), -1, SQLITE_STATIC);
    std::string command;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        command = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
    }
    else
    {
        std::cerr << "Программа не найдена: " << keyword << std::endl;
    }

    sqlite3_finalize(stmt);
    return command;
}

std::vector<std::string> Database::getLogicKeywords()
{
    std::vector<std::string> keywords;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT keyword FROM logic;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            keywords.push_back(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
        }
        sqlite3_finalize(stmt);
    }

    return keywords;
}

std::vector<std::string> Database::getAllKeywords()
{
    std::vector<std::string> keywords;
    sqlite3_stmt *stmt;

    const char *sqlLinks = "SELECT keyword FROM links;";
    if (sqlite3_prepare_v2(db, sqlLinks, -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            keywords.push_back(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
        }
        sqlite3_finalize(stmt);
    }

    const char *sqlPrograms = "SELECT keyword FROM programs;";
    if (sqlite3_prepare_v2(db, sqlPrograms, -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            keywords.push_back(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
        }
        sqlite3_finalize(stmt);
    }

    return keywords;
}
