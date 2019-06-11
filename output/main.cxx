#include <iostream>
#include <cstdlib>

#include <World.hxx>

int main(int argc, char *argv[])
{
	try
	{
		std::string fileName("config.xml");

		Generated::World world( new Generated::WorldConfig(fileName), world.useOpenMPSingleNode());

		world.initialize(argc,argv);
		world.run();
	}
	catch( std::exception & exceptionThrown )
	{
		std::cout << "exception thrown: " << exceptionThrown.what() << std::endl;
	}
	return 0;
}
