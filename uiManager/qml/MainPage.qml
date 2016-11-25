import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import Qt.labs.settings 1.0
import "components"

ApplicationWindow
{
    id: window
    width: 1024
    height: 600
    visible: true
    title: qsTr("piSmartHome")

    property alias stackView: stackView
    property alias window: window

    Connections
    {
        target: UIM
        onValidLoginUpdated:
        {
            console.log("Valid Login Updated: ", UIM.validLogin);
            // always go to SmartHomePanel after login successfull
            if(UIM.validLogin)
            {
                stackView.replace("qrc:/qml/components/AlarmPanel.qml");
                stackView.loginEnabled = true;
            }
            else
            {
                stackView.replace("qrc:/qml/components/LoginPanelContainer.qml");
                stackView.loginEnabled = false;
            }
        }
    }

    FontLoader {
        id: fontLoader
        // Font downloaded from https://material.google.com/resources/roboto-noto-fonts.html
        source: "qrc:/fonts/Roboto-Regular.ttf"
    }

    Settings
    {
        id: settings
        property string style: "Material"
    }

    AppSideNav
    {
        id: appSideNav
        visible: true
        width: 50
        height: window.height

        property alias applicationSideNavigation: appSideNav
    }

    StackView
    {
        id: stackView
        x: appSideNav.width
        width: window.width - appSideNav.width
        height: window.height

        initialItem: "qrc:/qml/components/LoginPanelContainer.qml"

        property bool loginEnabled: false
    }

    AboutDialogPopup
    {
        id: aboutDialogPopupWindow

        property alias aboutDialogPopupWindow: aboutDialogPopupWindow
    }

    SettingsPopup
    {
        id: settingsPopup

        property alias settingsPopup: settingsPopup
    }
}
