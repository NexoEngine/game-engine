    //// Exception.hpp ////////////////////////////////////////////////////////////
    //
    //  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
    //  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
    //  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
    //  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
    //  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
    //
    //  Author:      Mehdy MORVAN
    //  Date:        12/11/2024
    //  Description: Common header file for the nexo base exception class
    //
    ///////////////////////////////////////////////////////////////////////////////
    #pragma once

    #include <string>

    namespace nexo {
        class Exception : public std::exception {
            public:
                explicit Exception(std::string message, const char *file = __FILE__, const int line = __LINE__)
                    : message(std::move(message)), file(file), line(line) {}

                const char *what() const noexcept override;

                const std::string &getMessage() const noexcept { return message; }
                const char *getFile() const noexcept { return file; }
                int getLine() const noexcept { return line; }

                protected:
                std::string formatMessage() const;

            private:
                std::string message;
                const char *file;
                int line;
                mutable std::string formattedMessage;
        };
    }

#define THROW_EXCEPTION(ExceptionType, ...) \
throw ExceptionType(__VA_OPT__(__VA_ARGS__,) __FILE__, __LINE__)

