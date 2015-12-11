#!/usr/bin/env python3

from distutils.core import setup, Extension
from os import environ as env

version = '0.1'

extra_compile_args=[
  '-Wall',
  '-Wextra',
  '-pedantic',
  '-std=c++11',
]

setup(name='python-jsoncpp-demo',
      version=version,
      description='demo for jsoncpp converter for python',
      long_description='demo for jsoncpp converter for python.',
      author='Matthieu Bec',
      author_email='mdcb808@gmail.com',
      url='https://github.com/mdcb/python-jsoncpp',
      license='GPL',
      ext_modules=[
         Extension(
            name='jsoncpp_demo',
            sources = [
              'demo.cpp',
            ],
            include_dirs=[
              '/usr/include/boost',
              '/usr/include/jsoncpp', # pkg-config --cflags jsoncpp
            ],
            undef_macros=[],
            define_macros=[],
            libraries = [
              'boost_python3',
              'jsoncpp', # pkg-config --libs jsoncpp
            ],
            extra_compile_args = extra_compile_args,
            ),
         ],
      )

