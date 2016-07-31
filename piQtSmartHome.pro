TEMPLATE = subdirs

SUBDIRS = databaseManager \
          sensorManager \
          settingsManager

databaseManager.depends = settingsManager
sensorManager.depends = databaseManager
