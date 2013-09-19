HicknHack QSqlMigrator Library
==============================

https://github.com/hicknhack-software/QSqlMigrator

Overview
--------

A library to help you keep track of database schema migrations.
This tool is inspired by ActiveRecord migrations which is part of Ruby on Rails.

Features:
- builtin support for tables, columns and indices
- you can build custom commands
- easy api
- everything is tested
- database specific driver plugins (sqlite and mysql provided)


Prerequisites
-------------

QSqlMigrator is meant to be used with the Qt Framework.
It has been successfully tested with:

Windows: 
- Visual Studio 2010 compiler - Qt 4.8.4 and Qt 5.0.0.
- 
Linux:
- GCC 4.8.1 and Qt 4.8.5
- MinGw 4.8.1 and Qt 4.8.5 (should also work on windows)

You will need a working database driver plugin for your database, both for qt and the sql migrator.
Hint: Our tests include instructions to create the qt sql driver plugin


Tests
-----

All tests are tight to the appropriate source modules.
They will run automatically after they have been built and therefore the build will fail if the tests do not run successfully.
Some tests require some configuration options to access the database under test.
