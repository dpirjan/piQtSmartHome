import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0

Pane
{
    id: weatherPanel

    Label
    {
        text: qsTr("Weather Panel")
    }

    Component.onCompleted:
    {
        console.log("WeatherPanel completed");
    }
}
