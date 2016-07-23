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
import GraphEngine 1.0
import QtGraphicalEffects 1.0

Item{
	property QtObject headerProp: panelProp.createObject()
	property alias graphView: graphView
	id: gvItemRoot

	Component.onCompleted: {
		headerProp.panelTitle = qsTr("Graph Builder")
		setPanelButton()
	}

	function additionalFunction(){
		if(graphView.selectedVertex > 0)
			loadWindow("qrc:/VertexInfoView.qml",
					   {graphModel: graphView.graph,
						verIndex: graphView.selectedVertex})
		else if(graphView.selectedRib[0] > 0)
			loadWindow("qrc:/RibInfoView.qml",
					   {graphModel: graphView.graph,
						verA: graphView.selectedRib[0],
						verB: graphView.selectedRib[1]})
		else if(running) stopAlgo()
		else runAlgo()
	}

	function setPanelButton(){
		if(graphView.selectedVertex > 0 || graphView.selectedRib[0] > 0)
			headerProp.additionalBtnSource = "qrc:/Functional/Info.png"
		else if(running)
			headerProp.additionalBtnSource = "qrc:/Functional/Stop.png"
		else
			headerProp.additionalBtnSource = "qrc:/Functional/Run.png"
	}

	focus: true
	Keys.onReturnPressed: {
		if(graphView.selectedVertex > 0 || graphView.selectedRib[0] > 0)
			additionalFunction()
	}

	ScrollView{
		anchors.fill: parent
		horizontalScrollBarPolicy: (graphView.horSBvis)? Qt.ScrollBarAsNeeded:Qt.ScrollBarAlwaysOff
		verticalScrollBarPolicy:   (graphView.verSBvis)? Qt.ScrollBarAsNeeded:Qt.ScrollBarAlwaysOff

		GraphView{
			property bool horSBvis: (minimumSize.width  > gvItemRoot.width)
			property bool verSBvis: (minimumSize.height > gvItemRoot.height)

			id: graphView
			width:  (horSBvis)? minimumSize.width  : gvItemRoot.width
			height: (verSBvis)? minimumSize.height : gvItemRoot.height

			Behavior on backgroundColor{ ColorAnimation{duration: 100} }

			onRibSelected: setPanelButton()
			onVertexSelected: setPanelButton()

			onContextMenu: {
				if(selectedVertex > 0)
					vertexMenu.popup()
				else if(selectedRib[0] > 0)
					ribMenu.popup()
				else
					otherMenu.popup()
			}
		}
	}

	Menu{				// Menus
		id: vertexMenu
		MenuItem{
			text: qsTr("Add rib")
			iconSource: "qrc:/Functional/Add.png"
			onTriggered: graphView.cmAddRibBegin()
		}

		MenuItem{
			text: qsTr("Select rib")
			onTriggered: graphView.cmSelectRibBegin()
		}

		MenuItem{
			text: qsTr("Remove rib")
			iconSource: "qrc:/Functional/Remove.png"
			onTriggered: graphView.cmRemoveRibBegin()
		}

		MenuItem{
			text: qsTr("Edit vertex")
			onTriggered: additionalFunction()
		}

		MenuItem{
			text: qsTr("Remove vertex")
			iconSource: "qrc:/Functional/Remove.png"
			onTriggered: graphView.cmRemoveVertex()
		}
	}

	Menu{
		id: ribMenu
		MenuItem{
			text: qsTr("Edit rib")
			onTriggered: additionalFunction()
		}

		MenuItem{
			text: qsTr("Remove rib")
			iconSource: "qrc:/Functional/Remove.png"
			onTriggered: graphView.cmRemoveRib()
		}
	}

	Menu{
		id: otherMenu
		MenuItem{
			text: qsTr("Add vertex")
			iconSource: "qrc:/Functional/Add.png"
			onTriggered: graphView.cmAddVertex()
		}

		MenuItem{
			text: qsTr("Oriented")
			checkable: true
			checked: (graphView.graph)? graphView.graph.oriented:false
			onTriggered: {graphView.graph.oriented
						  = !graphView.graph.oriented}
		}

		MenuItem{
			text: qsTr("Clear")
			onTriggered: graphView.graph.clear()
		}
	}
}
