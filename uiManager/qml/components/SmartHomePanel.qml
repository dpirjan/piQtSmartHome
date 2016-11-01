import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0

Pane
{
    id: smartHomePanel

    TabBar
    {
        id: smartHomeTabBar
        y: 0
        width: parent.width / 4

        onCurrentIndexChanged:
        {
            console.log("smartHomeTabBar: currentIndex changed: ", currentIndex)
            if(currentIndex === 0)
            {
                console.log("Zones")
                zonesList.visible = true
                categoriesList.visible = false
            }
            else
            {
                console.log("Categories")
                categoriesList.visible = true
                zonesList.visible = false
            }
        }

        TabButton
        {
            font.family: fontLoader.name
            text: qsTr("Zones")
        }
        TabButton
        {
            font.family: fontLoader.name
            text: qsTr("Categories")
        }
    }

    ZonesListPanel
    {
        id: zonesList
        visible: true
        anchors.top: smartHomeTabBar.bottom
        height: parent.height - smartHomeTabBar.height
    }

    CategoriesListPanel
    {
        id: categoriesList
        visible: false
        anchors.top: smartHomeTabBar.bottom
        height: parent.height - smartHomeTabBar.height
    }
}
