
#ifndef __World_hxx__
#define __World_hxx__

#include <World.hxx>

namespace Generated 
{

class WorldConfig;

class World : public Engine::World
{
	void createRasters();
	void createAgents();

	void stepEnvironment();
public:
	Earth( EarthConfig * config, Engine::Scheduler * scheduler = 0);
	virtual ~Earth();
};

}

#endif
