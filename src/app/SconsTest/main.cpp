#include <iostream>
#include <pystring.h>
#include <boost/format.hpp>
#include <GL/glew.h>

using namespace std;

int main()
{
    glewInit();
	string tmp;
	tmp = pystring::capitalize("pystring");
    tmp = boost::str(boost::format( "Hello, %s") %tmp);
	cout<<tmp<<endl;
	return 0;
}