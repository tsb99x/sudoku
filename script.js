var App = (function() {

	/* MODE */

	'use strict';

	/* CONSTANTS */

	var SVG_NS = 'http://www.w3.org/2000/svg';

	/* SUPPORT FUNCTIONS */

	var randomInt = function(minNumber, maxNumber) {
		return Math.floor(Math.random() * (maxNumber - minNumber + 1)) + minNumber;
	};

	var timestampToTimeString = function(timestamp) {
		var passedSeconds = timestamp / 1000;
		var minutes = Math.floor(passedSeconds / 60);
		var seconds = Math.round(passedSeconds - minutes * 60);

		if (minutes.toString().length < 2)
			minutes = '0' + minutes;

		if (seconds.toString().length < 2)
			seconds = '0' + seconds;

		return minutes + ':' + seconds;
	};

	/* INTERFACE */

	return {
	
	init: function() {
		var svg = document.body.addTag('svg', SVG_NS)
			.setAttributes({
				'version': '1.1',
				'preserveAspectRatio': 'xMidYMid meet',
				'text-rendering': 'geometricPrecision'
			});

		App.startScreen(svg);
	},

	startScreen: function(svg) {
		var buttonWidth = 25,
			buttonHeight = 5;

		var toGameScreen = function() {
			App.gameScreen(svg);
		};

		svg.addTag('text', SVG_NS)
			.setAttributes({
				'x': 50,
				'y': 40
			}, {
				units: '%'
			})
			.setAttributes({
				class: 'info'
			})
			.setParameters({
				textContent: 'Добро пожаловать в Судоку!'
			});

		var g = svg.addTag('g', SVG_NS)
			.addListener('click', toGameScreen, false)
			.setAttributes({
				class: 'clickable'
			});

		var rect = g.addTag('rect', SVG_NS)
			.setAttributes({
				'x': 50 - buttonWidth / 2,
				'y': 50 - buttonHeight / 2,
				'width': buttonWidth,
				'height': buttonHeight
			}, {
				units: '%'
			});

		var buttonLabel = g.addTag('text', SVG_NS)
			.setAttributes({
				'x': 50,
				'y': 50 + 1 // FONT_SIZE of button
			}, {
				units: '%'
			})
			.setAttributes({
				class: 'label'
			})
			.setParameters({
				textContent: 'начать игру'
			});
	},

	gameScreen: function(svg) {
		svg.clear();

		var handler;

		var rootSolution = [
			[ 1, 2, 3,  4, 5, 6,  7, 8, 9 ],
			[ 4, 5, 6,  7, 8, 9,  1, 2, 3 ],
			[ 7, 8, 9,  1, 2, 3,  4, 5, 6 ],

			[ 2, 3, 4,  5, 6, 7,  8, 9, 1 ],
			[ 5, 6, 7,  8, 9, 1,  2, 3, 4 ],
			[ 8, 9, 1,  2, 3, 4,  5, 6, 7 ],

			[ 3, 4, 5,  6, 7, 8,  9, 1, 2 ],
			[ 6, 7, 8,  9, 1, 2,  3, 4, 5 ],
			[ 9, 1, 2,  3, 4, 5,  6, 7, 8 ]
		];

		var playGrid = rootSolution.map(function(row) {
			return row.map(function(cell) {
				return {
					value: '',
					solution: cell,
					locked: false
				};
			});
		});

		var checkSolution = function() {
			var isRowSolved = function(row) {
				var isCellSolved = function(cell) {
					return cell.value === cell.solution;
				};

				return row.every(isCellSolved);
			};

			if (playGrid.every(isRowSolved)) {
				clearInterval(handler);
				App.winScreen(svg, Date.now() - startTime);
			}
		};

		var i, posX, posY;
		for (i = 0; i < 80; i++) {
			posX = randomInt(0, 8);
			posY = randomInt(0, 8);

			playGrid[posY][posX].locked = true;
			playGrid[posY][posX].value = playGrid[posY][posX].solution;
		};

		var operation, subgrid, row1, row2, col1, col2;
		for (i = 0; i < 9; i++) {
			operation = randomInt(1, 2);

			if (operation === 1) { // row swap
				subgrid = randomInt(0, 2);
				row1 = randomInt(0, 2) + subgrid * 3;
				row2 = randomInt(0, 2) + subgrid * 3;

				var temp = playGrid[row1];
				playGrid[row1] = playGrid[row2];
				playGrid[row2] = temp;
			}
			if (operation === 2) { // col swap
				subgrid = randomInt(0, 2);
				col1 = randomInt(0, 2) + subgrid * 3;
				col2 = randomInt(0, 2) + subgrid * 3;

				for (var j = 0; j < 9; j++) {
					var temp = playGrid[j][col1];
					playGrid[j][col1] = playGrid[j][col2];
					playGrid[j][col2] = temp;
				}
			}
		};

		var CELL_SIZE = 8,
			SPACE_SIZE = 1,
			INTERSPACE_SIZE = 2,
			FONT_SIZE = 2.5,
			ORIGIN_POINT = { x: 8, y: 12 };

		var drawRow = function(row, rowIndex) {
			var drawCell = function(cell, cellIndex) {
				var offsetX = 0;
				var offsetY = 0;

				if (cellIndex > 2)
					offsetX++;
				if (cellIndex > 5)
					offsetX++;

				if (rowIndex > 2)
					offsetY++;
				if (rowIndex > 5)
					offsetY++;

				var cellStartX = ORIGIN_POINT.x + CELL_SIZE * cellIndex + SPACE_SIZE * cellIndex + offsetX * INTERSPACE_SIZE,
					cellStartY = ORIGIN_POINT.y + CELL_SIZE * rowIndex + SPACE_SIZE * rowIndex + offsetY * INTERSPACE_SIZE;

				var g = svg.addTag('g', SVG_NS)
					.setAttributes({
						'class': cell.locked ? 'locked' : 'clickable'
					});

				var rect = g.addTag('rect', SVG_NS)
					.setAttributes({
						'x': cellStartX,
						'y': cellStartY,
						'width': CELL_SIZE,
						'height': CELL_SIZE
					}, {
						units: '%'
					});

				var label = g.addTag('text', SVG_NS)
					.setAttributes({
						'x': cellStartX + CELL_SIZE / 2,
						'y': cellStartY + CELL_SIZE / 2 + FONT_SIZE / 2,
					}, {
						units: '%'
					})
					.setAttributes({
						'class': 'label'
					})
					.setParameters({
						textContent: cell.value,
					});

				var addValue = function() {
					if (cell.value == 9)
						cell.value = '';
					else
					if (cell.value < 9)
						cell.value++;
					else
					if (cell.value == '')
						cell.value = 1

					checkSolution();

					label.textContent = cell.value;
				};

				var subValue = function() {
					if (cell.value == 1)
						cell.value = '';
					else
					if (cell.value > 1)
						cell.value--;
					else
					if (cell.value == '')
						cell.value = 9;

					checkSolution();

					label.textContent = cell.value;
				};

				if (!cell.locked) {
					g.addListener('click', addValue, false);
					g.addListener('contextmenu', subValue, false);
				}
			};

			row.forEach(drawCell);
		};

		playGrid.forEach(drawRow);

		var startTime = Date.now();

		var time = svg.addTag('text', SVG_NS)
			.setAttributes({
				'x': 50,
				'y': 7 + 2.5 // FONT_SIZE of .timer
			}, {
				units: '%'
			})
			.setAttributes({
				'class': 'timer'
			});

		var updateTime = function() {
			time.textContent = timestampToTimeString(Date.now() - startTime);
		};
		updateTime();

		handler = setInterval(updateTime, 1000);
	},

	winScreen: function(svg, timestamp) {
		var toGameScreen = function() {
			App.gameScreen(svg);
		};

		svg.clear();

		var time = timestampToTimeString(timestamp);

		var buttonWidth = 30,
			buttonHeight = 5;

		var label = svg.addTag('text', SVG_NS)
			.setAttributes({
				'x': 50,
				'y': 40 + 1 // FONT_SIZE of button
			}, {
				units: '%'
			})
			.setAttributes({
				class: 'info'
			})
			.setParameters({
				textContent: 'Поздравляем! Вы решили судоку за ' + time
			});

		var g = svg.addTag('g', SVG_NS)
			.addListener('click', toGameScreen, false)
			.setAttributes({
				class: 'clickable'
			});

		var rect = g.addTag('rect', SVG_NS)
			.setAttributes({
				'x': 50 - buttonWidth / 2,
				'y': 50 - buttonHeight / 2,
				'width': buttonWidth,
				'height': buttonHeight
			}, {
				units: '%'
			});

		var buttonLabel = g.addTag('text', SVG_NS)
			.setAttributes({
				'x': 50,
				'y': 50  + 1 // FONT_SIZE of label
			}, {
				units: '%'
			})
			.setAttributes({
				class: 'label'
			})
			.setParameters({
				textContent: 'начать сначала'
			});
	}

	};

})();

Element.prototype.setAttributes = function(attributesObject, modifiersObject) {
	var namespace = null,
		units = null;

	if (modifiersObject != null) {
		namespace = modifiersObject.namespace || null;
		units = modifiersObject.units || null;
	}

	var applyAttribute = function(attributeKey) {
		this.setAttributeNS(
			namespace,
			attributeKey,
			attributesObject[attributeKey] + (units != null ? units : '')
		);
	};

	Object.keys(attributesObject)
		.forEach(applyAttribute, this);

	return this;
};

Element.prototype.setParameters = function(parametersObject) {
	var applyParameter = function(parameterKey) {
		this[parameterKey] = parametersObject[parameterKey];
	};

	Object.getOwnPropertyNames(parametersObject)
		.forEach(applyParameter, this);

	return this;
};

document.ontouchmove = Utility.Misc.preventDefault; // no scroll, bounce and zoom for iOS
document.oncontextmenu = Utility.Misc.preventDefault; // no context menu on RMB
document.onselectstart = Utility.Misc.preventDefault; // no text selection (for labels)
document.onmousedown = Utility.Misc.preventDefault; // no events with mouse down (working as text selection in firefox)

window.onload = App.init;
