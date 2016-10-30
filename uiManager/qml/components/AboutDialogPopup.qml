import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0

Popup
{
    modal: true
    focus: true
    x: (window.width - width) / 2
    y: window.height / 6
    width: Math.min(window.width, window.height) / 3 * 2
    contentHeight: aboutColumn.height

    contentItem: ColumnLayout
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
            width: aboutDialogPopupWindow.availableWidth
            //@TODO update this
            text: qsTr("piSmartHome")
            wrapMode: Label.Wrap
            font.pixelSize: 12
        }

        Label
        {
            width: aboutDialogPopupWindow.availableWidth
            //@TODO update this
            text: qsTr("Description here to be added later ..")
            wrapMode: Label.Wrap
            font.pixelSize: 12
        }
    }
}

    MouseArea
    {
        anchors.fill: parent
        onClicked: aboutDialogPopupWindow.close()
    }
}
