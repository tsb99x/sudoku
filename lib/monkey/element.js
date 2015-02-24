Element.prototype.clear = function() {
	while (this.firstChild)
		this.removeChild(this.firstChild);

	return this;
};

Element.prototype.addTag = function(tagNameString, namespaceString) {
	return this.appendChild(
		this.ownerDocument.createElementNS(namespaceString, tagNameString)
	);
};

Element.prototype.addListener = function(eventNameString, callbackFunction, bubbleBoolean) { // addListener ?
	this.addEventListener(eventNameString, callbackFunction, bubbleBoolean);

	return this;
};

Element.prototype.setAttributes = function(attributesObject) {
	var applyAttribute = function(attributeKey) {
		this.setAttribute(attributeKey, attributesObject[attributeKey]);
	};

	Object.getOwnPropertyNames(attributesObject)
		.forEach(applyAttribute, this);

	return this;
};

Element.prototype.setText = function(string) {
	this.textContent = string;

	return this;
};
