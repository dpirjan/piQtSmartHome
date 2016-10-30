import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0

Popup
{
    x: (window.width - width) / 2
    y: window.height / 6
    width: Math.min(window.width, window.height) / 3 * 2
    height: settingsColumn.implicitHeight + topPadding + bottomPadding
    modal: true
    focus: true

    contentItem: ColumnLayout
    {
        id: settingsColumn
        spacing: 20

        Label
        {
            text: "Settings"
            font.bold: true
        }

        RowLayout
        {
            spacing: 10

            Label
            {
                text: "Style:"
            }

            ComboBox
            {
                id: styleBox
                property int styleIndex: -1
                model: ["Default", "Material", "Universal"]
                Component.onCompleted:
                {
                    styleIndex = find(settings.style, Qt.MatchFixedString)
                    if (styleIndex !== -1)
                        currentIndex = styleIndex
                }
                Layout.fillWidth: true
            }
        }

        Label
        {
            text: "Restart required"
            color: "#e41e25"
            opacity: styleBox.currentIndex !== styleBox.styleIndex ? 1.0 : 0.0
            horizontalAlignment: Label.AlignHCenter
            verticalAlignment: Label.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        RowLayout
        {
            spacing: 10

            Button {
                id: okButton
                text: qsTr("Ok")
                onClicked:
                {
                    settings.style = styleBox.displayText
                    settingsPopup.close()
                }

                Material.foreground: Material.primary
                Material.background: "transparent"
                Material.elevation: 0

                Layout.preferredWidth: 0
                Layout.fillWidth: true
            }

            Button {
                id: cancelButton
                text: qsTr("Cancel")
                onClicked:
                {
                    styleBox.currentIndex = styleBox.styleIndex
                    settingsPopup.close()
                }

                Material.background: "transparent"
                Material.elevation: 0

                Layout.preferredWidth: 0
                Layout.fillWidth: true
            }
        }
    }
}
