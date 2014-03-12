#include <iostream>
#include <fstream>
#include <core/serialize/serialize.h>
#include <core/serialize/util.h>
#include <ecs/ecs.h>

#include <rlut/app/rlapp.h>

extern pgn::cMain g_app;

int main(int argc, char ** argv)
{
	pgn::mainapp() = new pgn::rlut::cRlApp(argc,argv);
	pgn::mainecs() = new pgn::cECS();
	pgn::mainapp()->Init();

	pugi::xml_document doc_in2;
	if(pgn::LoadXML(doc_in2, "C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\data\\ecs.xml", std::cout))
	{
		doc_in2.print(std::cout);
		SerializeIn(doc_in2, pgn::ECS());

		pugi::xml_document doc_out;
		SerializeOut(doc_out, "ECS", pgn::ECS());
		doc_out.save_file("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\data\\ecs_out.xml");
	}

	pgn::mainapp()->Destroy();
	delete pgn::mainapp();
	delete pgn::mainecs();
}