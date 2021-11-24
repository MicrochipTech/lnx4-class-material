/*
 * main.cpp
 *
 *  Created on: 24 Sep 2021
 *      Author: masters
 */

#include <egt/ui>

using namespace egt;

int main(int argc, char** argv)
{
	Application app(argc, argv);

	TopWindow window;
	Label label(window, "Animation");
	label.align(AlignFlag::top | AlignFlag::center_horizontal);

	// create a frame in the top half of the screen
	Window btnWindow(window, Rect(0, label.height(), window.width(), window.height() / 2));
	btnWindow.color(Palette::ColorId::bg, Palette::black);
	btnWindow.show();

	// create two clickable buttons inside the window frame
	auto b1 = std::make_shared<Button>(btnWindow, "Option 1", Rect(0, 0, 110, 66));
	auto b2 = std::make_shared<Button>(btnWindow, "Option 2", Rect(0, 0, 110, 66));

	// calculate the left, centre and right of the window adjusting for button sizes
	auto btnLeftPos = Point(0, (btnWindow.height()  - b1->height()) / 2);
	auto btnCenterPos = Point((btnWindow.width() - b1->width()) / 2, btnLeftPos.y());
	auto btnRightPos = Point(btnWindow.width() - b2->width(), btnLeftPos.y());

	// position the buttons at initial positions
	b1->move(btnCenterPos);
	b2->move(btnRightPos);

	// create property animators to move the buttons to the left
	PropertyAnimator animationLeft1(btnCenterPos.x(), 0,
			std::chrono::milliseconds(500), easing_linear);
	animationLeft1.on_change([b1] (PropertyAnimator::Value value) {
		b1->x(value);
	});

	PropertyAnimator animationLeft2(btnRightPos.x(), btnCenterPos.x(),
									std::chrono::milliseconds(500), easing_spring);
	animationLeft2.on_change([b2] (PropertyAnimator::Value value) {
		b2->x(value);
	});
	// create a single animator to move them to the right, this uses a scale
	// from 0.0 to 1.0 to allow interpolation for both buttons
	PropertyAnimatorF animationRight(0, 1, std::chrono::milliseconds(500), easing_bounce);
	animationRight.on_change([b1, b2, btnCenterPos] (PropertyAnimatorF::Value value) {
		auto pixelPos = value * btnCenterPos.x();
		b1->x(pixelPos);
		b2->x(pixelPos + btnCenterPos.x());
	});

	// create a left and right button to move the animated buttons
	auto leftButton = std::make_shared<ImageButton>(btnWindow, Image("icon:rewind.png"));
	leftButton->move(Point(0, btnWindow.height() - leftButton->height()));
	auto rightButton = std::make_shared<ImageButton>(btnWindow, Image("icon:fastforward.png"));
	rightButton->move(Point(btnWindow.width() - rightButton->width(),
							btnWindow.height() - rightButton->height()));
	rightButton->disable();

	// two click handlers for the buttons
	leftButton->on_click([&animationLeft1, &animationLeft2, &leftButton, &rightButton] (Event& event) {
		animationLeft1.start();
		animationLeft2.start();
		leftButton->disable_toggle();
		rightButton->disable_toggle();
	});

	rightButton->on_click([&animationRight, &leftButton, &rightButton] (Event& event) {
		animationRight.start();
		leftButton->disable_toggle();
		rightButton->disable_toggle();
	});

	// create an animation sequence that shows a balloon at the bottom of the screen
	Window balloonBox;
	auto balloonImage = std::make_shared<ImageLabel>(Image("file:ball.png"));
	balloonBox.resize(balloonImage->size());
	balloonBox.add(balloonImage);
	balloonBox.move(Point((window.width() - balloonBox.width()) / 2,
							window.height() - balloonBox.height()));
	balloonBox.show();
	window.add(balloonBox);

	// create an animator to move the balloon from the bottom of the screen to the center
	PropertyAnimator balloonUp(window.height() - balloonBox.height(),
								btnWindow.box().bottom(),
								std::chrono::milliseconds(2000),
								easing_bounce);
	balloonUp.on_change([&balloonBox] (PropertyAnimator::Value value) {
		balloonBox.y(value);
	});

	AnimationDelay balloonDelay(std::chrono::milliseconds(2000));

	// and an animator to move it down
	PropertyAnimator balloonDown(btnWindow.box().bottom(),
								window.height() - balloonBox.height(),
								std::chrono::milliseconds(2000),
								easing_bounce);
	balloonDown.on_change([&balloonBox] (PropertyAnimator::Value value) {
		balloonBox.y(value);
	});

	AnimationDelay balloonDelay2(std::chrono::milliseconds(2000));

	// build the animation sequence
	AnimationSequence seq(true); // loop
	seq.add(balloonUp);
	seq.add(balloonDelay);
	seq.add(balloonDown);
	seq.add(balloonDelay2);

	seq.start();

	window.show();
	return app.run();
}





