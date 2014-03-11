#include <rlut/ai/btree.h>
#include <rlut/app/rlapp.h>

int main(int argc, char ** argv)
{
	pgn::cSDLApp*& app = pgn::mainapp();
	app = new pgn::rlut::cRlApp(argc, argv);
	pgn::mainecs() = new pgn::cECS();
	app->Init();

	pgn::bt::cBehaviorTree btree;
	std::string ret;
	pugi::xml_document doc, doc_out;
	auto result = doc.load_file("C:\\Users\\Babis\\Documents\\GitHub\\pagan\\src\\data\\behaviors.xml", pugi::parse_default | pugi::parse_comments);
	if (result)
	{
		pgn::bt::cBehavior * ptr;
		pgn::bt::cBehavior::SerializeIn(*doc.begin(), ptr);
		btree.SetRoot(ptr);

		btree.Tick();

		ptr->SerializeOut(doc_out.append_child(ptr->Type()));
		doc_out.print(std::cout);
	}
	else
	{
		std::cout <<result.offset<<": "<< result.description() <<std::endl;
	}

	app->Destroy();
	delete app;
}