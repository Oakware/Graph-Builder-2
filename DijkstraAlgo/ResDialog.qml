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
import "modelSort.js" as ModelSort

Dialog{
	property QtObject algoPlugin
	title: qsTr("Result")
	modality: Qt.ApplicationModal

	Component.onCompleted: {
		var cnt = algoPlugin.getResultCount()
		for(var i=1; i<cnt; ++i){
			var dis = algoPlugin.getResultDistance(i)
			var ver = algoPlugin.getResultVertex(i)
			dijResModel.append({vertex: i, vertexName: ver, distance: dis})
		}
		resTable.resizeColumnsToContents()
	}

	onVisibleChanged: {
		if(!visible)
			endAlgo()
	}

	function resetPath(row){
		var ver = dijResModel.get(row).vertex
		algoPlugin.resetPath(ver)
	}

	function sort(){
		if(resTable.sortIndicatorOrder == Qt.AscendingOrder){
			if(resTable.sortIndicatorColumn == 0)
				 ModelSort.mSort(dijResModel, function(a, b)
					{return a.vertexName > b.vertexName})
			else ModelSort.mSort(dijResModel, function(a, b)
					{return a.distance   > b.distance})
		}else{
			if(resTable.sortIndicatorColumn == 0)
				 ModelSort.mSort(dijResModel, function(a, b)
					{return a.vertexName < b.vertexName})
			else ModelSort.mSort(dijResModel, function(a, b)
					{return a.distance   < b.distance})
		}
	}

	ListModel{id: dijResModel; dynamicRoles: true}

	contentItem: Rectangle{
		implicitWidth: 250
		implicitHeight: 300
		color: "#ECF0F1"

		TableView{
			id: resTable
			anchors.top: parent.top
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.bottom: dialogButtons.top
			anchors.margins: 10

			sortIndicatorVisible: true
			model: dijResModel

			TableViewColumn{
				width: 100
				role: "vertexName"
				title: qsTr("Vertex")
			}

			TableViewColumn{
				width: 100
				role: "distance"
				title: qsTr("Distance")
			}

			onClicked: resetPath(row)
			onSortIndicatorColumnChanged: sort()
			onSortIndicatorOrderChanged: sort()
		}

		Button{
			id: dialogButtons
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.bottom: parent.bottom
			anchors.margins: 10
			text: qsTr("OK")
			onClicked: close()
		}

		Keys.onReturnPressed: close()
		Keys.onEscapePressed: close()
	}
}
