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

Rectangle{
	property int	posX
	property int	posY
	property double length
	property double alpha
	property int	weight
	property color	col
	property string rtext
	property bool isOriented:	 false
	property bool isSelected:	 false
	property bool isHidden:		 true
	property bool isTextVisible: false
	property bool isTextBlack:	 true

	antialiasing: true
	x: posX; y: posY; z: (isSelected)? 1:0
	width: length; height: (isSelected)? 3:1
	color: col; opacity: (isHidden)? 0:1
	transformOrigin: Item.Left; rotation: alpha

	Behavior on color{ ColorAnimation{duration: 100} }
	Behavior on height{ NumberAnimation{duration: 100} }
	Behavior on opacity{ NumberAnimation{duration: 200} }

	Rectangle{
		antialiasing: true
		width: 20; height: parent.height
		anchors.right: parent.right
		color: parent.color
		transformOrigin: Item.Right; rotation: (isOriented)? 25:0

		Behavior on rotation{ NumberAnimation{duration: 100} }
	}

	Rectangle{
		antialiasing: true
		visible: isOriented
		width: 20; height: parent.height
		anchors.right: parent.right
		color: parent.color
		transformOrigin: Item.Right; rotation: (isOriented)? -25:0

		Behavior on rotation{ NumberAnimation{duration: 100} }
	}

	Text{
		anchors.bottom: parent.top
		anchors.left: parent.horizontalCenter
		visible: isTextVisible; rotation: -alpha
		font.bold: isSelected
		color: (isTextBlack)? "darkred":"white"
		text: (rtext.length)? rtext:weight
	}
}
