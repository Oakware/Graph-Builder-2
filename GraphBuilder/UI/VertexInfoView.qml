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
import QtQuick.Window 2.2
import GraphEngine 1.0

Dialog{
	property GraphModel graphModel
	property int verIndex
	property VertexInfo vertexInfo: VertexInfo{}

	id: vertexInfoRoot
	modality: Qt.ApplicationModal
	title: qsTr("Vertex info")

	Component.onCompleted: {
		cBridge.getVertexInfo(graphModel, vertexInfo, verIndex)
		radSB.value = vertexInfo.radius
		labelEdit.text = vertexInfo.text
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

			Text{text: qsTr("Number:")}
			Text{text: vertexInfo.number}

			Text{text: qsTr("x:")}
			Text{text: vertexInfo.x}

			Text{text: qsTr("y:")}
			Text{text: vertexInfo.y}

			Text{text: qsTr("Degree:")}
			Text{text: vertexInfo.degree}

			Text{text: qsTr("Radius:")}
			SpinBox{
				id: radSB
				Layout.fillWidth: true
				minimumValue: 5; maximumValue: 100
				onValueChanged: {
					if(vertexInfo.x-value >= 0 && vertexInfo.y-value >= 0){
						if(vertexInfo.radius == value)
							return
						vertexInfo.radius = value
						cBridge.setVertexInfo(graphModel, vertexInfo, verIndex)
					}else
						value = vertexInfo.radius
				}
			}

			Text{text: qsTr("Color:")}
			RowLayout{
				Layout.fillWidth: true; spacing: 5
				Text{text: vertexInfo.color; Layout.fillWidth: true}
				Button{text: ".."; onClicked: colDialog.change()}
			}

			Text{text: qsTr("Label:")}
			TextField{
				id: labelEdit
				Layout.fillWidth: true
				onTextChanged: {
					if(vertexInfo.text == text)
						return
					vertexInfo.text = text
					cBridge.setVertexInfo(graphModel, vertexInfo, verIndex)
				}
			}

			Item{width: 1; height: 5} Item{width: 1; height: 5}
		}

		ColorDialog{
			property color oldColor
			id: colDialog

			function change(){
				color = oldColor = vertexInfo.color
				open()
			}

			function stopEdit(){
				if(visible){
					vertexInfo.color = oldColor
					cBridge.setVertexInfo(graphModel, vertexInfo, verIndex)
					close()
				}
			}

			onColorChanged: {
				vertexInfo.color = color
				cBridge.setVertexInfo(graphModel, vertexInfo, verIndex)
			}

			onRejected: {
				vertexInfo.color = oldColor
				cBridge.setVertexInfo(graphModel, vertexInfo, verIndex)
			}
		}

		Keys.onReturnPressed: close()
		Keys.onEscapePressed: close()
	}
}
