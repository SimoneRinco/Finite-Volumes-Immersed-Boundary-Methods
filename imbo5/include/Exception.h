#pragma once
#include <exception>
#include <string>

namespace imbo5{

class Exception : public std::exception
{
public:

    Exception(const std::string& msg);

    const char* what() const noexcept override;

private:
    std::string _message;
};

}
