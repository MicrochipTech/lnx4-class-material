/*
 * main.cpp
 *
 *  Created on: 24 Feb 2021
 *      Author: masters
 */

#include <egt/ui>

int main(int argc, char** argv)
{
		egt::Application app(argc, argv);

		egt::TopWindow window;
		egt::Button button(window, "Press Me");
		egt::center(button);
		window.show();

		return app.run();
}



