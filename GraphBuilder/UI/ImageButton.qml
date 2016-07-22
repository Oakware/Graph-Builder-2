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

Item{
	readonly property alias mouseArea: buttonMouseArea
	readonly property alias image: buttonImage

	property bool pressVisible: true
	property color pressColor: "white"
	property color pressBorderColor: "transparent"
	property int pressBorderWidth: 0

	property string imageSource: ""
	property int imageMargins: 0

	Image{			// Image
		id: buttonImage
		anchors.fill: parent
		anchors.margins: imageMargins
		mipmap: true
		fillMode: Image.PreserveAspectFit
		source: imageSource
	}

	Rectangle{		// Press rect
		property double diam: Math.min(parent.width, parent.height)/1.2
		anchors.centerIn: parent
		width: (buttonMouseArea.pressed)? diam:0
		height: width; radius: width/2
		color: pressColor
		border.color: pressBorderColor
		border.width: pressBorderWidth

		opacity: (buttonMouseArea.pressed && pressVisible)? 0.3:0
		Behavior on opacity { NumberAnimation{ duration: 100 } }
	}

	MouseArea{
		id: buttonMouseArea
		anchors.fill: parent
	}
}
