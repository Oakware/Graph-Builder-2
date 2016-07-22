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
	property int	number
	property int	posX
	property int	posY
	property int	rad: 0
	property color	col: "white"
	property string vtext
	property bool isSelected:	 false
	property bool isHidden:		 true
	property bool isTextBlack:	 true
	property bool isBorderBlack: true

	x: posX-rad; y: posY-rad; z: (isSelected)? 3:2
	width: 2*rad; height: 2*rad
	radius: rad; color: col
	opacity: (isHidden)? 0:1

	border.color: (isBorderBlack)? "black":"white"
	border.width: (isSelected)? 5:1

	Behavior on color{ ColorAnimation{duration: 100} }
	Behavior on rad{ NumberAnimation{duration: 100} }
	Behavior on border.width{ NumberAnimation{duration: 100} }
	Behavior on opacity{ NumberAnimation{duration: 200} }

	Text{
		width: parent.width; height: parent.height
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
		elide: Text.ElideMiddle

		color: (isTextBlack)? "black":"white"
		text: (vtext.length)? vtext:number
	}
}
