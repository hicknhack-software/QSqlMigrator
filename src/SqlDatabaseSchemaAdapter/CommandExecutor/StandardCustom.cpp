#include "StandardCustom.h"

#include "SqlSchemaCommand/CustomBase.h"

namespace QSqlMigrator {
namespace SqlDatabaseSchemaAdapter {

const StandardCustom::CommandType &
StandardCustom::commandType() const
{
    return SqlSchemaCommand::CustomBase::commandName();
}

bool
StandardCustom::execute(const Command &command, Context &context) const
{
    auto customBase = command.staticCast<const SqlSchemaCommand::CustomBase>();
    Q_ASSERT(customBase);

    bool success = customBase->up( context.sqlAdapter().database() );

    if (success && context.undo().isUsed())
        context.undo().setCommand(customBase->reverse(command));

    return success;
}

bool
StandardCustom::isValid(const Command&, const Context&) const
{
    return true;
}

} // namespace SqlDatabaseSchemaAdapter
} // namespace QSqlMigrator

