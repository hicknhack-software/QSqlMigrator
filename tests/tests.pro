
!contains(CONFIG, NO_QSM_SQLITE) {
    SUBDIRS += sqlite
    SUBDIRS += api
}
!contains(CONFIG, NO_QSM_MYSQL): {
    SUBDIRS += mysql
}
!contains(CONFIG, NO_QSM_POSTGRES) {
    SUBDIRS += postgresql
}
#!contains(CONFIG, NO_QSM_FIREBIRD) {
#    SUBDIRS += firebird
#}

include(../_subdir.pri)
