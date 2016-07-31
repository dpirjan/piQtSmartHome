#!/bin/sh

CONF="/etc/ld.so.conf.d/piHome.conf"
EXPORT="$PWD/databaseManager/databaseManagerInfo"

if [ -f ${CONF} ];
then
	echo "${CONF} exists doing nothing"
else
	sudo touch ${CONF}
	sudo chmod 777 ${CONF}
	sudo echo ${EXPORT} > ${CONF}
	sudo ldconfig
	sudo rm ${CONF}
fi;

