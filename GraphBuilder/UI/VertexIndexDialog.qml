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
import QtQuick.Layouts 1.2

Dialog{
	property int verCount: 1
	id: renameRoot
	title: qsTr("Vertex index")

	function apply(){
		close()
		algoStart(verIndex.value)
	}

	contentItem: Rectangle{
		implicitWidth: 200
		implicitHeight: 100
		color: "#ECF0F1"

		RowLayout{
			anchors.top: parent.top; anchors.bottom: buttonsRow.top
			anchors.left: parent.left; anchors.right: parent.right
			anchors.margins: 5

			Text{text: qsTr("Start index:")}
			SpinBox{
				id: verIndex
				Layout.fillWidth: true
				minimumValue: 1; maximumValue: verCount
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

		Keys.onReturnPressed: apply()
		Keys.onEscapePressed: close()
	}
}
