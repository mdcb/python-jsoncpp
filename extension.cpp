/*
 *    Copyright (C) 2012  Matthieu Bec, mdcb808@gmail.com
 *
 *    This file is part of python-jsoncpp.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <exception>
#include <boost/python.hpp>
#include <json/json.h>

using namespace boost::python;

//-----------------------------------------------------
// Json::Value to python converter
//-----------------------------------------------------

struct jsonvalue_topython_converter
{
  static PyObject * convert(Json::Value const & node)
  {
    switch (node.type())
      {

      case Json::ValueType::nullValue:
      {
        return incref(Py_None);
      }

      case Json::ValueType::intValue:
      {
        object result;
        //result = object(node.asInt());
        //result = object(node.asInt64());
        result = object(node.asLargestInt());
        return incref(result.ptr());
      }

      case Json::ValueType::uintValue:
      {
        object result;
        //result = object(node.asUInt());
        //result = object(node.asUInt64());
        result = object(node.asLargestUInt());
        return incref(result.ptr());
      }

      case Json::ValueType::realValue:
      {
        object result;
        //result = object(node.asFloat());
        result = object(node.asDouble());
        return incref(result.ptr());
      }

      case Json::ValueType::stringValue:
      {
        object result;
        result = object(node.asString());
        return incref(result.ptr());
      }

      case Json::ValueType::booleanValue:
      {
        object result;
        result = object(node.asBool());
        return incref(result.ptr());
      }

      case Json::ValueType::arrayValue:
      {
        list result;

        for (auto it = node.begin(); it != node.end(); ++it)
          {
            PyObject * obj = /*jsonnode_topython_converter::*/convert(*it);
            object pyobj = object(handle<>(obj));
            result.append(pyobj);
          }

        return incref(result.ptr());
      }

      case Json::ValueType::objectValue:
      {
        dict result;

        for (auto it = node.begin(); it != node.end(); ++it)
          {
            PyObject * kobj = /*jsonvalue_topython_converter::*/convert(it.key());
            object pykobj = object(handle<>(kobj));
            PyObject * vobj = /*jsonvalue_topython_converter::*/convert(*it);
            object pyvobj = object(handle<>(vobj));
            result[pykobj] = pyvobj;
          }

        return incref(result.ptr());
      }

      default:
      {
        throw std::runtime_error("undefined json value");
      }
      }
  }
};


struct jsonvalue_frompython_converter
{
  jsonvalue_frompython_converter()
  {
    converter::registry::push_back(
      &convertible,
      &construct,
      type_id<Json::Value>());
  }

  static void * convertible(PyObject * obj_ptr)
  {
    return obj_ptr; // TODO, assume True.
  }


  static void jp_convert(PyObject * obj_ptr, Json::Value & node)
  {
    if (PyBytes_Check(obj_ptr)) // NB: strings are not bytes
      {
        //debug("DEBUG converting bytes");
        node = std::string(PyBytes_AsString(obj_ptr));
      }

    else if (PyUnicode_Check(obj_ptr))
      {
        //debug("DEBUG converting unicode");
        node = std::string(PyBytes_AsString(PyUnicode_AsASCIIString(obj_ptr)));
      }

    else if (PySequence_Check(obj_ptr)) // test before mapping because list have mapping in python3
      {
        //debug("DEBUG converting sequence");
        node = Json::Value(Json::ValueType::arrayValue);

        for (Py_ssize_t i = 0; i < PySequence_Length(obj_ptr); i++)
          {
            PyObject * elt = PySequence_GetItem(obj_ptr, i);
            Json::Value elty;
            jp_convert(elt, elty);
            node.append(elty);
          }
      }

    else if (PyMapping_Check(obj_ptr))
      {
        //debug("DEBUG converting map");
        node = Json::Value(Json::ValueType::objectValue);

        PyObject * keyvals = PyMapping_Items(obj_ptr);

        for (Py_ssize_t i = 0; i < PyMapping_Length(keyvals); i++)
          {
            PyObject * kv = PyList_GetItem(keyvals, i);
            PyObject * k = PyTuple_GetItem(kv, 0);
            PyObject * v = PyTuple_GetItem(kv, 1);
            Json::Value * peltk = new Json::Value(); // refcount
            Json::Value eltv;
            jp_convert(k, *peltk);
            jp_convert(v, eltv);
            node[peltk->asString()] = eltv; // only support string keys
          }
      }

    else if (PyBool_Check(obj_ptr))
      {
        //debug("DEBUG converting bool");
        node = static_cast<bool>(PyLong_AsLong(obj_ptr));
      }

    else if (PyLong_Check(obj_ptr))
      {
        //debug("DEBUG converting long");
        node = (Json::LargestInt) PyLong_AsLong(obj_ptr); // json cast
      }

    else if (PyFloat_Check(obj_ptr))
      {
        //debug("DEBUG converting float");
        node = PyFloat_AsDouble(obj_ptr);
      }

    else
      {
        //debug("DEBUG conversion not supported");
        throw std::runtime_error("invalid type for conversion.");
      }
  }

  // Convert obj_ptr into a Json::Value
  static void construct(
    PyObject * obj_ptr,
    converter::rvalue_from_python_stage1_data * data)
  {
    Json::Value node;

    jp_convert(obj_ptr, node);

    // Grab pointer to memory into which to construct the new Json::Value
    void * storage = ((converter::rvalue_from_python_storage<Json::Value> *) data)->storage.bytes;

    // in-place construct the new Json::Value using the character data
    // extracted from the python object
    new(storage) Json::Value(node);

    // Stash the memory chunk pointer for later use by boost.python
    data->convertible = storage;
  }
};

//-----------------------------------------------------
// module definition
//-----------------------------------------------------


BOOST_PYTHON_MODULE(jsoncpp)
{
  to_python_converter<Json::Value, jsonvalue_topython_converter>();
  jsonvalue_frompython_converter();
}


