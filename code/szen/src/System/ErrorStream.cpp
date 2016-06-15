#include <iostream>
#include <szen/System/ErrorStream.hpp>
#include <Windows.h>

namespace
{

	void msgInfo(const std::string &title, const std::string &msg)
	{
		MessageBoxA(NULL, msg.c_str(), title.c_str(), MB_ICONINFORMATION | MB_OK);
	}

	void msgWarn(const std::string &title, const std::string &msg)
	{
		MessageBoxA(NULL, msg.c_str(), title.c_str(), MB_ICONEXCLAMATION | MB_OK);
	}

	void msgError(const std::string &title, const std::string &msg)
	{
		MessageBoxA(NULL, msg.c_str(), title.c_str(), MB_ICONERROR | MB_OK);
	}

}

namespace sz
{

	ErrorStream szerr;
	
	////////////////////////////////////////////////////////////////
	ErrorStream& operator<<(ErrorStream& left, ErrorStream::Signal right)
	{
		switch(right)
		{
		case ErrorStream::console:
			std::cout << left.out.str() << std::endl;
			break;

		case ErrorStream::info:
			msgInfo("Info", left.out.str());
			break;

		case ErrorStream::warning:
			msgWarn("Warning", left.out.str());
			break;

		case ErrorStream::error:
			msgError("Error", left.out.str());
			break;
		}

		left.out.str("");

		return left;
	}

}