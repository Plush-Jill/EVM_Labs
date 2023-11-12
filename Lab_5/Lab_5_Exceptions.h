#include <exception>
#include <string>
#include <utility>


class Lab_5_Exceptions : public std::exception{
private:
    std::string message_;
public:
    ~Lab_5_Exceptions() override = default;
    explicit Lab_5_Exceptions(std::string  message) : message_(std::move(message)){}
    virtual const char* what() const throw(){
        return message_.c_str();
    }
};

class CaptureException : public Lab_5_Exceptions{
public:
    explicit CaptureException(std::string message) : Lab_5_Exceptions (std::move(message)){}
};
class FrameException : public Lab_5_Exceptions{
public:
    explicit FrameException(std::string message) : Lab_5_Exceptions (std::move(message)){}
};