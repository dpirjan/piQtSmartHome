import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import QtQuick.Controls.Styles 1.4

Rectangle
{
    id: appSideNavItem
    width: parent.width
    height: parent.width //not a bug, must be Rectangle

    color: applicationSideNavigation.color

    property alias appSideNavItem: appSideNavItem
    property alias appSideNavItemIcon: appSideNavItemIcon
    property alias appSideNavItemMouseArea: appSideNavItemMouseArea

    Image
    {
        id: appSideNavItemIcon
        smooth: true
        anchors.centerIn: parent
    }

    MouseArea
    {
        id: appSideNavItemMouseArea
        anchors.fill: parent
    }
}
