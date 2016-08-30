TEMPLATE = subdirs

CONFIG(debug, debug|release) {
    message("$${TARGET} - debug mode")
}else {
    message("$${TARGET} - release mode")
}

SUBDIRS = databaseManagerInfo \
          databaseManagerService \
          mailManagerService

databaseManagerService.depends = databaseManagerInfo
