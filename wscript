#! /usr/bin/env python
# encoding: utf-8

# the following two variables are used by the target "waf dist"
VERSION='0.0.1'
APPNAME='fortress'

# these variables are mandatory ('/' are converted automatically)
top = '.'
out = 'build'

def options(opt):
	opt.load('compiler_cxx')

def configure(conf):
	conf.load('compiler_cxx')
	conf.env.DEFINES_PDCURSES   = ['PDCURSES']
	conf.env.LIB_PDCURSES       = ['pdcurses']
	conf.env.LIBPATH_PDCURSES   = ['D:\Code\packages\c++\pdcurses-3.4']
	conf.env.INCLUDES_PDCURSES  = ['D:\Code\packages\c++\pdcurses-3.4']
	


def build(bld):
: bld.program(source='./src/c++/main.cpp',
		target='fortress',
		use='PDCURSES')

