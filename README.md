Linux: [![Build Status](https://travis-ci.com/hicknhack-software/QSqlMigrator.svg?branch=master)](https://travis-ci.com/hicknhack-software/QSqlMigrator)
Windows: [![Build Status](https://ci.appveyor.com/api/projects/status/github/hicknhack-software/QSqlMigrator?branch=master&svg=true)](https://ci.appveyor.com/project/arBmind/qsqlmigrator)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/898/badge.svg)](https://scan.coverity.com/projects/898)

# QSqlMigrator

A library to help you manage SQL database schemas and migrations.
This tool is inspired by Ruby on Rails ActiveRecord migrations.

Benefits:
- Perform schema and data migrations
- Ensure that all migrations have been run
- Ensure that nobody tempered with the database schema

Features:
- builtin support for tables, columns and indices
- you can build custom commands
- easy api
- everything is tested
- database specific driver plugins (includes: PostgreSql, MySql and Sqlite)

Use the main Github repository for issues and pull requests to
https://github.com/hicknhack-software/QSqlMigrator

## Prerequisites

QSqlMigrator is meant to be used with the Qt Framework.
It has been successfully tested with:

Windows:
- Visual Studio 2010, Qt 4.8.6 and Qt 5.4.1
- Visual Studio 2012, Qt 5.4.1
- Visual Studio 2013, Qt 5.4.1
- MinGW 4.9.1 provided with Qt 5.4.1

Linux:
- GCC 4.9.1 with Qt 4.8.6 and Qt 5.4.1
- CLANG 3.3 with Qt 4.8.6 and Qt 5.4.1

You need the working database driver plugin for your database, both for Qt and the SqlMigrator.

**Hint:** Our tests include instructions to create the Qt SQL driver plugins.

## Usage

The backbone of migrations are immutable migration steps once they have been published. Each migration should transform the database from an old valid state into a new valid state.

First Create a new migration class. The order of the name has to express the intended order of the migrations. You may use simple numbers, but we advice the following naming pattern:
`MyyyyMMdd_hhmmss_<Verb><Target>`

For this example we use: `M201503301340654_CreateUsers`

The header `M201503301340654_CreateUsers.h` always looks like this:
```cpp
#include "api.h"

class M201503301340654_CreateUsers : public Migrations::Migration {
public:
    M201503301340654_CreateUsers();
};
```

As you see, we only need to implement the constructor.
That's the place where all the schema and data changing steps are prepared.
```cpp
#include "M201503301340654_CreateUsers.h"

M201503301340654_CreateUsers::M201503301340654_CreateUsers() {
    using namespace Structure;
    using namespace Commands;
    // create the table
    Table::Builder table("users");
    table << Column("id", Type::Integer, Column::Primary|Column::AutoIncrement)
          << Column("name", Type(Type::VarChar, 50))
          << Column("email", Type(Type::VarChar, 100))
          << Column("password_salt", Type(Type::VarChar, 64))
          << Column("password_encrypted", Type(Type::VarChar, 64));
    add(new CreateTable(table));
    // add an index to the emails
    Index::Builder emailIndex( "users_email", "users" );
    emailIndex << Index::Column("email");
    add(new CreateIndex(emailIndex));
}
```

Now we need a map of all the migration classes. We tried some distributed registrations, but faild to implement one that works all the time. (See ticket #9)

The current best practice is to build a service class that implements all your migration requirements. QSqlMigrator will help you to achieve this.

A basic example may look like this:
```cpp
class MyAppMigrator {
    /// \returns true if database is ok
    static bool migrate(QSqlDatabase database);
};
```

A very basic implementation:
```cpp
#include "M201503301340654_CreateUsers.h"

#include "Migrations/MigrationRepository.h"
#include "QSqlMigrator/QSqlMigratorService.h"
#include "SqliteMigrator/SqliteMigrator.h"

MigrationRepository::NameMigrationMap migrations() {
    MigrationRepository::NameMigrationMap result;
    result.insert("M201503301340654_CreateUsers", new M201503301340654_CreateUsers());
    return result;
}

bool MyAppMigrator::migrate(QSqlDatabase database) {
    auto contextBuilder = MigrationExecutionContext::Builder(migrations());
    contextBuilder.setDatabase(database);
    auto context = SqliteMigrator::buildContext(contextBuilder);

    QSqlMigrator::QSqlMigratorService manager;
    return manager.applyAll(*context);
}
```

This is a very simple application. For development purposes this should be good to go.

### Validate the database schema

In production systems it's advisable to check if the present database schema is what we expect and simply refuse execution if the schema was tampered.

An implementation to the above example might look like:
```cpp
#include "MigrationExecution/LocalSchemeMigrationExecutionContext.h"
#include "MigrationExecution/LocalSchemeMigrationExecutionService.h"
#include "LocalSchemeMigrator/LocalSchemeMigrator.h"
#include "LocalSchemeMigrator/LocalSchemeComparisonContext.h"
#include "LocalSchemeMigrator/LocalSchemeComparisonService.h"

bool MyAppMigrator::validate(QSqlDatabase database) {
    LocalSchemePtr localScheme(new Structure::LocalScheme);
    auto local = LocalSchemeMigrationExecutionContext(migrations());
    local.setLocalScheme(localScheme);
    local.setLocalSchemeCommandServiceRepository(createCommandServiceRepository());

    LocalSchemeMigrationExecutionService localManager;
    localManager.executeBatch(local.migrationMap().keys(), local);

    auto contextBuilder = MigrationExecutionContext::Builder(migrations());
    contextBuilder.setDatabase(database);
    auto context = SqliteMigrator::buildContext(contextBuilder);

    LocalSchemeComparisonContext comparisonContext(localScheme, context->helperRepository(), database);
    LocalSchemeComparisonService comparisonService;
    return comparisonService.compareLocalSchemeWithDatabase(comparisonContext);
}
```

Look at the `example/` folder for a complete working version of the above example.

## Changelog

**Version 1.5.1** (bugfix) 2014-11-11

Fixed crash for removed migrations.

**Version 1.5.0** (postgresql) 2014-04-17

Over a year after the 1.0, we are proud to release the next version. With over 100 commits, some code and api cleanups.

We now follow semantic versioning schema.

Some highlights:
* many cleanups
* more compiler support (see readme)
* support for new Qt versions
* postgresql database support

**v1.0** (first release) 2013-05-22

Features:
* builtin support for tables, columns and indices
* you can build custom commands
* easy api
* everything is tested
* database specific driver plugins (sqlite and mysql provided)

### Roadmap

**Version 2.0.0** (restructure) 2015-xx

This next version will contain a full refactoring of all the code to C++11 and modern library design. The feature set should stay the same, so migrations should go easy.

## Development

### Tests

All tests are tied to the appropriate source modules.
They will run automatically after they have been built and therefore the build will fail if the tests do not run successfully. (You can easily disable tests if you want to hack away)
Some tests require configuration options to access the database under test.
