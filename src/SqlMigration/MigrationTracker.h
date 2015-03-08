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

#include <QStringList>

namespace QSqlMigrator {
namespace SqlMigration {

/// \brief Interface of a service that is used to track successfully executed migrations
class QSQLMIGRATOR_DLL_EXPORT MigrationTracker
{
public:
    using MigrationName = QString;
    using MigrationNames = QStringList;

public:
    /// \return true, if the tracking is ready to process migrations
    virtual bool isPrepared() const = 0;

    /// \return true, if all facilities for the tracking were created
    /// \note call this method only if isPrepared returns false!
    virtual bool prepare() const = 0;

    /// \return a list of all tracked successful migration executions
    virtual MigrationNames trackedNames() const = 0;

    /// \returns true if the specified migrations was tracked as successfully executed
    virtual bool wasTracked(const MigrationName &) const = 0;

    /// \return true, if it was able to sucessfully record that the migrations was executed
    virtual bool track(const MigrationName &) const = 0;

    /// \return true, if it was able to sucessfully remove a migration from the tracked successfully executed migrations
    virtual bool forget(const MigrationName &) const = 0;
};

} // namespace SqlMigration
} // namespace QSqlMigrator
