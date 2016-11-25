import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import QtQuick.VirtualKeyboard 2.1

Pane
{
    id: loginPanel

    Column
    {
        spacing: 40
        anchors.fill: parent

        Label
        {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignLeft
            font.family: fontLoader.name
            font.bold: true
            font.pixelSize: 35
            text: qsTr("Login")
        }

        Column
        {
            spacing: 20
            width: parent.width

            ComboBox
            {
                id: usernameField
                model: UIM.userList
                font.family: fontLoader.name
                width: Math.max(implicitWidth, Math.max(implicitWidth * 2, loginPanel.availableWidth / 3))
                anchors.left: parent.left

                onCurrentIndexChanged:
                {
                    console.log("Username selected: ",  textAt(currentIndex));
                    UIM.selectedUser = textAt(currentIndex);
                    passwordField.text = "";
                    pinField.text = "";
                }
            }

            TextField
            {
                id: passwordField
                font.family: fontLoader.name
                placeholderText: qsTr("Password")
                width: Math.max(implicitWidth, Math.max(implicitWidth * 2, loginPanel.availableWidth / 3))
                anchors.left: parent.left
                echoMode: TextInput.Password
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase | Qt.ImhSensitiveData | Qt.ImhNoPredictiveText

                onAccepted: UIM.enteredPassword = text;
            }

            TextField
            {
                id: pinField
                font.family: fontLoader.name
                placeholderText: qsTr("Pin")
                width: Math.max(implicitWidth, Math.max(implicitWidth * 2, loginPanel.availableWidth / 3))
                anchors.left: parent.left
                echoMode: TextInput.Password
                validator: IntValidator { bottom: 0; top: 9999; }
                inputMethodHints: Qt.ImhDigitsOnly

                onAccepted: UIM.enteredPin = text;
            }
        }
    }
}

