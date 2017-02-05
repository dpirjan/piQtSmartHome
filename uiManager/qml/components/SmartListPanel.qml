import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0

Pane
{
    width: parent.width - smartHomeTabBar.width - ioList.width
    height: parent.height

    Component
    {
        id: smartListDelegate
        Item
        {
            width: parent.width
            height: 40

            Row
            {
                Text
                {
                    font.family: fontLoader.name
                    text: '<b>Ev: </b> ' + address + " " + timestamp + " " + value
                    color: settings.style==="Material" ? Material.foreground : Universal.foreground
                }
            }

            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    listView.currentIndex = index
                    listView.highlight = highlightBar
                    //smartHomeEventsModel.selectedIndex = index
                }
            }
        }
    }

    // Define a highlight with customized movement between items.
    Component
    {
        id: highlightBar
        Rectangle
        {
            color: settings.style==="Material" ? Material.accent  : Universal.accent
            width: parent.width
            height: 30
            y: listView.currentItem.y;
            Behavior on y
            {
                SpringAnimation
                {
                    spring: 0.9
                    damping: 0.15
                }
            }
        }
    }

    ColumnLayout
    {
        id: smartListColumn
        spacing: 40
        anchors.fill: parent
        anchors.topMargin: 10

        ListView
        {
            id: listView
            anchors.fill: parent
            model: smartHomeEventsModel
            delegate: smartListDelegate
            focus: true
            clip: true
        }
    }
}
