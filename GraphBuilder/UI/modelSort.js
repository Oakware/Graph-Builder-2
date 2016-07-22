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

function searchItem(model, cmp){
	for(var i=0; i<model.count; ++i){
		var itm = model.get(i)
		if(cmp(itm)) return i
	}
	return -1
}

function searchInsertPos(model, val, cmp){
	for(var i=0; i<model.count; ++i){
		var itm = model.get(i)
		if(cmp(itm, val)) break // a >= b
	}
	return i
}

function mSort(model, cmp){
	for(var i=0; i<model.count; ++i){
		var itm1 = model.get(i)
		for(var j=0; j<i; ++j){
			var itm2 = model.get(j)
			if(cmp(itm2, itm1)) break
		}
		model.move(i, j, 1)
	}
}
