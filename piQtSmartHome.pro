TEMPLATE = subdirs

CONFIG += debug

CONFIG(debug, debug|release) {
    message("$${TARGET} - debug mode")
}else {
    message("$${TARGET} - release mode")
}

SUBDIRS = dataManager \
          sensorManager \
          piHomeCommon

dataManager.depends = piHomeCommon
sensorManager.depends = dataManager
