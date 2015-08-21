#ifndef THREADEXCEPTION_H_INCLUDED
#define THREADEXCEPTION_H_INCLUDED

#include <string>
class ThreadException
{
public:
	typedef std::string string;
public:
	ThreadException(const string &_msg = string(), int errRet = 0);
	string what() const;

private:
	string msg;
};
ThreadException::ThreadException(const string &_msg, int errRet)
	:msg(_msg)
{
	msg += strerror(errRet);
}
std::string ThreadException::what() const
{
	return msg;
}

#endif // THREADEXCEPTION_H_INCLUDED
