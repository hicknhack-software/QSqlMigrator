#pragma once

#include <QSqlDatabase>

class MyAppMigrator {
public:
    /// \returns true if database is ok
    static bool migrate(QSqlDatabase database);

    /// \returns true if database matches migrations
    static bool validate(QSqlDatabase database);
};
