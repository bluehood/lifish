#pragma once

namespace lif {

namespace ui {

struct ScreenStyle {
	std::string name;

	int spacing = 0;
	bool interactable = false;
	std::string font;
	unsigned int characterSize = 0;
	std::string vAlign, hAlign;
};

}

}
