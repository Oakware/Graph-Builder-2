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
	property GraphModel graphModel
	property int verA
	property int verB
	property RibInfo ribInfo: RibInfo{}

	id: ribInfoRoot
	modality: Qt.ApplicationModal
	title: qsTr("Rib info")

	Component.onCompleted: {
		cBridge.getRibInfo(graphModel, ribInfo, verA, verB)
		widSB.value = ribInfo.weight
		labelEdit.text = ribInfo.text
	}

	onVisibleChanged: {
		if(!visible)
			colDialog.stopEdit()
	}

	contentItem: Rectangle{
		id: contentRoot
		implicitWidth: 200
		implicitHeight: contentGrid.height
		color: "#ECF0F1"

		GridLayout{
			id: contentGrid
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.margins: 7
			columns: 2; columnSpacing: 10

			Item{width: 1; height: 5} Item{width: 1; height: 5}

			Text{text: qsTr("From:")}
			Text{text: ribInfo.from}

			Text{text: qsTr("To:")}
			Text{text: ribInfo.to}

			Text{text: qsTr("Weight:")}
			SpinBox{
				id: widSB
				Layout.fillWidth: true
				minimumValue: 1; maximumValue: 10000
				onValueChanged: {
					if(ribInfo.from > 0){
						if(ribInfo.weight == value)
							return
						ribInfo.weight = value
						cBridge.setRibInfo(graphModel, ribInfo, verA, verB)
					}
				}
			}

			Text{text: qsTr("Color:")}
			RowLayout{
				Layout.fillWidth: true; spacing: 5
				Text{text: ribInfo.color; Layout.fillWidth: true}
				Button{text: ".."; onClicked: colDialog.change()}
			}

			Text{text: qsTr("Label:")}
			TextField{
				id: labelEdit
				Layout.fillWidth: true
				onTextChanged: {
					if(ribInfo.text == text)
						return
					ribInfo.text = text
					cBridge.setRibInfo(graphModel, ribInfo, verA, verB)
				}
			}

			Item{width: 1; height: 5} Item{width: 1; height: 5}
		}

		ColorDialog{
			property color oldColor
			id: colDialog

			function change(){
				color = oldColor = ribInfo.color
				open()
			}

			function stopEdit(){
				if(visible){
					ribInfo.color = oldColor
					cBridge.setRibInfo(graphModel, ribInfo, verA, verB)
					close()
				}
			}

			onColorChanged: {
				ribInfo.color = color
				cBridge.setRibInfo(graphModel, ribInfo, verA, verB)
			}

			onRejected: {
				ribInfo.color = oldColor
				cBridge.setRibInfo(graphModel, ribInfo, verA, verB)
			}
		}

		Keys.onReturnPressed: close()
		Keys.onEscapePressed: close()
	}
}
