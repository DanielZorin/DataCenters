#pragma once

#include "defs.h"
#include <string>

class Port {
public:
	Port(std::string name, Element* parentNode):
		name(name), parentNode(parentNode) {
	}

	inline void connect(Edge* link, Port* otherPort) {
		connectedPort =  otherPort;
		assosiatedLink = link;
	}

	inline Element* getParentNode() const {
		return parentNode;
	}

	inline std::string getName() const {
		return name;
	}

	inline Port* getConnectedPort() const {
		return connectedPort;
	}

	inline Edge* getConnectedLink() {
		return assosiatedLink;
	}

private:
	std::string name;
	Port* connectedPort;
	Edge* assosiatedLink;
	Element* parentNode;
};
