import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0

ToolBar
{
    Material.foreground: "white"
    RowLayout
    {
        spacing: 20
        anchors.fill: parent

        Label
        {
            id: titleLabel
            text: qsTr("piSmartHome")
            font.pixelSize: 20
            font.family: fontLoader.name
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            Layout.fillWidth: true
        }

        ToolButton
        {
            contentItem: Image
            {
                fillMode: Image.Pad
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment: Image.AlignVCenter
                source: "qrc:/images/menu.png"
            }
            onClicked: optionsMenu.open()

            Menu
            {
                id: optionsMenu
                x: parent.width - width
                transformOrigin: Menu.TopRight

                MenuItem
                {
                    text: qsTr("Settings")
                    font.family: fontLoader.name
                    onTriggered: settingsPopup.open()
                }
                MenuItem
                {
                    text: qsTr("About")
                    font.family: fontLoader.name
                    onTriggered: aboutDialogPopupWindow.open()
                }
            }
        }
    }
}
