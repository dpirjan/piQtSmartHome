import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0

Pane
{
    id: alarmPanel


    Column
    {
        spacing: 40
        anchors.fill : parent

        Label
        {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignLeft
            font.family: fontLoader.name
            font.bold: true
            font.pixelSize: 35
            text: "Alarm modes"
        }

        Column
        {
            spacing: 20
            width: parent.width

            Switch
            {
                id: mainState
                text: "General Mode State (Armed/Disarmed)"

                checked: UIM.alarmGeneralState
            }
            Switch
            {
                id: nightState
                text: "Night Mode State"

                checked: UIM.alarmNightState
            }
            Switch
            {
                id: vacationState
                text: "Vacation Mode State"

                checked: UIM.alarmVacationState
            }
        }
    }

    Connections
    {
        ignoreUnknownSignals: true
        target: mainState
        onCheckedChanged: UIM.alarmGeneralState = mainState.checked
    }

    Connections
    {
        ignoreUnknownSignals: true
        target: nightState
        onCheckedChanged: UIM.alarmNightState = nightState.checked
    }

    Connections
    {
        ignoreUnknownSignals: true
        target: vacationState
        onCheckedChanged: UIM.alarmVacationState = vacationState.checked
    }

    Component.onCompleted:
    {
        console.log("AlarmPanel completed");
    }
}
