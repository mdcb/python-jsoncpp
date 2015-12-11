#!/usr/bin/env python3
#
#  Copyright (C) 2015  Matthieu Bec, mdcb808@gmail.com
#
#  This file is part of python-jsoncpp.
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#


from distutils.core import setup, Extension
from os import environ as env

version = '0.1'

extra_compile_args=[
  '-Wall',
  '-Wextra',
  '-pedantic',
  '-std=c++11',
]

setup(name='python-jsoncpp',
      version=version,
      description='jsoncpp converter for python',
      long_description='jsoncpp converter for python.',
      author='Matthieu Bec',
      author_email='mdcb808@gmail.com',
      url='https://github.com/mdcb/python-jsoncpp',
      license='GPL',
      ext_modules=[
         Extension(
            name='jsoncpp',
            sources = [
              'extension.cpp',
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

