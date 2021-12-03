/*
 * main.cpp
 *
 *  Created on: 24 Sep 2021
 *      Author: masters
 */

#include <egt/ui>
#include <iostream>
#include <sys/inotify.h>

using namespace egt;

class FileEventMonitor
{
public:
	using Callback = std::function<void(inotify_event& event)>;

	FileEventMonitor() = delete;

	FileEventMonitor(std::string filePath, Callback callback)
		: input(Application::instance().event().io()),
		  callback(std::move(callback)),
		  buffer(4)
	{
		// initialise inotify
		fd = inotify_init();
		wd = inotify_add_watch(fd, filePath.c_str(), IN_ALL_EVENTS);

		// assign the inotify descriptor to the stream
		input.assign(fd);

		// start an asynchronous read
		asio::async_read(input, asio::buffer(buffer),
				std::bind(&FileEventMonitor::handle_file_event, this,
				std::placeholders::_1));
	}

	~FileEventMonitor()
	{
		inotify_rm_watch(fd, wd);
		close(fd);
	}

	void handle_file_event(const asio::error_code& error)
	{
		// execute the callback
		if (callback != nullptr)
			callback(buffer.front());

		// register additional async read
		asio::async_read(input, asio::buffer(buffer),
				std::bind(&FileEventMonitor::handle_file_event, this,
				std::placeholders::_1));
	}

private:
	// wrapper around the fd
	asio::posix::stream_descriptor input;
	// registered callback
	Callback callback;
	// buffer to hold the notify event data
	std::vector<inotify_event> buffer;
	// inotify descriptors
	int fd, wd;
};

int main(int argc, char** argv)
{
	Application app(argc, argv);

	TopWindow window;
	Label label(window, "Event Handling");
	label.align(AlignFlag::top | AlignFlag::center_horizontal);

	// Simple event handling, responding to widget clicks
	Button button(window, "Press Me");
	button.align(AlignFlag::center);
	button.on_click([&label] (egt::Event& event) {
		label.text("Button pressed");
	});

	// Create a key handler for the TopWindow
	window.on_event([&label] (Event & event) {
		if (event.id() == EventId::keyboard_up)
			label.text("Keyboard UP");
		else if (event.id() == EventId::keyboard_down)
			label.text("Keyboard DOWN");

	}, {EventId::keyboard_down, EventId::keyboard_up});

	// Create an Event Monitor on an external directory
	FileEventMonitor eventMonitor("./", [&label] (inotify_event& event) {
		std::string strName = "No file";
		if (event.len > 0) {
			strName = event.name;
		}
		label.text("File event: " + strName);
	});

	window.show();
	return app.run();
}





