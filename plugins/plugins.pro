
!contains(CONFIG, NO_QSM_SQLITE) {
    SUBDIRS += SqliteMigrator
}
!contains(CONFIG, NO_QSM_MYSQL) {
    SUBDIRS += MysqlMigrator
}
!contains(CONFIG, NO_QSM_POSTGRES) {
    SUBDIRS += PostgresqlMigrator
}
!contains(CONFIG, NO_QSM_FIREBIRD) {
    SUBDIRS += FirebirdMigrator
}

include(../_subdir.pri)
