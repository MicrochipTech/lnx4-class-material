/*
 * main.cpp
 *
 *  Created on: 25 Feb 2021
 *      Author: masters
 */

#include <egt/ui>

int main(int argc, char** argv)
{
		egt::Application app(argc, argv);
		egt::TopWindow window;

		egt::Label label(window, "I'm a label");
		egt::center(label);

		egt::Label label2(window, "Another label",
				egt::Rect(10, 10, 100, 20));
		label2.font(egt::Font("Sans", 40));
		label2.color(egt::Palette::ColorId::label_text, egt::Palette::red);

		egt::Label label3(window, "Labeltastic",
				egt::Rect(0, 400, 800, 20), egt::AlignFlag::right);

		egt::Button button(window, "Press Me");
		button.align(egt::AlignFlag::top | egt::AlignFlag::center);
		button.font(egt::Font("Sans", 20));
		button.color(egt::Palette::ColorId::button_text, egt::Palette::green);

		button.on_click([&label] (egt::Event& event) {
			label.text("button pressed");
		});

		window.show();
		return app.run();
}



