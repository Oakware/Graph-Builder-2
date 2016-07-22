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
import QtGraphicalEffects 1.0

Item{
	readonly property alias mouseArea: buttonMouseArea
	property color buttonColor: "white"
	property color contentColor: "white"
	property int buttonDiameter: Math.min(width, height)

	property Component content: Item{	// + image
		Rectangle{
			anchors.centerIn: parent
			width: buttonDiameter*0.3
			height: buttonDiameter*0.04
			color: contentColor
		}

		Rectangle{
			anchors.centerIn: parent
			width: buttonDiameter*0.04
			height: buttonDiameter*0.3
			color: contentColor
		}
	}

	DropShadow{			// Shadow
		anchors.fill: round
		horizontalOffset: buttonDiameter*0.02
		verticalOffset:   buttonDiameter*0.02
		radius: buttonDiameter*0.2
		samples: radius*2

		color: "#60000000"
		source: round
	}

	Rectangle{			// Round
		id: round
		anchors.centerIn: parent
		width: buttonDiameter; height: width; radius: width/2
		color: buttonColor

		Behavior on color{ ColorAnimation{duration: 100} }

		Loader{
			anchors.fill: parent
			sourceComponent: content
		}

		Rectangle{
			anchors.centerIn: parent
			width: (buttonMouseArea.pressed)? buttonDiameter:0
			height: width; radius: width/2

			opacity: (buttonMouseArea.pressed)? 0.3:0
			Behavior on opacity { NumberAnimation{ duration: 50 } }
			Behavior on width   { NumberAnimation{ duration: 50 } }
		}

		MouseArea{
			id: buttonMouseArea
			anchors.fill: parent
		}
	}
}
