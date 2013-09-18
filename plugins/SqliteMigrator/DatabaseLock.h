#ifndef DATABASELOCK_H
#define DATABASELOCK_H

#include "CommandExecution/CommandExecutionServiceRepository.h"
#include "SqliteMigrator.h"

#include <QSqlDatabase>
#include <QUuid>

namespace MigrationExecution {
class MigrationExecutionContext;
}

namespace SqliteMigrator {

class SQLITEMIGRATOR_DLL_EXPORT DatabaseLock
{
    enum
    {
        check_interval_in_sec = 1,
        time_out = 5 //60 * 5
    };

public:
    DatabaseLock(MigrationExecution::MigrationExecutionContext &context, unsigned int timeOutInSec = time_out);

    /*! is the exclusive lock setted */
    bool isExclusiveLocked() const;

    /*! auto release lock */
    ~DatabaseLock();

private:
    static QString buildLockFileName(const MigrationExecution::MigrationExecutionContext &context);

    bool waitForLock() const;
    bool isStillLocked() const;
    bool makeLock() const;
    bool releaseLock() const;

    const QUuid m_uuid;
    const QString m_lockFileName;
    const unsigned int m_timeOutInSec;
    const bool m_lockedSuccessful;
};

} // namespace SqliteMigrator

#endif // DATABASELOCK_H
