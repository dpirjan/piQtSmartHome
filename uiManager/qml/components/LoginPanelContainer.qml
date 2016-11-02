import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.VirtualKeyboard 2.1

Item
{
    Item
    {
        id: appContainer
        width: Screen.width < Screen.height ? parent.height : parent.width
        height: Screen.width < Screen.height ? parent.width : parent.height
        anchors.centerIn: parent
        rotation: Screen.width < Screen.height ? 90 : 0
        LoginPanel
        {
            id: loginPanel
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: inputPanel.top
        }

        InputPanel {
            id: inputPanel
            z: 99
            y: appContainer.height
            anchors.left: parent.left
            anchors.right: parent.right
            states: State {
                name: "visible"
                /*  The visibility of the InputPanel can be bound to the Qt.inputMethod.visible property,
                    but then the handwriting input panel and the keyboard input panel can be visible
                    at the same time. Here the visibility is bound to InputPanel.active property instead,
                    which allows the handwriting panel to control the visibility when necessary.
                */
                when: inputPanel.active
                PropertyChanges {
                    target: inputPanel
                    y: appContainer.height - inputPanel.height
                }
            }
            transitions: Transition {
                from: ""
                to: "visible"
                reversible: true
                ParallelAnimation {
                    NumberAnimation {
                        properties: "y"
                        duration: 250
                        easing.type: Easing.InOutQuad
                    }
                }
            }
            AutoScroller {}
        }
    }
}
