/*****************************************************************************
 * Copyright © 2016 Oakware - All Rights Reserved
 *
 * Github : <https://github.com/Oakware>
 * Authors: Ivan Kondratuk <i.o.kondratuk@gmail.com>
 *
 * This file is part of Graph Builder.
 *
 * Graph Builder is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Graph Builder is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Graph Builder.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2
import GraphEngine 1.0

Dialog{
	property color gvBackgroundColor
	id: settingsRoot
	title: qsTr("Settings")

	Component.onCompleted: {
		langCB.currentIndex = cBridge.getTranslation()
		speedSlider.value   = 1000 - cBridge.getPluginLoader().algoDelay

		gvEditableCB.checked   = cBridge.getProperty("graphView/editable", 1)==1
		gvBackAppColCB.checked = cBridge.getProperty("graphView/backAppColor", 0)==1
		gvColorRow.gvBackCol   = cBridge.getProperty("graphView/backColor", "white")

		ribStdWgt.value   = cBridge.getProperty("graphView/stdRib/weight", 10)
		gvStdRCRow.stdCol = cBridge.getProperty("graphView/stdRib/color", "black")
		verStdRad.value   = cBridge.getProperty("graphView/stdVertex/radius", 20)
		gvStdVCRow.stdCol = cBridge.getProperty("graphView/stdVertex/color", "#F13746")
	}

	onVisibleChanged: {
		if(!visible){
			colDialog.close()
			updateSettings()
		}
	}

	function apply(){
		cBridge.setProperty("appColor", appColor)
		cBridge.setTranslation(langCB.currentIndex)
		cBridge.getPluginLoader().algoDelay = 1000 - speedSlider.value

		cBridge.setProperty("graphView/editable", (gvEditableCB.checked)? 1:0)
		cBridge.setProperty("graphView/backAppColor", (gvBackAppColCB.checked)? 1:0)
		cBridge.setProperty("graphView/backColor", gvColorRow.gvBackCol)

		cBridge.setProperty("graphView/stdRib/weight", ribStdWgt.value)
		cBridge.setProperty("graphView/stdRib/color",  gvStdRCRow.stdCol)
		cBridge.setProperty("graphView/stdVertex/radius", verStdRad.value)
		cBridge.setProperty("graphView/stdVertex/color",  gvStdVCRow.stdCol)

		close()
	}

	contentItem: Rectangle{
		id: contentRoot
		implicitWidth: 320
		implicitHeight: 300
		color: "#ECF0F1"

		ScrollView{
			anchors.fill: parent
			anchors.bottomMargin: buttonsRow.height+10

			ColumnLayout{
				width: contentRoot.width-20
				anchors.left: parent.left
				anchors.margins: 10

				Item{width: 1; height: 5}
				Text{
					Layout.alignment: Qt.AlignHCenter
					font.bold: true
					text: qsTr("Gui")
				}

				Text{
					Layout.alignment: Qt.AlignHCenter
					text: qsTr("Color")
				}

				Flow{
					Layout.alignment: Qt.AlignTop
					Layout.fillWidth: true
					spacing: 4
					Repeater{
						delegate: colorRect
						model: ["#F44336", "#FF5722", "#FF9800", "#4CAF50", "#009688",
								"#00BCD4", "#2196F3", "#3F51B5", "#673AB7", "#9C27B0"]
					}
				}

				Rectangle{Layout.fillWidth: true; height: 2}		// ***
				GridLayout{
					columns: 2
					Text{text: qsTr("Language")}
					ComboBox{
						id: langCB
						Layout.alignment: Qt.AlignHCenter
						model: ["English", "Українська", "Русский"]
					}

					Text{text: qsTr("Algorithm speed")}
					Slider{
						id: speedSlider
						Layout.fillWidth: true
						minimumValue: 0; maximumValue: 900
						tickmarksEnabled: true; stepSize: 50
					}
				}

				Rectangle{Layout.fillWidth: true; height: 2}		// Graph
				Text{
					Layout.alignment: Qt.AlignHCenter
					font.bold: true
					text: qsTr("Graph")
				}

				CheckBox{
					id: gvEditableCB
					text: qsTr("Editable")
				}

				CheckBox{
					id: gvBackAppColCB
					text: qsTr("Background as app color")
				}

				RowLayout{
					id: gvColorRow
					property color gvBackCol
					Layout.fillWidth: true; spacing: 5
					enabled: !gvBackAppColCB.checked

					Text{text: qsTr("Background:")}
					Text{text: gvColorRow.gvBackCol; Layout.fillWidth: true}
					Button{text: ".."; onClicked: colDialog.change(gvColorRow.gvBackCol,
													function(col){gvColorRow.gvBackCol=col})}
				}

				Rectangle{Layout.fillWidth: true; height: 2}		// Vertex
				Text{
					Layout.alignment: Qt.AlignHCenter
					text: qsTr("Vertex")
				}

				GridLayout{
					columns: 2
					Text{text: qsTr("Radius:")}
					SpinBox{
						id: verStdRad
						minimumValue: 5; maximumValue: 100
					}

					Text{text: qsTr("Color:")}
					RowLayout{
						id: gvStdVCRow
						property color stdCol
						Layout.fillWidth: true; spacing: 5

						Text{text: gvStdVCRow.stdCol; Layout.fillWidth: true}
						Button{text: ".."; onClicked: colDialog.change(gvStdVCRow.stdCol,
														function(col){gvStdVCRow.stdCol=col})}
					}
				}

				Rectangle{Layout.fillWidth: true; height: 2}		// Rib
				Text{
					Layout.alignment: Qt.AlignHCenter
					text: qsTr("Rib")
				}

				GridLayout{
					columns: 2
					Text{text: qsTr("Weight:")}
					SpinBox{
						id: ribStdWgt
						minimumValue: 1; maximumValue: 10000
					}

					Text{text: qsTr("Color:")}
					RowLayout{
						id: gvStdRCRow
						property color stdCol
						Layout.fillWidth: true; spacing: 5

						Text{text: gvStdRCRow.stdCol; Layout.fillWidth: true}
						Button{text: ".."; onClicked: colDialog.change(gvStdRCRow.stdCol,
														function(col){gvStdRCRow.stdCol=col})}
					}
				}

				Item{width: 1; height: 5}
			}
		}

		Row{
			id: buttonsRow
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.bottom: parent.bottom
			anchors.margins: 10
			spacing: 5

			Button{
				text: qsTr("Cancel")
				onClicked: close()
			}

			Button{
				text: qsTr("OK")
				onClicked: apply()
			}
		}

		ColorDialog{
			property var resFunc
			id: colDialog

			onAccepted: resFunc(color)
			function change(oldCol, rf){
				color = oldCol
				resFunc = rf
				open()
			}
		}

		Keys.onReturnPressed: apply()
		Keys.onEscapePressed: close()
	}

	Component{
		id: colorRect
		Rectangle{
			width: 25; height: width
			radius: width/2
			color: modelData
			border.width: (appColor === color)? height/7:0
			border.color: "skyblue"

			Behavior on border.width{ NumberAnimation{duration: 100; easing.type: Easing.InOutSine} }
			MouseArea{anchors.fill: parent; onClicked: appColor = color}
		}
	}
}
