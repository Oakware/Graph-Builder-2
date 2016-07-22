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
import QtQuick.Dialogs 1.2
import GraphEngine 1.0
import "modelSort.js" as ModelSort

Dialog{
	property PluginLoader plugLoader: cBridge.getPluginLoader()
	property bool removeState: false

	id: aboutRoot
	modality: Qt.ApplicationModal
	title: qsTr("Plugins manager")

	Component.onCompleted: {
		var plist = plugLoader.getNames()
		for(var i=0; i<plist.length; ++i)
			addPlugItem(plist[i])
	}

	onVisibleChanged: {
		if(!visible)
			updatePlugins("")
	}

	function addPlugItem(name){
		var pos = ModelSort.searchInsertPos(plugListModel, name,
						function(pi, nm){return pi.name >= nm})
		var pst = plugLoader.getPlugState(name)
		if(pos < plugListModel.count && plugListModel.get(pos).name == name)
			plugListModel.get(pos).plugState = pst
		else
			plugListModel.insert(pos, {"name": name, "plugState": pst})
	}

	contentItem: Rectangle{
		implicitWidth: 300
		implicitHeight: 350
		color: "#ECF0F1"

		ListView{
			id: plugListView
			anchors.fill: parent
			anchors.topMargin: plugPanel.height

			delegate: plugListItem
			model: ListModel{id: plugListModel}
		}

		MenuPanel{
			id: plugPanel
			width: parent.width; height: 40
			panelTitle: title
			panelColor: appColor

			menuBtnVisible: false
			additionalBtnSource: (removeState)? "qrc:/Functional/Done.png":
												"qrc:/Functional/Gear.png"
			onAdditionalClicked: removeState = !removeState
		}

		RoundButton{
			width: 40; height: 40
			anchors.bottom: parent.bottom
			anchors.right: parent.right
			anchors.margins: 15

			visible: !removeState
			buttonColor: "#2ECC71"
			mouseArea.onClicked: fileDialog.open()
		}

		MessageDialog{
			property int delInd
			property string delName

			id: removeDialog
			title: qsTr("Plugin remove")
			standardButtons: StandardButton.Yes|StandardButton.No

			function plugRemove(ind, name){
				delInd = ind; delName = name
				text = qsTr("Do you want to remove %1 plugin?").arg(name)
				open()
			}

			onYes: {
				plugLoader.removePlugin(delName)
				plugListModel.get(delInd).plugState = plugLoader.getPlugState(delName)
			}
		}

		FileDialog{
			id: fileDialog
			title: qsTr("Open file")
			selectMultiple: true

			Component.onCompleted: {
				folder = shortcuts.documents
				var pl = cBridge.getPlatform()
				var ap = qsTr("Algo plugin")
				if(pl == "windows")
					nameFilters = [ ap + " (*.dll)"   ]
				else if(pl == "osx")
					nameFilters = [ ap + " (*.dylib)" ]
				else
					nameFilters = [ ap + " (*.so)"	  ]
			}

			onAccepted: {
				for(var i=0; i<fileUrls.length; ++i){
					var name = plugLoader.loadPlugin(fileUrls[i])
					if(name.length) addPlugItem(name)
				}
			}
		}

		Keys.onReturnPressed: close()
		Keys.onEscapePressed: close()
	}

	Component{
		id: plugListItem
		Rectangle{
			width: parent.width
			height: 45

			Text {
				anchors.verticalCenter: parent.verticalCenter
				anchors.left: parent.left
				anchors.right: removeBtn.left
				anchors.margins: 10

				elide: Text.ElideRight
				font.pixelSize: parent.height/3
				text: name + ((plugState == PluginLoader.Reload)?
								("<br><i>"+qsTr("Reload after restart programm")+"</i>"):
							 (plugState == PluginLoader.Remove)?
								("<br><i>"+qsTr("Remove after restart programm")+"</i>"):"")
			}

			ImageButton{		// Remove button
				id: removeBtn
				width: parent.height/2
				height: width
				anchors.verticalCenter: parent.verticalCenter
				anchors.right: parent.right
				anchors.rightMargin: 5
				visible: removeState

				imageSource: "qrc:/Functional/Remove.png"
				mouseArea.onClicked: removeDialog.plugRemove(index, name)
			}

			Rectangle{			// Borders
				width: parent.width; height: 1
				visible: index != 0
				color: "lightgray"
			}

			Rectangle{			// Shadow
				width: parent.width; height: 3
				anchors.bottom: parent.top
				visible: index == 0

				gradient: Gradient{
					GradientStop{position: 1; color: "#60000000"}
					GradientStop{position: 0; color: "transparent"}
				}
			}

			Rectangle{
				width: parent.width; height: 3
				anchors.top: parent.bottom
				visible: plugListView.count == index+1

				gradient: Gradient{
					GradientStop{position: 0; color: "#60000000"}
					GradientStop{position: 1; color: "transparent"}
				}
			}
		}
	}
}
