import QtQuick 2.6
import QtQuick.Window 2.2

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3
import QtQuick.Dialogs 1.2

Window {
    visible: true
    width: 800
    height: 600

    minimumWidth: 640
    minimumHeight: 480

    title: qsTr("Конвертер медиафайлов")

    Material.accent: Material.Blue

    FontLoader { id: fixedFont; source: "/fonts/Roboto-Regular.ttf" }

    Label {
        id: filenameLabel

        text: "Имя исходного файла"
        color: Material.accent
        y: 10
        x: 15
        width: 70
        height: 30

        verticalAlignment: Label.AlignBottom

        font.pixelSize: 14
        font.family: fixedFont.name
    }

    TextField {
        id: filenameField

        anchors.left: filenameLabel.left

        anchors.right: chooseFileButton.left
        anchors.rightMargin: 20

        anchors.top: filenameLabel.bottom

        width: 200
        height: 50

        placeholderText: "Путь к файлу"
        font.pixelSize: 18
    }

    Button {
        id: chooseFileButton

        width: 160
        height: filenameField.height + 5

        x: parent.width - 15 - width
        y: 30

        text: "Выбрать файл..."
        font.pixelSize: 16
        font.family: fixedFont.name

        onClicked: {
            fileDialog.setVisible(true)
        }
    }

    FileDialog {
        id: fileDialog

        title: "Выберите медиафайл"
        folder: shortcuts.home

        selectExisting: true
        selectMultiple: false

        onAccepted: {
            var filename = fileDialog.fileUrl

            filenameField.text = filename.toString().replace("file:///", "")
            setVisible(false)
        }
        onRejected: {
            setVisible(false)
        }
    }

    Label {
        id: videoMetaDataLabel
        anchors.left: filenameLabel.left
        anchors.top: filenameField.bottom
        anchors.topMargin: 30

        height: 30
        width: 70

        text: "Видеопоток"
        font.pixelSize: 18
        color: Material.accent
    }

    property int halfWidth: width / 2

    Label {
        id: frameWidthLabel

        anchors.left: videoMetaDataLabel.left
        anchors.top: videoMetaDataLabel.bottom

        verticalAlignment: Label.AlignBottom

        height: 30
        width: 70

        text: "Ширина кадра"
        font.pixelSize: 14
        color: Material.accent
    }

    TextField {
        id: frameWidthField

        anchors.left: frameWidthLabel.left
        anchors.top: frameWidthLabel.bottom

        width: halfWidth / 2 - 40
        height: 50

        placeholderText: ""
        font.pixelSize: 18
    }

    Label {
        id: frameHeightLabel

        anchors.left: frameWidthField.right
        anchors.leftMargin: 50
        anchors.top: frameWidthLabel.top

        verticalAlignment: Label.AlignBottom

        height: frameWidthLabel.height
        width: frameWidthLabel.width

        text: "Высота кадра"
        font.pixelSize: 14
        color: Material.accent
    }

    TextField {
        id: frameHeightField

        anchors.left: frameHeightLabel.left
        anchors.top: frameWidthField.top

        width: frameWidthField.width
        height: frameWidthField.height

        placeholderText: ""
        font.pixelSize: 18
    }

    Label {
        id: videoBitrateLabel

        anchors.left: frameWidthField.left
        anchors.top: frameWidthField.bottom
        anchors.topMargin: 10

        verticalAlignment: Label.AlignBottom

        height: frameWidthLabel.height
        width: frameWidthLabel.width

        text: "Битрейт"
        font.pixelSize: 14
        color: Material.accent
    }

    TextField {
        id: videoBitrateField

        anchors.left: videoBitrateLabel.left
        anchors.top: videoBitrateLabel.bottom

        width: halfWidth - 30
        height: frameWidthField.height

        placeholderText: ""
        font.pixelSize: 18
    }

    Label {
        id: subtitlesMetaDataLabel
        x: halfWidth + 5
        anchors.top: videoMetaDataLabel.top

        height: 30
        width: 70

        text: "Субтитры"
        font.pixelSize: 18
        color: Material.accent
    }

    ListView {
        id: subtilesListView

        anchors.top: subtitlesMetaDataLabel.bottom
        anchors.left: subtitlesMetaDataLabel.left

        width: halfWidth - 20

        delegate: ItemDelegate {

        }
    }

    Button {
        id: addSubtitleButton

        anchors.top: subtilesListView.bottom
        anchors.left: subtilesListView.left

        width: 200
        height: 50

        text: "Добавить субтитры..."
        font.pixelSize: 14
    }

    Button {
        id: multiplexButton

        property var parentToAnchor: (subtilesListView.y + subtilesListView.height > videoBitrateField.y + videoBitrateField.width) ?
                                     subtilesListView : videoBitrateField;

        anchors.top: parentToAnchor.bottom
        anchors.topMargin: 10
        anchors.left: videoBitrateField.left

        width: 250
        height: 50

        text: "Мультиплексировать"
        font.pixelSize: 18
    }

    Label {
        id: outFilenameLabel

        text: "Имя результирующего файла"
        color: Material.accent

        x: 15
        anchors.bottom: outFilenameField.top

        width: 200
        height: 30

        verticalAlignment: Label.AlignVCenter

        font.pixelSize: 14
        font.family: fixedFont.name
    }

    CheckBox {
        id: outFilenameCheckBox

        anchors.left: outFilenameLabel.right
        anchors.leftMargin: 20
        anchors.top: outFilenameLabel.top

        height: outFilenameLabel.height
        width: 250

        text: qsTr("Сохранить в исходный файл")

        font.pixelSize: 14
        font.family: fixedFont.name

        contentItem: Text {
            text: parent.text
            font: parent.font

            leftPadding: parent.indicator.width + parent.spacing
            verticalAlignment: Text.AlignVCenter

            color: Material.accent
        }

        onCheckStateChanged: {
            if(checkState == Qt.Checked)
            {
                outFilenameField.text = filenameField.text
                outFilenameField.readOnly = true
                chooseOutFileButton.enabled = false
            }
            else
            {
                outFilenameField.readOnly = false
                chooseOutFileButton.enabled = true
            }
        }
    }

    TextField {
        id: outFilenameField

        anchors.left: outFilenameLabel.left

        anchors.bottom: muxingStateLabel.top
        anchors.bottomMargin: 20

        anchors.right: chooseOutFileButton.left
        anchors.rightMargin: 20

        height: 50

        placeholderText: "Путь к результирующему файлу"
        font.pixelSize: 18
    }

    Button {
        id: chooseOutFileButton

        width: 160
        height: outFilenameField.height + 5

        x: parent.width - 15 - width
        y: outFilenameField.y - 10

        text: "Выбрать файл..."
        font.pixelSize: 16
        font.family: fixedFont.name

        onClicked: {
            outFileDialog.setVisible(true)
        }
    }

    FileDialog {
        id: outFileDialog

        title: "Выберите медиафайл"
        folder: shortcuts.home

        nameFilters: [ "Видеофайлы (*.mp4 *.mkv)"]

        selectExisting: false
        selectMultiple: false

        onAccepted: {
            var filename = outFileDialog.fileUrl

            outFilenameField.text = filename.toString().replace("file:///", "")
            setVisible(false)
        }
        onRejected: {
            setVisible(false)
        }
    }

    Label {
        id: muxingStateLabel
        x: 15
        anchors.bottom: muxingStateProgressBar.top

        height: 30
        width: 70

        text: "Статус"
        font.pixelSize: 18
        color: Material.accent

        visible: true
    }

    ProgressBar {
        id: muxingStateProgressBar

        x: 15
        y: parent.height - height - 10

        width: parent.width - 30

        value: 0

        background: Rectangle {
            implicitHeight: 22
            color: "#e6e6e6"
            radius: 3
        }

        contentItem: Item {
            implicitHeight: 20

            Rectangle {
                width: muxingStateProgressBar.visualPosition * parent.width
                height: parent.height
                radius: 2
                color: Material.accent
            }
        }
    }
}
