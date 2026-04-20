#include "Application.h"

int main(int argc, char* argv[])
{
	sf::ContextSettings settings;
	settings.antiAliasingLevel = 8;
	Application app(settings);
	app.start(argc, argv);

	return 0;
}