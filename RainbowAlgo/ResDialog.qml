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

Dialog{
	property QtObject algoPlugin
	title: qsTr("Result")
	modality: Qt.ApplicationModal

	Component.onCompleted: {
		resText.text = algoPlugin.getResult()
	}

	onVisibleChanged: {
		if(!visible)
			endAlgo()
	}

	contentItem: Rectangle{
		implicitWidth: 200
		implicitHeight: resText.height+dialogButtons.height+30
		color: "#ECF0F1"

		Text{
			id: resText
			anchors.top: parent.top
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.margins: 10

			wrapMode: Text.Wrap
			horizontalAlignment: Text.AlignHCenter
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
