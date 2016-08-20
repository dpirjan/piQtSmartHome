TEMPLATE = subdirs

SUBDIRS = dataManager \
          sensorManager

sensorManager.depends = dataManager
