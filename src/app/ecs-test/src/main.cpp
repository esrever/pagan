#include <iostream>
#include <core/serialize/serialize.h>
#include <ecs/ecs.h>

#include <core/app/sdlapp.h>
#include <core/texture/texturelib.h>

#include "RegisterComponents.h"

extern pgn::cMain g_app;

int main(int argc, char ** argv)
{
	pgn::mainapp() = new pgn::cSDLApp(argc,argv);
	pgn::mainecs() = new pgn::ecs::cECS();
	pgn::mainapp()->Init();
	//pgn::mainapp()->Resources<pgn::cTextureLib>("")->Load("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\data\\tilemap.png","");
	pgn::mainapp()->Resources<pgn::cTextureLib>("")->Load("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\data\\tiledesc.xml", "");

	auto& ecs = *mainecs();
	pgn::rl::RegisterComponents(ecs);
	pugi::xml_document doc_in2;
	auto res = doc_in2.load_file("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\lib\\ecs\\data\\example_in.xml");
	if (!res)
		std::cout << res.description() << std::endl;
	else
	{
		doc_in2.print(std::cout);
		pgn::SerializeIn(doc_in2, *mainecs());

		pugi::xml_document doc_out;
		pgn::SerializeOut(doc_out, "ECS", *mainecs());
		doc_out.save_file("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\lib\\ecs\\data\\example_out.xml");
	}

	pgn::mainapp()->Destroy();
	delete pgn::mainapp();
	delete pgn::mainecs();
}