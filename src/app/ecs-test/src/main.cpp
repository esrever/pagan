#include <iostream>
#include <core/serialize/serialize.h>

#include <ecs/ecs.h>

#include "RegisterComponents.h"

int main(int argc, char ** argv)
{
	auto& ecs = pgn::ECS();
	pgn::RegisterComponents(ecs);
	pugi::xml_document doc_in2;
	auto res = doc_in2.load_file("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\lib\\ecs\\data\\example_in.xml");
	if (!res)
		std::cout << res.description() << std::endl;
	else
	{
		doc_in2.print(std::cout);
		SerializeIn(doc_in2, pgn::ECS());

		pugi::xml_document doc_out;
		SerializeOut(doc_out, "ECS", pgn::ECS());
		doc_out.save_file("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\lib\\ecs\\data\\example_out.xml");
	}
		
}