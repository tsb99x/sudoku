/* ==================== */
/* NAMESPACE DEFINITION */
/* ==================== */

var Utility = Utility || { };

/* ================= */
/* MODULE DEFINITION */
/* ================= */

Utility.Misc = (function() {

	/* ============== */
	/* INTERPRET MODE */
	/* ============== */

	'use strict';

	/* ================ */
	/* MODULE INTERFACE */
	/* ================ */

	return {

	preventDefault: function(event) {
		event.preventDefault();

		return false;
	},

	toArray: function(arrayLike) {
		return Array.prototype.slice.call(arrayLike);
	}

	};

})();
