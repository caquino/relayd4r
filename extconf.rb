#!/usr/bin/env ruby
require 'mkmf'
dir_config("relayd4r")
have_library("relay")
create_makefile("relayd4r")
