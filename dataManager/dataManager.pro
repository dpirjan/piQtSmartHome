TEMPLATE = subdirs

SUBDIRS = databaseManagerInfo \
          databaseManagerService \
          mailManagerService

databaseManagerService.depends = databaseManagerInfo
