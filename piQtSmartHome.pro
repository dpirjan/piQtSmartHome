TEMPLATE = subdirs

SUBDIRS = dataManager \
          sensorManager \
          settingsManager

dataManager.depends = settingsManager
sensorManager.depends = dataManager
