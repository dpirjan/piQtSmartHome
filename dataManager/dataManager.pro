TEMPLATE = subdirs

CONFIG(debug, debug|release) {
    message("$${TARGET} - debug mode")
}else {
    message("$${TARGET} - release mode")
}

SUBDIRS = databaseManagerService \
          mailManagerService
