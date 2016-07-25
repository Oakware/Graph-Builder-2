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
	id: aboutRoot
	title: qsTr("About")

	contentItem: Rectangle{
		implicitWidth: 400
		implicitHeight: 200
		color: "#ECF0F1"

		Image{
			id: appImage
			width: 150; height: 150
			anchors.top: parent.top
			anchors.left: parent.left
			anchors.margins: 10

			mipmap: true
			fillMode: Image.PreserveAspectFit
			source: "qrc:/AppIcon.png"
		}

		Item{
			id: aboutItem
			anchors.top: parent.top
			anchors.bottom: parent.bottom
			anchors.left: appImage.right
			anchors.right: parent.right
			anchors.margins: 10

			ScrollView{
				anchors.fill: parent
				horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff

				Text{
					width: aboutItem.width
					anchors.top: parent.top
					anchors.left: parent.left
					font.pixelSize: 13

					wrapMode: Text.Wrap
					horizontalAlignment: Text.AlignHCenter

					text: qsTr( "<b>%1</b><br><i>version %2</i>").arg(qsTr("Graph Builder")).arg("2.0.2") + "<br><br>" +
						  qsTr( "Created for building graphs and visualizing algorithms") + "<br>" +
						  qsTr( "Created by <a href=\"mailto:i.o.kondratuk@gmail.com?Subject=Graph%20Builder\">Ivan Kondratuk</a>") + "<br>" +
						  qsTr( "Links: ") +
						  qsTr(		"<a href=\"https://oakware.github.io/Graph-Builder-2\">Project Page</a>") + ", " +
						  qsTr(		"<a href=\"https://github.com/Oakware/Graph-Builder-2\">Github</a>") + ", " +
						  qsTr(		"<a href=\"https://github.com/Oakware/Graph-Builder-2/wiki\">Tutorial</a>") + "<br><br>" +

								"<i>Licensed under GNU GPLv3<br>" +
								"Copyright © 2016 <a href=\"https://github.com/Oakware\">Oakware</a><br>" +
								"All Rights Reserved</i>"

					onLinkActivated: Qt.openUrlExternally(link)
				}
			}
		}

		Button{
			anchors.horizontalCenter: appImage.horizontalCenter
			anchors.bottom: parent.bottom
			anchors.margins: 10
			text: qsTr("OK")
			onClicked: close()
		}

		Keys.onReturnPressed: close()
		Keys.onEscapePressed: close()
	}
}
