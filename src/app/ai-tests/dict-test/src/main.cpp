#include <pystring.h>

#include <core/container/dict.h>

template<class T>
void pod_printer(const std::string& key, const T& v)
{
	std::cout << key << " a POD of value " << v << std::endl;
}

template<class T>
void cont1_printer(const std::string& key, const T& v)
{
	std::cout << key << " a cont of value: ";
	for (const auto& vv : v)
		std::cout << vv << ", ";
	std::cout << std::endl;
}

int main(int argc, char ** argv)
{
	std::vector<int> intvec;
	intvec.push_back(4);
	intvec.push_back(8);

	pgn::cDict dict;
	dict.Insert("an_int", int(5));
	dict.Insert("another_int", int(8));
	dict.Insert("a_float", 3.2f);
	dict.Insert("a_double", 4.5);
	dict.Insert("a_vector", intvec);

	dict.Visit<int>(&pod_printer<int>);
	dict.Visit<double>(&pod_printer<double>);
	dict.Visit<decltype(intvec)>(&cont1_printer<decltype(intvec)>);

	std::cout << "Getting " << "an_int" << " : " << *dict.Get<int>("an_int") << std::endl;


	std::cout << "------------------------------------------------------\n";

	auto * vp = dict.Get<int>("an_int");
	*vp = 123;
	
	dict.Visit<int>(&pod_printer<int>);
	dict.Visit<double>(&pod_printer<double>);
	dict.Visit<decltype(intvec)>(&cont1_printer<decltype(intvec)>);
	
	std::cout << "Getting " << "an_int" << " : " << (vp ? *vp : -1) << std::endl;

	std::cout << "------------------------------------------------------\n";

	dict.Erase<int>("an_int");

	dict.Visit<int>(&pod_printer<int>);
	dict.Visit<double>(&pod_printer<double>);
	dict.Visit<decltype(intvec)>(&cont1_printer<decltype(intvec)>);

	auto * vp2 = dict.Get<int>("an_int");
	std::cout << "Getting " << "an_int" << " : " << (vp2 ? *vp2 : -1) << std::endl;

	std::cout << "------------------------------------------------------\n";

}