/*
 * main.cpp
 *
 *  Created on: 27 Feb 2021
 *      Author: masters
 */

#include <egt/ui>

using namespace egt;

// Create a grid for some the controls
auto createGrid(TopWindow& parent, int numControls, AlignFlag alignment)
{
	auto grid = std::make_shared<StaticGrid>(StaticGrid::GridSize(1, numControls));
	grid->resize(Size(200,480));
	grid->margin(5);
	grid->horizontal_space(5);
	grid->vertical_space(5);
	grid->align(alignment);
	parent.add(grid);
	return grid;
}

void createEditBoxes(std::shared_ptr<StaticGrid> parent)
{
	// create a simple text box
	auto textBox1 = std::make_shared<TextBox>("Some Text");
	// change the font
	textBox1->align(AlignFlag::center);
	textBox1->font(Font("Sans", 30));

	// finally add it to the parent grid
	parent->add(expand(textBox1));

	// create a second text box
	auto textBox2 = std::make_shared<TextBox>("Validate");
	// but use the on change event to update textBox1
	textBox2->on_text_changed([textBox1, textBox2] () {
		textBox1->text(textBox2->text());
	});

	// use a lambda function to validate the entry
	textBox2->add_validator_function([] (const std::string& input) {
		// only allow alphabetic characters
		if (std::isalpha(input[0]) || std::isspace(input[0]))
			return true;
		else
			return false;
	});
	textBox2->input_validation_enabled(true);

	parent->add(expand(textBox2));
}

std::unique_ptr<ButtonGroup> createCheckBoxes(std::shared_ptr<StaticGrid> parent,
		std::shared_ptr<ButtonGroup> & radioButtonGroup)
{
	// create a small grid to hold the checkboxes
	auto checkboxGrid = std::make_shared<StaticGrid>(StaticGrid::GridSize(1,3));
	parent->add(expand(checkboxGrid));

	// create a simple checkbox
	auto checkbox1 = std::make_shared<CheckBox>("Checkbox 1");
	// add it to the grid
	checkboxGrid->add(expand(checkbox1));

	// and some more
	auto checkbox2 = std::make_shared<CheckBox>("Checkbox 2");
	checkboxGrid->add(expand(checkbox2));
	auto checkbox3 = std::make_shared<CheckBox>("Checkbox 3");
	checkboxGrid->add(expand(checkbox3));

	// add the check boxes to a button group that ensures at least one option is selected
	auto cbGroup = std::make_unique<ButtonGroup>(true, false);
	cbGroup->add(checkbox1);
	cbGroup->add(checkbox2);
	cbGroup->add(checkbox3);

	// create some radio buttons
	auto radioGrid = std::make_shared<StaticGrid>(StaticGrid::GridSize(1,3));
	parent->add(expand(radioGrid));

    auto radiobox1 = std::make_shared<egt::RadioBox>("Option 1");
    radioGrid->add(egt::expand(radiobox1));
    auto radiobox2 = std::make_shared<egt::RadioBox>("Option 2");
    radioGrid->add(egt::expand(radiobox2));
    radiobox2->checked(true);
    auto radiobox3 = std::make_shared<egt::RadioBox>("Option 3");
    radioGrid->add(egt::expand(radiobox3));

    std::shared_ptr<ButtonGroup> radioGroup = std::make_shared<ButtonGroup>(true);
    radioGroup->add(radiobox1);
    radioGroup->add(radiobox2);
    radioGroup->add(radiobox3);

    // assign the radio group to the passed in parameter so that we can reference them in main
    radioButtonGroup = radioGroup;

    // create a toggle box
    auto toggle1 = std::make_shared<ToggleBox>();
    toggle1->toggle_text("Off",  "On");
    // add it to the parent grid
    parent->add(expand(toggle1));

	// return the button group
	return cbGroup;
}

void createListBoxes(std::shared_ptr<StaticGrid> parent)
{
	// create a label to print the name of the selected item
	auto label = std::make_shared<Label>("Selected: ", AlignFlag::left);
	parent->add(expand(label));

	// create a simple listbox
	auto list0 = std::make_shared<ListBox>();
	list0->add_item(std::make_shared<StringItem>("Item 1"));
	list0->add_item(std::make_shared<StringItem>("Item 2"));
	list0->add_item(std::make_shared<StringItem>("Item 3"));
	parent->add(expand(list0));

	// using a lambda function to handle change of selection
	list0->on_selected([label, list0] (size_t selectedItem) {
		auto item = list0->item_at(selectedItem);
		label->text("Selected: " + item->text());
	});


	// create a listbox from an array
	std::vector<std::shared_ptr<StringItem>> items = {
			std::make_shared<StringItem>("One"),
			std::make_shared<StringItem>("Two"),
			std::make_shared<StringItem>("Three")
	};
	auto list1 = std::make_shared<ListBox>(items);
	parent->add(expand(list1));


	// create a listbox with images
	auto list2 = std::make_shared<ListBox>();
	list2->add_item(std::make_shared<StringItem>("Help", Image("icon:help.png")));
	list2->add_item(std::make_shared<StringItem>("Info", Image("icon:info.png")));
	list2->add_item(std::make_shared<StringItem>("Warning", Image("icon:warning.png")));
	list2->selected(1);
	parent->add(expand(list2));
}

std::shared_ptr<egt::Slider> createSliders(std::shared_ptr<StaticGrid> parent)
{
	// create a boxsizer to hold the sliders
	auto sizer = std::make_shared<BoxSizer>(Orientation::flex);
	parent->add(expand(sizer));

	// the first slider is vertical and holds integers from 0-100
    auto slider0 = std::make_shared<egt::Slider>(egt::Rect(0, 0, 40, 80), 0, 100, 0, Orientation::vertical);
    slider0->value(50);
    sizer->add(slider0);

    // the second slider is horizontal and holds floating point values
    auto slider1 = std::make_shared<egt::SliderF>(egt::Rect(0, 0, 200, 80), 100, 0);
    slider1->value(75);
    // display a round handle and the current value in the label
    slider1->slider_flags().set({egt::SliderF::SliderFlag::round_handle,
                                 egt::SliderF::SliderFlag::show_label});
    sizer->add(expand(slider1));

    // We will use the value from slider0 elsewhere
    return slider0;
}

void createMeters(std::shared_ptr<StaticGrid> parent, std::shared_ptr<Slider> slider)
{
	// create a level meter
    auto levelMeter = std::make_shared<egt::LevelMeter>();
    levelMeter->num_bars(10);
    parent->add(egt::expand(levelMeter));

    // create an analog meter
    auto analogMeter = std::make_shared<egt::AnalogMeter>();
    analogMeter->font(Font("Sans", 12));
    parent->add(egt::expand(analogMeter));

    // connect the slider value to the control values and update them when it changes
    slider->live_update(true);
    slider->on_value_changed([slider, levelMeter, analogMeter] () {
    	int sliderValue = slider->value();
    	levelMeter->value(sliderValue);
    	analogMeter->value(sliderValue);
    });
}

int main(int argc, char** argv)
{
	Application app(argc, argv);

	TopWindow window;
	Label label(window, "Additional Widgets");
	center(label);
	window.show();

	std::shared_ptr<StaticGrid> leftGrid = createGrid(window, 5, AlignFlag::left);
	std::shared_ptr<StaticGrid> rightGrid = createGrid(window, 5, AlignFlag::right);
	std::shared_ptr<ButtonGroup> radioButtonGroup;

	// create some edit boxes
	createEditBoxes(leftGrid);

	// create check boxes, note the assignment to keep the smart pointer in scope
	std::unique_ptr<ButtonGroup> buttonGroup = createCheckBoxes(leftGrid, radioButtonGroup);

	// create the listboxes
	createListBoxes(rightGrid);

	// create sliders
	std::shared_ptr<Slider> slider = createSliders(rightGrid);

	// create a grid to hold the meters at the top of the screen
	auto topGrid = std::make_shared<StaticGrid>(StaticGrid::GridSize(2, 1));
	topGrid->resize(Size(400,160));
	topGrid->move(Point(200, 0));
	window.add(topGrid);

	// create the meters, pass in the slider object from earlier
	createMeters(topGrid, slider);

	return app.run();
}



