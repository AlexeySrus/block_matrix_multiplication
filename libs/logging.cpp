#include "logging.h"

using namespace std;


std::string logtype_to_str(LoggingType logt){
    std::string res;

    switch (logt){
        case INFO: {
            res = "INFO";
            break;
        }
        case DEBUG: {
            res = "DEBUG";
            break;
        }
        case ERROR: {
            res = "ERROR";
        }
        case CRITICAL: {
            res = "CRITICAL";
        }
        default: res = "";
    }

    return res;
}


Logger::Logger(bool saving) : out(cout), mode(INFO), save(saving) {}


Logger::Logger(std::ostream& os, bool saving) : out(os), mode(INFO), save(saving) {}


Logger::Logger(std::ostream & os, const std::string & _name, bool saving) :
    out(os), mode(INFO), save(saving), name(_name + ":") {}

Logger::~Logger() {
    this->log_list.clear();
}


void Logger::set_mode(LoggingType mode) {
    this->mode = mode;
}


void Logger::set_name(const std::string & _name) {
    this->name = _name + ":";
}


void Logger::add_log_line(LoggingType logt, const std::string & s) {
    if (this->save) {
        this->mut.lock();
        this->log_list.emplace_back(make_pair(logt, this->name + s));
        this->mut.unlock();
    }
}


void Logger::show_log(LoggingType logt, const std::string & s) {
    this->mut.lock();
    if (logt <= this->mode)
        this->out << this->name + logtype_to_str(logt) + ":" + s;
    this->mut.unlock();
}


void Logger::info(const std::string & s) {
    this->add_log_line(INFO, s);
    this->show_log(INFO, s);
}


void Logger::debug(const std::string & s) {
    this->add_log_line(DEBUG, s);
    this->show_log(DEBUG, s);
}


void Logger::error(const std::string & s) {
    this->add_log_line(ERROR, s);
    this->show_log(ERROR, s);
}


void Logger::critical(const std::string & s) {
    this->add_log_line(CRITICAL, s);
    this->show_log(CRITICAL, s);
}

const std::list<std::pair<LoggingType, std::string>> & Logger::get_logging_list() {
    return this->log_list;
}
