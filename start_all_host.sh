#!/bin/sh

sudo ls -las

if pidof -x "databaseManager" >/dev/null;
then
	echo "DatabaseManager already started"
else
	sudo /usr/share/pismarthome/databaseManager &
fi;

if pidof -x "mailManager" >/dev/null;
then
	echo "MailManager already started"
else
	sudo /usr/share/pismarthome/mailManager &
fi;

if pidof -x "userManager" >/dev/null;
then
	echo "UserManager already started"
else
	sudo /usr/share/pismarthome/userManager &
fi;

if pidof -x "sensorManager" >/dev/null;
then
	echo "SensorManager already started"
else
	sudo /usr/share/pismarthome/sensorManager &
fi;

if pidof -x "alarmManager" >/dev/null;
then
	echo "AlarmManager already started"
else
	sudo /usr/share/pismarthome/alarmManager &
fi;

if pidof -x "uiManager" >/dev/null;
then
	echo "UIManager already started"
else
	sudo /usr/share/pismarthome/uiManager &
fi;