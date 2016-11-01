import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import QtQuick.Controls.Styles 1.4


Rectangle {
    width: 50
    color: settings.style==="Material" ? Material.background  : Universal.background

    Material.background: "transparent"
    Material.elevation: 0

    property alias appSideNavMenuItem: appSideNavMenuItem
    property alias appSideNavHomeItem: appSideNavHomeItem
    property alias appSideNavWeatherItem: appSideNavWeatherItem
    property alias appSideNavUsersItem: appSideNavUsersItem
    property alias appSideNavDrawer: appSideNavDrawer

    Column
    {
        spacing: 0.5
        anchors.fill: parent

        AppSideNavItem
        {
            id: appSideNavMenuItem
            appSideNavItemIcon.source: "qrc:/icons/ic_menu_white_48dp.png"

            appSideNavItemMouseArea.onClicked:
            {
                console.log("Menu clicked")
                appSideNavDrawer.open()
            }
        }

        AppSideNavItem
        {
            id: appSideNavHomeItem
            appSideNavItemIcon.source: "qrc:/icons/ic_home_white_48dp.png"

            appSideNavItemMouseArea.onClicked:
            {
                console.log("Home clicked")
                stackView.replace("qrc:/qml/components/SmartHomePanel.qml")

            }
        }

        AppSideNavItem
        {
            id: appSideNavWeatherItem
            appSideNavItemIcon.source: "qrc:/icons/ic_location_on_white_48dp.png"

            appSideNavItemMouseArea.onClicked:
            {
                console.log("Weather clicked")
                stackView.replace("qrc:/qml/components/WeatherPanel.qml")
            }
        }

        AppSideNavItem
        {
            id: appSideNavUsersItem
            appSideNavItemIcon.source: "qrc:/icons/ic_people_white_48dp.png"

            appSideNavItemMouseArea.onClicked:
            {
                console.log("Users clicked")
                stackView.replace("qrc:/qml/components/UserPanel.qml")
            }
        }

        AppSideNavItem
        {
            id: appSideNavAlarmItem
            appSideNavItemIcon.source: "qrc:/icons/ic_security_white_48dp.png"

            appSideNavItemMouseArea.onClicked:
            {
                console.log("Alarm clicked")
                stackView.replace("qrc:/qml/components/AlarmPanel.qml")
            }
        }

        // @TODO move More to bottom without this ugly approach
        AppSideNavItem
        {
        }

        AppSideNavItem
        {
        }

        AppSideNavItem
        {
        }

        AppSideNavItem
        {
        }

        AppSideNavItem
        {
        }

        AppSideNavItem
        {
        }

        AppSideNavItem
        {
            id: appSideNavMoreItem
            appSideNavItemIcon.source: "qrc:/icons/ic_more_horiz_white_48dp.png"

            y: parent.height - height

            appSideNavItemMouseArea.onClicked:
            {
                console.log("More clicked")
                optionsMenu.open()
            }

            Menu
            {
                id: optionsMenu
                x: parent.width - width
                transformOrigin: Menu.TopRight

                MenuItem
                {
                    font.family: fontLoader.name
                    text: "Settings"
                    onTriggered: settingsPopup.open()
                }
                MenuItem
                {
                    font.family: fontLoader.name
                    text: "About"
                    onTriggered: aboutDialogPopupWindow.open()
                }
            }
        }
    }

    Drawer
    {
        id: appSideNavDrawer
        width: Math.min(window.width, window.height) / 9 * 2
        height: window.height

        ListView
        {
            id: appSideNavDrawerListView
            currentIndex: -1
            anchors.fill: parent

            delegate: ItemDelegate
            {
                width: parent.width
                font.family: fontLoader.name
                text: model.title
                highlighted: ListView.isCurrentItem
                onClicked:
                {
                    if (appSideNavDrawerListView.currentIndex != index)
                    {
                        appSideNavDrawerListView.currentIndex = index
                        console.log("Loading ", model.source);
                        stackView.replace(model.source)
                    }
                    appSideNavDrawer.close()
                }
            }

            model: ListModel
            {
                ListElement { title: "Menu"; source: "qrc:/qml/components/LoginPanel.qml" }
                ListElement { title: "Home"; source: "qrc:/qml/components/SmartHomePanel.qml" }
                ListElement { title: "Weather"; source: "qrc:/qml/components/WeatherPanel.qml" }
                ListElement { title: "Users"; source: "qrc:/qml/components/UserPanel.qml" }
                ListElement { title: "Alarm"; source: "qrc:/qml/components/AlarmPanel.qml" }
                ListElement { title: "About"; source: "qrc:/qml/components/AboutDialogPanel.qml" }
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }
}
