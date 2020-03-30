#ifndef DGT_COMMON_LOG_H
#define DGT_COMMON_LOG_H

#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <type_traits>

#define RST "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST

#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST

#define __FILENAME__ (strrchr(__FILE__, '/') ? std::strrchr(__FILE__, '/') + 1 : __FILE__)

#define DGT_LOG_INFO(...) Logger::GetInstance().Log(std::cout, syscall(__NR_gettid), BOLD(FCYN("INFO")), __VA_ARGS__)

#define DGT_LOG_WARNING(...) Logger::GetInstance().Log(std::cout, syscall(__NR_gettid), BOLD(FYEL("WARNING")), __VA_ARGS__)

#define DGT_LOG_ERROR(...)                                                           \
    Logger::GetInstance().LogD(std::cout, syscall(__NR_gettid), BOLD(FRED("ERROR")), \
                               __FILENAME__, __LINE__, __FUNCTION__, __VA_ARGS__);

#define DGT_LOG_DEBUG(...)                                                               \
    if (Logger::GetInstance().GetDebugActive()) {                                        \
        Logger::GetInstance().LogD(std::cout, syscall(__NR_gettid), BOLD(FGRN("DEBUG")), \
                                   __FILENAME__, __LINE__, __FUNCTION__, __VA_ARGS__);   \
    }

class Logger {
   public:
    static Logger& GetInstance() {
        static Logger instance;
        return instance;
    }

    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;

    void SetDebugActive(bool active) {
        m_debug_active = active;
    }
    bool GetDebugActive() const {
        return (m_debug_active);
    }

    void SetIdentifier(const std::string& identifier) {
        m_identifier = identifier;
    }

    const std::string GetIdentifier() const {
        return m_identifier;
    }

    template <typename... Args>
    void LogD(std::ostream& out, pid_t pid, const char* type, const char* file_name, int line, const char* function_name, Args const&... args) {
        out << GetLogTime(std::chrono::system_clock::now()) << " " << pid << " ";
        if (!m_identifier.empty()) {
            out << m_identifier << " ";
        }
        out << type << KGRN << " [" << file_name << ":" << line << " " << function_name << "()]" << RST << " ";
        Write(out, args...);
    }

    template <typename... Args>
    void Log(std::ostream& out, pid_t pid, const char* type, Args const&... args) {
        out << GetLogTime(std::chrono::system_clock::now()) << " " << pid << " ";
        if (!m_identifier.empty()) {
            out << m_identifier << " ";
        }
        out << type << " ";
        Write(out, args...);
    }

    template <typename First, typename... Args>
    void Write(std::ostream& out, First const& first, Args const&... args) {
        out << first;
        Write(out, args...);
    }

    template <typename Last>
    void Write(std::ostream& out, Last const& last) {
        out << last;
        WriteEnd(out);
    }

    void WriteEnd(std::ostream& out) {
        out << std::endl;
    }

   private:
    bool m_debug_active{true};
    std::string m_identifier;

    Logger(){};

    std::string GetLogTime(std::chrono::time_point<std::chrono::system_clock> time) {
        auto epoch_seconds = std::chrono::system_clock::to_time_t(time);
        std::stringstream stream;
        stream << std::put_time(gmtime(&epoch_seconds), "%F %T");
        auto truncated = std::chrono::system_clock::from_time_t(epoch_seconds);
        auto delta_us = std::chrono::duration_cast<std::chrono::microseconds>(time - truncated).count();
        stream << "." << std::fixed << std::setw(6) << std::setfill('0') << delta_us;
        return stream.str();
    }
};

#endif  // DGT_COMMON_LOG_H