#!/bin/sh

CONF="/etc/ld.so.conf.d/piHome.conf"
EXPORT_1="$PWD/dataManager/databaseManagerInfo"
EXPORT_2="$PWD/utils"

if [ -f ${CONF} ];
then
	echo "${CONF} exists doing nothing"
else
	sudo touch ${CONF}
	sudo chmod 777 ${CONF}
	sudo echo ${EXPORT_1} > ${CONF}
	sudo echo ${EXPORT_2} >> ${CONF}
	sudo ldconfig
	sudo rm ${CONF}
fi;
