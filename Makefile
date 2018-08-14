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
PLUGIN_TEST_SUPPORT_DIR =? ./build-tools
KALEIDOSCOPE_BUILDER_DIR =? ./libraries/Kaleidoscope/bin/

endif

update-submodules: checkout-submodules
	@echo "All Kaleidoscope libraries have been updated from GitHub"

build-all: travis-test-all

travis-test-all: travis-install-arduino
	TRAVIS_ARDUINO_PATH=$(TRAVIS_ARDUINO_PATH) perl build-tools/quality/test-recursively travis-test
	TRAVIS_ARDUINO_PATH=$(TRAVIS_ARDUINO_PATH) perl build-tools/quality/test-recursively cpplint

checkout-submodules: git-pull
	git submodule update --init --recursive



maintainer-update-submodules:
	git submodule update --recursive --remote --init

git-pull:
	git pull

blindly-commit-updates: git-pull maintainer-update-submodules
	git commit -a -m 'Blindly pull all plugins up to current'
	git push


doxygen-generate:
	doxygen build-tools/automation/etc/doxygen/doxygen.conf


-include build-tools/makefiles/rules.mk
