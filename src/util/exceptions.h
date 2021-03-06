// Copyright (C) 2011  Lukas Lalinsky
// Distributed under the MIT license, see the LICENSE file for details.

#ifndef ACOUSTID_UTIL_EXCEPTIONS_H_
#define ACOUSTID_UTIL_EXCEPTIONS_H_

#include <exception>
#include <QString>

namespace Acoustid {

class Exception : public std::exception
{
public:
	Exception(const QString &msg) : m_msg(msg.toLocal8Bit()) { }
	~Exception() throw() { }
	const char *what() const throw() { return m_msg.constData(); }
private:
	QByteArray m_msg;
};

class IOException : public Exception
{
public:
	IOException(const QString &msg) : Exception(msg) { }
};

}

#endif
