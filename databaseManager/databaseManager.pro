TEMPLATE = subdirs

SUBDIRS = databaseManagerInfo \
        databaseManagerService

databaseManagerService.depends = databaseManagerInfo
