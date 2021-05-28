/*
 * main.cpp
 *
 *  Created on: 28 May 2021
 *      Author: masters
 */

#include <iostream>
#include <egt/ui>

using namespace egt;

// Create a grid for some buttons at the bottom of the screen
auto createGrid(TopWindow& parent, int numButtons)
{
	auto grid = std::make_shared<StaticGrid>(StaticGrid::GridSize(numButtons, 1));
	grid->resize(Size(800,50));
	grid->margin(5);
	grid->horizontal_space(5);
	grid->vertical_space(5);
	grid->align(AlignFlag::bottom);
	parent.add(grid);
	return grid;
}

// create the button using a smart pointer, the lifetime is explicitly managed
// and resources will be deleted once nothing is using the object
void buttonBuilder(std::shared_ptr<StaticGrid> parent, std::string text)
{
	// create the button using a smart pointer
	auto button = std::make_shared<Button>(text);

	// add and expand it to fit one cell of the grid
	parent->add(expand(button));
}

int main(int argc, char** argv)
{
		Application app(argc, argv);

		TopWindow window;

		// create the grid to manage layout
		auto grid = createGrid(window, 3);

		// This object has been created on the stack frame
		Button button1(window, "Button 1");
		align(button1, AlignFlag::top | AlignFlag::center);


		// This is a button created with a smart pointer
		auto button2 = std::make_shared<Button>(window, "Button 2");
		center(button2);


		// Create 3 buttons in the grid
		buttonBuilder(grid, "Button 3");
		buttonBuilder(grid, "Button 4");
		buttonBuilder(grid, "Button 5");


		window.show();
		return app.run();
}



