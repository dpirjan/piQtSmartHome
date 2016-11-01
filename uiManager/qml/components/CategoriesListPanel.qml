import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0

Pane
{
    width: parent.width / 4
    height: parent.height

    Component
    {
        id: categoryDelegate
        Item
        {
            width: smartHomeTabBar.width
            height: 40

            Row
            {
                Text
                {
                    font.family: fontLoader.name
                    text: '<b>Category:</b> ' + display
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
                    console.log("Selected: ", display)
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
            width: smartHomeTabBar.width
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
        id: categoriesColumn
        spacing: 40
        anchors.fill: parent
        anchors.topMargin: 10

        ListView
        {
            id: listView
            anchors.fill: parent
            model: categoriesModel
            delegate: categoryDelegate
            focus: true
            clip: true
        }
    }
}
