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
                onCheckedChanged:
                {
                    console.log("Enabled mainState changed ", checked)
                    if(checked !== UIM.alarmGeneralState)
                    {
                        UIM.alarmGeneralState = checked;
                        UIM.alarmNightState = !checked;
                        UIM.alarmVacationState = !checked;
                    }
                }
            }
            Switch
            {
                id: nightState
                text: "Night Mode State"

                checked: UIM.alarmNightState
                onCheckedChanged:
                {
                    if(checked !== UIM.alarmNightState)
                    {
                        UIM.alarmGeneralState = !checked;
                        UIM.alarmNightState = checked;
                        UIM.alarmVacationState = !checked;
                    }
                }
            }
            Switch
            {
                id: vacationState
                text: "Vacation Mode State"

                checked: UIM.alarmVacationState
                onCheckedChanged:
                {
                    if(checked !== UIM.alarmVacationState)
                    {
                        UIM.alarmGeneralState = !checked;
                        UIM.alarmNightState = !checked;
                        UIM.alarmVacationState = checked;
                    }
                }
            }
        }
    }

    Component.onCompleted:
    {
        console.log("AlarmPanel completed");
    }
}
