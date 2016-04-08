#include <Exception.h>

namespace imbo5{

Exception::Exception(const std::string& msg)
:   _message(msg)
{}

const char* Exception::what() const noexcept 
{
    return _message.c_str();
}

}
