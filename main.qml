import QtQuick 2.6
import QtQuick.Window 2.2

import QtQuick.Layouts 1.3

import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3
import QtQuick.Dialogs 1.2

import FFMpeg.Wrapper 1.0

Window {
    visible: true
    width: 800
    height: 600

    minimumWidth: 640
    minimumHeight: 480

    title: qsTr("Конвертер медиафайлов")

    FontLoader { id: fixedFont; source: "/fonts/Roboto-Regular.ttf" }

    Converter {
        id: converter

        inputFilename: filenameField.text
        outputFilename: outFilenameField.text

        property bool twoPass: false
        property bool secondPass: false

        onMediaDataLoaded: {
            videoBitrateField.text = videoBitrate.toString()
            frameWidthField.text = frameWidth.toString()
            frameHeightField.text = frameHeight.toString()
        }

        onMuxStateUpdated: {
            var c_percent = 0;
            if(!twoPass)
                c_percent = percent
            else
                c_percent = (secondPass) ? (50 + percent / 2) : (percent / 2)

            muxingStateProgressBar.value = c_percent
        }

        onMuxStarted: {
            twoPass = two_pass
            secondPass = false

            muxingStateLabel.text = (twoPass) ? "Статус: Первый проход мультиплексирования" : "Статус: Мультиплексирование"
        }

        onFirstPassFinished: {
            secondPass = true

            muxingStateLabel.text = "Статус: Мультиплексирование"
        }

        onMuxingFinished: {
            muxingStateProgressBar.value = 100
            twoPass = false
            secondPass = false

            muxingStateLabel.text = "Статус: Мультиплексирование завершено"
        }
    }

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

        onTextChanged: {
            converter.inputFilename = text
        }
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

        model: converter.subtitleModel()

        anchors.top: subtitlesMetaDataLabel.bottom
        anchors.left: subtitlesMetaDataLabel.left

        width: halfWidth - 20

        delegate: Item {
            id: subtitleDelegate

            height: 50
            width: parent.width

            Text {
                id: subtitleIdLabel

                height: parent.height
                width: 20

                x: 0
                y: 0

                verticalAlignment: Qt.AlignVCenter

                text: "#" + index
                font.pixelSize: 16
            }

            TextField {
                id: subtitleTitleField

                anchors.left: subtitleIdLabel.right
                anchors.leftMargin: 10
                anchors.right: subtitleLanguageBox.left
                anchors.rightMargin: 10
                anchors.top: subtitleIdLabel.top

                height: parent.height

                placeholderText: "Название субтитров"
                font.pixelSize: 14

                Component.onCompleted: {
                    text = title
                }

                onTextChanged: {
                    title = text
                }
            }

            ComboBox {
                id: subtitleLanguageBox

                anchors.right: subtitleDeleteButton.left
                anchors.rightMargin: 10
                anchors.top: subtitleTitleField.top

                height: subtitleDelegate.height
                width: 140

                flat: true

                model: ListModel {
                    id: countriesModel

                    ListElement {
                        name: "Не определено"
                        source: "icons/united-nations.png"
                    }

                    ListElement {
                        name: "Русский"
                        source: "icons/russia.png"
                    }

                    ListElement {
                        name: "Английский"
                        source: "icons/united-kingdom.png"
                    }
                }

                delegate: ItemDelegate {
                    width: parent.width

                    text: name
                    font.pixelSize: 12

                    icon.name: "countryIcon"
                    icon.source: source
                    icon.height: 30
                    icon.width: 30
                    icon.color: "transparent"
                }

                textRole: "name"
                font.pixelSize: 14

                Component.onCompleted: {
                    if(language == "rus")
                        currentIndex = 1;
                    if(language == "eng")
                        currentIndex = 2;
                }

                onActivated: {
                    var lang
                    switch(currentIndex) {
                    case 0:
                        lang = ""
                        break
                    case 1:
                        lang = "rus"
                        break
                    case 2:
                        lang = "eng"
                        break
                    }

                    language = lang
                }
            }

            Button {
                id: subtitleDeleteButton

                anchors.top: subtitleLanguageBox.top
                anchors.right: parent.right

                height: parent.height
                width: 80

                text: "Удалить"
                font.pixelSize: 14

                onClicked: {
                    converter.subtitleModel().removeRows(index, 1)
                }
            }
        }

        onCountChanged: {
            height = count * 50
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

        onClicked: {
            subtitleFileDialog.visible = true
        }
    }

    FileDialog {
        id: subtitleFileDialog

        title: "Выберите файл субтитров"
        folder: shortcuts.home

        nameFilters: [ "Субтитры (*.srt)"]

        selectExisting: true
        selectMultiple: false

        onAccepted: {
            var filename = subtitleFileDialog.fileUrl
            var subsUrl = filename.toString().replace("file:///", "");

            converter.subtitleModel().addSubtitle(subsUrl)

            setVisible(false)
        }
        onRejected: {
            setVisible(false)
        }
    }

    Button {
        id: multiplexButton

        property var parentToAnchor: videoBitrateField;

        anchors.top: parentToAnchor.bottom
        anchors.topMargin: 10
        anchors.left: videoBitrateField.left

        width: 250
        height: 50

        text: "Мультиплексировать"
        font.pixelSize: 18

        onClicked: {
            converter.frameWidth = frameWidthField.text
            converter.frameHeight = frameHeightField.text
            converter.videoBitrate = videoBitrateField.text

            converter.mux()
        }
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

        text: qsTr("Заменить исходный файл")

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

        onTextChanged: {
            converter.outputFilename = text
        }
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

        to: 100

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
