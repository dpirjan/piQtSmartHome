TEMPLATE = subdirs

CONFIG += debug

CONFIG(debug, debug|release) {
    message("$${TARGET} - debug mode")
}else {
    message("$${TARGET} - release mode")
}

SUBDIRS = piHomeCommon \
          dataManager \
          userManager \
          sensorManager \
          uiManager

dataManager.depends = piHomeCommon
sensorManager.depends = dataManager
userManager.depends = piHomeCommon
uiManager.depends = userManager
uiManager.depends = sensorManager
