import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0

Pane
{
    id: loginPanel

    Column
    {
        spacing: 60
        anchors.fill: parent

        Label
        {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignLeft
            text: qsTr("Login:")
        }

        Column
        {
            spacing: 20

            TextField
            {
                id: usernameField
                placeholderText: qsTr("Username")
                width: Math.max(implicitWidth, Math.min(implicitWidth * 2, pane.availableWidth / 3))
                anchors.left: parent.left

                onAccepted: console.log("Username entered: ", text)
            }

            TextField
            {
                id: passwordField
                placeholderText: qsTr("Password")
                width: Math.max(implicitWidth, Math.min(implicitWidth * 2, pane.availableWidth / 3))
                anchors.left: parent.left
                echoMode: TextInput.Password

                onAccepted: console.log("Password entered: ", text)
            }

            TextField
            {
                id: pinField
                placeholderText: qsTr("Pin")
                width: Math.max(implicitWidth, Math.min(implicitWidth * 2, pane.availableWidth / 3))
                anchors.left: parent.left
                echoMode: TextInput.Password
                validator: IntValidator { bottom: 0; top: 9999; }

                onAccepted: console.log("Pin entered: ", text)
            }
        }
    }
}

