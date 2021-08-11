/*
 * main.cpp
 *
 *  Created on: 3 Mar 2021
 *      Author: masters
 */

#include <egt/ui>

using namespace egt;

int main(int argc, char** argv)
{
	Application app(argc, argv);

	TopWindow window;
	Label label(window, "Layout Management");
	center(label);

	// create a StaticGrid to hold the icons
	auto grid = std::make_shared<StaticGrid>(Rect(200, 280, 400, 200), StaticGrid::GridSize(3, 2));

	// add the icons expanding them to align in the center of each cell
	grid->add(expand(std::make_shared<ImageLabel>(Image("icon:lock.png"))));
	grid->add(expand(std::make_shared<ImageLabel>(Image("icon:home.png"))));
	grid->add(expand(std::make_shared<ImageLabel>(Image("icon:camera.png"))));
	grid->add(expand(std::make_shared<ImageLabel>(Image("icon:delete.png"))));
	grid->add(expand(std::make_shared<ImageLabel>(Image("icon:calendar.png"))));
	grid->add(expand(std::make_shared<ImageLabel>(Image("icon:settings.png"))));
	window.add(grid);

	// create a horizontal BoxSizer to hold 4 buttons
	BoxSizer boxSizer(window, Orientation::horizontal);
	expand_horizontal(boxSizer);
	for(auto i = 0; i < 4; i++)
	{
		auto button = std::make_shared<Button>("Button " + std::to_string(i));
		button->margin(10);
		boxSizer.add(button);
	}

	window.show();
	return app.run();
}




