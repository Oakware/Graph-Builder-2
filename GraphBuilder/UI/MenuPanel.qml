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

Rectangle{
	property string panelTitle: ""
	property color panelColor: "#FFFFFF"
	property string additionalBtnSource: ""
	property Menu panelMenu

	property bool titleVisible: true
	property bool shadowVisible: true
	property bool backBtnVisible: false
	property bool menuBtnVisible: true

	signal backClicked()
	signal additionalClicked()

	id: panelRoot
	color: panelColor
	Behavior on color{ ColorAnimation{duration: 100} }

	ImageButton{			// <- icon
		id: backBtn

		width: (backBtnVisible)? parent.height:0
		height: width
		visible: backBtnVisible

		imageSource: "qrc:/Functional/Arrow_left.png"
		imageMargins: panelRoot.height/4
		mouseArea.onClicked: backClicked()
	}

	Text{			// Title
		anchors.verticalCenter: parent.verticalCenter
		anchors.left: backBtn.right
		anchors.right: buttonsRow.left
		anchors.leftMargin: panelRoot.height/4

		visible: titleVisible
		font.pixelSize: panelRoot.height/3
		elide: Text.ElideRight
		color: "white"
		text: panelTitle
	}

	Row{
		id: buttonsRow

		height: parent.height
		anchors.right: parent.right

		ImageButton{		// Additional button
			width: parent.height
			height: parent.height
			visible: additionalBtnSource.length > 0

			imageSource:  additionalBtnSource
			imageMargins: panelRoot.height/4
			mouseArea.onClicked: additionalClicked()
		}

		ImageButton{		// Menu button
			width: parent.height/1.5
			height: parent.height
			visible: menuBtnVisible

			imageSource: "qrc:/Functional/More.png"
			imageMargins: panelRoot.height/10

			mouseArea.onClicked: panelMenu.popup()
		}
	}

	Rectangle{			// Shadow
		width: parent.width; height: panelRoot.height/15
		anchors.top: parent.bottom
		visible: shadowVisible

		gradient: Gradient{
			GradientStop{position: 0; color: "#60000000"}
			GradientStop{position: 1; color: "transparent"}
		}
	}
}
