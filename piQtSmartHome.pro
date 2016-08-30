TEMPLATE = subdirs

CONFIG += debug

CONFIG(debug, debug|release) {
    message("$${TARGET} - debug mode")
}else {
    message("$${TARGET} - release mode")
}


SUBDIRS = dataManager \
          sensorManager \
          utils

dataManager.depends = utils
sensorManager.depends = utils
sensorManager.depends = dataManager
