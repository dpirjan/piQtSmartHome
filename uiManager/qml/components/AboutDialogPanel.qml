import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import QtQuick.Layouts 1.1

Pane
{
        id: aboutDialogPanel

        Column
        {
            id: aboutColumn
            spacing: 50

            Label
            {
                text: qsTr("About")
                font.bold: true
            }

    ColumnLayout
{
    spacing: 10
            Label
            {
                width: aboutDialogPanel.availableWidth
                //@TODO update this
                text: qsTr("piSmartHome")
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label
            {
                width: aboutDialogPanel.availableWidth
                //@TODO update this
                text: qsTr("Description here to be added later ..")
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
        }
}

        Component.onCompleted:
        {
            console.log("AboutDialog completed");
        }

    MouseArea
    {
        anchors.fill: parent
        onClicked: console.log("ABOUT DIALOG PANEL CLICKED");
    }
}
