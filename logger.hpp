# ifndef   LOGGER_HPP
# define   LOGGER_HPP

# include <iostream>

class Logger
{
public:
    static void Log(LogLevel logLevel, const std::string& message)
    {
        switch (logLevel)
        {
        case LogLevel::INFO:
            std::cout << "[INFO]: " << message << std::endl;
            break;
        case LogLevel::WARNING:
            std::cout << "[WARNING]: " << message << std::endl;
            break;
        case LogLevel::ERROR:
            std::cout << "[ERROR]: " << message << std::endl;
            break;
        default:
            break;
        }
    }
};

# endif