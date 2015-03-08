/****************************************************************************
**
** Copyright (C) 2013, HicknHack Software
** All rights reserved.
** Contact: http://www.hicknhack-software.com/contact
**
** This file is part of the QSqlMigrator
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/
#pragma once

#include "config.h"

#include <QString>
#include <QStack>
#include <QStringList>

#include <functional>

namespace QSqlMigrator {
namespace LoggingTrace {

/// \brief abstract baseclass for Logging with traces
class QSQLMIGRATOR_DLL_EXPORT Logging {
public:
    using Message = QString;
    using Trace = std::function<Message()>;
    using TraceStack = QStack<Trace>;
    using TraceStrings = QStringList;

    struct Scope {
        Scope(Logging &, const Trace&);
        ~Scope();

    private:
        Logging &m_logging;
    };

public:
    /// creates a scope with a trace
    Scope scope(const Trace &);

    /// creates a list of scope traces (the current scope is first)
    TraceStrings traceStrings() const;

    /// reports an error message in current scope
    virtual void error(const Message &) const = 0;

    /// reports a warning message in current scope
    virtual void warning(const Message &) const = 0;

    //    /// reports an info message in current scope
    //    virtual void info(const Message&) = 0;

    //    /// reports an info message without the scope
    //    virtual void infoWithoutScope(const Message&) = 0;

private:
    TraceStack m_traceStack;
};

inline Logging::Scope::Scope(Logging &logging, const Trace & trace)
    : m_logging(logging)
{
    m_logging.m_traceStack.push(trace);
}

inline Logging::Scope::~Scope()
{
    m_logging.m_traceStack.pop();
}

inline Logging::Scope Logging::scope(const Logging::Trace &trace)
{
    return {*this, trace};
}

inline Logging::TraceStrings Logging::traceStrings() const
{
    TraceStrings traceStrings;
    auto it = QVectorIterator<Trace>(m_traceStack);
    it.toBack();
    while(it.hasPrevious()) {
        traceStrings << it.previous()();
    }
    return traceStrings;
}

} // namespace LoggingTrace
} // namespace QSqlMigrator
