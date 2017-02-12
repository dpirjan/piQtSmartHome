#!/bin/sh

sudo kill -2 `pidof uiManager`

sudo kill -2 `pidof alarmManager`

sudo kill -2 `pidof sensorManager`

sudo kill -2 `pidof userManager`

sudo kill -2 `pidof mailManager`

sudo kill -2 `pidof databaseManager`