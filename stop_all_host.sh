#!/bin/sh

sudo kill -2 `pidof uiManager` ##<--- piHomeUI.service

sudo kill -2 `pidof alarmManager` ##<--- piHomeAlarm.service

sudo kill -2 `pidof sensorManager` ##<--- piHomeSensor.service

sudo kill -2 `pidof userManager` ##<--- piHomeLogin.service

sudo kill -2 `pidof mailManager` ##<--- piHomeMail.service

sudo kill -2 `pidof webManager` ##--- piHomeWeb.service

sudo kill -2 `pidof databaseManager` ##<--- piHomeDatabase.service
