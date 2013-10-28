#ifndef LOCALSCHEMEMIGRATOR_LOCALSCHEMEMIGRATOR_H
#define LOCALSCHEMEMIGRATOR_LOCALSCHEMEMIGRATOR_H

#include "config.h"

#include "CommandExecution/LocalSchemeCommandExecutionServiceRepository.h"

#include <QSharedPointer>

namespace LocalSchemeMigrator {

QSharedPointer<CommandExecution::LocalSchemeCommandExecutionServiceRepository> QSQLMIGRATOR_DLL_EXPORT createCommandServiceRepository();

} // namespace Scheme

#endif // LOCALSCHEMEMIGRATOR_H
