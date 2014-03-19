#include <iostream>
#include <fstream>
#include <core/serialize/serialize.h>
#include <core/serialize/util.h>
#include <ecs/ecs.h>

#include <rlut/app/rlapp.h>

extern pgn::cMain g_app;

int main(int argc, char ** argv)
{
	std::cout << PROJECT_ROOT << std::endl;
	pgn::mainapp() = new pgn::rlut::cRlApp(argc,argv);
	auto& ecs = pgn::mainecs() = new pgn::ecs::cECS();
	pgn::mainapp()->Init();

	const char * fname_in = PROJECT_ROOT "data\\ecs.xml";
	const char * fname_out = PROJECT_ROOT "data\\ecs_out.xml";

	pugi::xml_document doc_in2;
	if(pgn::LoadXML(doc_in2, fname_in, std::cout))
	{
		doc_in2.print(std::cout);
		pgn::SerializeIn(doc_in2, ecs);

		pugi::xml_document doc_out;
		pgn::SerializeOut(doc_out, "ECS", ecs);
		doc_out.save_file(fname_out);
	}

	pgn::mainapp()->Destroy();
	delete pgn::mainapp();
	delete pgn::mainecs();
}