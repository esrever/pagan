#include <iostream>
#include <core/serialize/serialize.h>

#include <core/texture/texture.h>

int main(int argc, char ** argv)
{
	pugi::xml_document doc_out;
	pgn::SerializeOut(doc_out, "int", -5);
	pgn::SerializeOut(doc_out, "float", 5.6f);
	pgn::SerializeOut(doc_out, "double", 5.6);
	pgn::SerializeOut(doc_out, "uint", 4);
	pgn::SerializeOut(doc_out, "ivec2", glm::ivec2(3,4));
	pgn::SerializeOut(doc_out, "ivec2", glm::vec3(3.1, 4.2,5.3));

	std::set<std::string> sset = {"John", "Kelly", "Amanda", "Kim"};
	std::vector<float> fvec = { 2.2f, 3.4f, 5.4f };
	std::map<int, std::string> vmap = { { 0, "num0" }, { 5, "num5" }, { 2, "hey you" } };
	//std::map<std::string, int> vmap = { {  "num0",0 }, { "num5",5 }, { "hey_you", 2} };
	std::map< std::set<std::string>, std::vector<float>> weirdmap; weirdmap[sset] = fvec;
	pgn::SerializeOut(doc_out, "set", sset);
	pgn::SerializeOut(doc_out, "fvec", fvec);
	pgn::SerializeOut(doc_out, "vmap", vmap);
	pgn::SerializeOut(doc_out, "wmap", weirdmap);

	SDL_Rect rect;
	rect.w = 1;
	rect.h = 2;
	rect.x = 5;
	rect.y = -3;
	pgn::SerializeOut(doc_out, "rect", rect);

	std::list<std::string> slist = { "John", "Kelly", "Amanda", "Kim" };
	pgn::SerializeOut(doc_out, "list", slist);

	doc_out.save_file("testmich.xml");
	doc_out.print(std::cout);

	pugi::xml_document doc_in;
	doc_in.load_file("testmich.xml");
	doc_in.print(std::cout);

	pugi::xml_document doc_in2;
	auto res = doc_in2.load_file("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\lib\\ecs\\data\\example.xml");
	if (!res)
		std::cout << res.description() << std::endl;
	else
		doc_in2.print(std::cout);
}