#ifndef BLOCK_MATRIX_MULTIPLICATION_LOGGING_H
#define BLOCK_MATRIX_MULTIPLICATION_LOGGING_H

#include <iostream>
#include <string>
#include <list>
#include <mutex>

enum LoggingType{
    CRITICAL,
    ERROR,
    INFO,
    DEBUG
};

std::string logtype_to_str(LoggingType);

class Logger{
private:
    std::list<std::pair<LoggingType, std::string>> log_list;
    std::ostream& out;
    LoggingType mode;
    std::string name;
    std::mutex mut;
    bool save;

    void add_log_line(LoggingType, const std::string&);
    void show_log(LoggingType, const std::string&);
public:
    Logger();
    explicit Logger(bool);
    explicit Logger(std::ostream&, bool);
    explicit Logger(std::ostream&, const std::string&, bool);

    ~Logger();

    void set_mode(LoggingType);
    void set_name(const std::string&);

    void info(const std::string&);
    void debug(const std::string&);
    void error(const std::string&);
    void critical(const std::string&);

    const std::list<std::pair<LoggingType, std::string>> & get_logging_list();
};

#endif //BLOCK_MATRIX_MULTIPLICATION_LOGGING_H
