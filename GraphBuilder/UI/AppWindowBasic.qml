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
import QtQuick.Window 2.2

/*		Panel properties
 *	property QtObject headerProp: panelProp.createObject()
 *
 *		Button function
 *	function additionalFunction(){}
 *
 *		Layot background
 *	color: "#ECF0F1"
*/

ApplicationWindow {
	property alias appPanel: menuPanel
	property alias appStack: layerStack
	property alias panelProp: panelProperties

	property double globalZoom: 1
	property color appColor: "#4CAF50"

	function pushLayer(layer){
		if(!layerStack.busy)
			layerStack.push(layer)
	}

	function popLayer(){
		layerStack.pop()
	}

	toolBar: MenuPanel{		// Panel
		id: menuPanel
		width: parent.width
		height: 40*globalZoom

		backBtnVisible: layerStack.depth > 1
		menuBtnVisible: layerStack.depth == 1

		onBackClicked: popLayer()
		onAdditionalClicked: layerStack.currentItem.additionalFunction()
	}

	StackView{			// Layer stack
		id: layerStack
		anchors.fill: parent

		onCurrentItemChanged: {
			if(currentItem != null)
				setPanelParams(menuPanel, currentItem.headerProp)
		}
	}

	Component{		// Panel properties
		id: panelProperties
		QtObject{
			property string panelTitle: title
			property color panelColor: appColor
			property string additionalBtnSource: ""

			property bool titleVisible: true
			property bool shadowVisible: true
		}
	}

	function setPanelParams(mnPan, par){
		mnPan.panelTitle = Qt.binding(function(){return par.panelTitle})
		mnPan.panelColor = Qt.binding(function(){return par.panelColor})
		mnPan.additionalBtnSource = Qt.binding(function(){return par.additionalBtnSource})

		mnPan.titleVisible = Qt.binding(function(){return par.titleVisible})
		mnPan.shadowVisible = Qt.binding(function(){return par.shadowVisible})
	}
}
