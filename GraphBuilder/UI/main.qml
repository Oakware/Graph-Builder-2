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
import GraphEngine 1.0

AppWindowBasic{
	property bool running: false
	property QtObject runPlugin
	property QtObject runGraphModel

	property VertexInfo stdVertex: VertexInfo{}
	property RibInfo stdRib: RibInfo{}

	id: windowRoot
	title: qsTr("Graph Builder")
	visible: true

	Component.onCompleted: {
		graphVI.graphView.graph = graphModel
		width =  cBridge.getProperty("width",  600)
		height = cBridge.getProperty("height", 400)
		updateSettings()
		updatePlugins(cBridge.getProperty("selectedAlgo", ""))

		if(cBridge.isSaveFileExist())
			openFile()
	}

	onRunningChanged: graphVI.setPanelButton()
	onClosing: close.accepted = onExit()

	function onExit(){
		if(graphModel.changes && cBridge.isSaveFileExist()){
			closeDialog.closeFile(Qt.quit)
			return false
		}else if(running)
			return false
		return true
	}

	function resetTitle(){
		var sf = cBridge.getSaveFileName()
		if(!sf.length) sf = qsTr("Graph Builder")
		if(graphModel.changes) sf += "*"
		title = sf
	}

	function openFile(){
		if(cBridge.openFile(graphModel)){
			graphModel.changes = false
			resetTitle()
		}
	}

	function saveFile(){
		if(cBridge.isSaveFileExist()){
			var res = cBridge.saveFile(graphModel)
			graphModel.changes = !res
			resetTitle()
		}else
			fileDialog.choose(false)
	}

	function closeFile(){
		if(!cBridge.isSaveFileExist()) return
		if(graphModel.changes){
			closeDialog.closeFile(function()
				{graphModel.changes = false; closeFile()})
			return
		}

		cBridge.setSaveFile("")
		graphModel.clear()
		graphModel.changes = false
		resetTitle()
	}

	function runAlgo(){
		if(running || !graphModel.vertexCount) return
		if(algoExclGroup.current == null) return
		var sel = algoExclGroup.current.text

		runPlugin = cBridge.getPluginLoader().getPlugin(sel)
		if(runPlugin == null) return

		loadWindow("qrc:/VertexIndexDialog.qml", {verCount: graphModel.vertexCount})
	}

	function algoStart(index){
		running = true
		runGraphModel = graphModel.clone()
		graphVI.graphView.editable = false
		graphVI.graphView.graph = runGraphModel
		var res = runPlugin.run(runGraphModel, index)

		if(res) loadWindow(runPlugin.qmlDialog(),
						   {algoPlugin: runPlugin})
		else	endAlgo()
	}

	function endAlgo(){
		if(!running) return
		running = false
		windowLoader.setSource("")
		graphVI.graphView.graph = graphModel
		runGraphModel.destroy()
		runGraphModel = runPlugin = null
		updateSettings()
	}

	function stopAlgo(){
		if(runPlugin)
			runPlugin.stop()
	}

	function updatePlugins(sel){
		var mn, ms
		if(algoExclGroup.current && !sel.length)
			sel = algoExclGroup.current.text

		ms = null
		algoMenu.clear()
		var plist = cBridge.getPluginLoader().getNames()

		for(var i=0; i<plist.length; ++i){
			mn = algoMenu.addItem(plist[i])
			mn.checkable = true
			algoExclGroup.bindCheckable(mn)
			if(plist[i] === sel)
				ms = mn
		}

		if(!ms && algoMenu.items.length)
			ms = algoMenu.items[0]
		algoExclGroup.current = ms
	}

	function updateSettings(){
		appColor = cBridge.getProperty("appColor", "#2196F3")

		graphVI.graphView.editable = cBridge.getProperty("graphView/editable", 1)
		if(cBridge.getProperty("graphView/backAppColor", 0))
			graphVI.graphView.backgroundColor = Qt.binding(function(){return Qt.tint(appColor, "#50ffffff")})
		else
			graphVI.graphView.backgroundColor = cBridge.getProperty("graphView/backColor", "white")

		stdRib.weight = cBridge.getProperty("graphView/stdRib/weight", 10)
		stdRib.color  = cBridge.getProperty("graphView/stdRib/color", "black")
		cBridge.setStdRib(graphVI.graphView, stdRib)

		stdVertex.radius = cBridge.getProperty("graphView/stdVertex/radius", 20)
		stdVertex.color  = cBridge.getProperty("graphView/stdVertex/color", "#F13746")
		cBridge.setStdVertex(graphVI.graphView, stdVertex)
	}

	CBridge{		// Bridge
		id: cBridge
		onAboutToExit: {
			setProperty("width", width)
			setProperty("height", height)

			var sel
			if(algoExclGroup.current)
				sel = algoExclGroup.current.text
			setProperty("selectedAlgo", sel)
		}
	}

	GraphModel{		// Graph
		property bool changes: false
		id: graphModel
		onChanged: {
			changes = true
			resetTitle()
		}
	}

	MessageDialog{
		property var resFunc
		id: closeDialog
		title: qsTr("Closing")
		text: qsTr("Do you want to save graph?")
		icon: StandardIcon.Question
		standardButtons: StandardButton.Yes
						 | StandardButton.No
						 | StandardButton.Cancel

		function closeFile(rf){
			resFunc = rf
			open()
		}

		onYes: {saveFile(); resFunc()}
		onNo: resFunc()
	}

	FileDialog{
		property bool isOpen
		id: fileDialog
		title: qsTr("Open file")
		nameFilters: [qsTr("Graph builder file (*.grm *.graph)"),
					  qsTr("Graph model (*.grm)"),
					  qsTr("Graph (*.graph)")]

		function choose(isOp){
			isOpen = isOp
			folder = cBridge.getSaveFileFolder()
			if(isOpen)  selectExisting = true
			else		selectExisting = false
			open()
		}

		onAccepted: {
			cBridge.setSaveFile(fileUrl)
			if(isOpen)  openFile()
			else		saveFile()
		}
	}

	Loader{id: windowLoader}		// Window loader
	function loadWindow(src, prop){
		var ret = windowLoader.setSource(src, prop)
		windowLoader.item.visible = true
		return ret
	}

	appStack.focus: true
	appStack.initialItem: GraphViewItem{id: graphVI}
	appPanel.menuBtnVisible: !running
	appPanel.panelMenu: Menu{
		Menu{
			title: qsTr("File")
			MenuItem{
				enabled: !running
				text: qsTr("Open")
				shortcut: StandardKey.Open
				onTriggered: fileDialog.choose(true)
			}

			MenuItem{
				enabled: !running
				text: qsTr("Save")
				shortcut: StandardKey.Save
				onTriggered: saveFile()
			}

			MenuItem{
				enabled: !running
				text: qsTr("Save as")
				shortcut: StandardKey.SaveAs
				onTriggered: fileDialog.choose(false)
			}

			MenuItem{
				enabled: !running
				text: qsTr("Close file")
				shortcut: StandardKey.Close
				onTriggered: closeFile()
			}
		}

		MenuSeparator{}
		MenuItem{
			enabled: !running
			text: qsTr("Run algorithm")
			shortcut: "Ctrl+R"
			onTriggered: runAlgo()
		}

		Menu{
			id: algoMenu
			title: qsTr("Select algorithm")
			ExclusiveGroup{id: algoExclGroup}
		}

		MenuItem{
			text: qsTr("Plugins manager")
			onTriggered: loadWindow("qrc:/PluginManagerView.qml", {})
		}

		MenuSeparator{}
		MenuItem{
			enabled: !running
			text: qsTr("Preferences")
			shortcut: StandardKey.Preferences
			onTriggered: loadWindow("qrc:/SettingsView.qml", {})
		}

		MenuItem{
			text: qsTr("About")
			onTriggered: loadWindow("qrc:/AboutView.qml", {})
		}

		MenuSeparator{}
		MenuItem{
			enabled: !running
			text: qsTr("Exit")
			shortcut: StandardKey.Quit
			onTriggered: if(onExit()) Qt.quit()
		}
	}
}
