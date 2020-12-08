# Copyright 2018 Keyboard.io, inc. <jesse@keyboard.io>
#
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
 


ifeq ($(BOARD_HARDWARE_PATH),)
PLUGIN_TEST_SUPPORT_DIR ?= ./build-tools
KALEIDOSCOPE_BUILDER_DIR ?= ./avr/libraries/Kaleidoscope/bin/

endif

prepare-virtual:
	perl -p	-e 's/^(.*?).build.core=arduino:arduino/\1.hide=true\n\1.build.core=keyboardio:arduino/' < avr/boards.txt >virtual/boards.txt
	[ -h virtual/platform.txt ] || ln -s platform-real.txt virtual/platform.txt

update-submodules: checkout-submodules
	@echo "Kaleidoscope has been updated from GitHub"

checkout-submodules: git-pull
	git submodule update --init --recursive



maintainer-update-submodules:
	git submodule update --recursive --remote --init
	git submodule foreach git checkout master
	git submodule foreach git pull origin master

git-pull:
	git pull

blindly-commit-updates: git-pull maintainer-update-submodules
	git commit -a -m 'Blindly pull all submodules up to current'
	git push


-include build-tools/makefiles/rules.mk
