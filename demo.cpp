#include <iostream>
#include <boost/python.hpp>
#include <json/json.h>

using namespace boost::python;

class demo
{

public:

  Json::Value ret;

  demo()
  {
    ret["int"] = 1;
    ret["float"] = 1.1;
    ret["string"] = "one";
    ret["list"][0] = 2;
    ret["list"][1] = 2.2;
    ret["list"][2] = "two";
    ret["dict"]["a"] = 3;
    ret["dict"]["b"] = 3.3;
    ret["dict"]["c"] = "three";
  }

  void set(const Json::Value & node)
  {
    std::cerr << node << std::endl;
    ret = node;
  }

  Json::Value get(void)
  {
    return ret;
  }

};


//-----------------------------------------------------
// module definition
//-----------------------------------------------------


BOOST_PYTHON_MODULE(jsoncpp_demo)
{
  PyEval_InitThreads();

  std::cerr << "jsoncpp demo initialized\n";

  class_<demo, boost::noncopyable>("demo", "demo doc string", init<>())
  .def("set", &demo::set)
  .def("get", &demo::get)
  ;

}

