#! /bin/sh
# Copyright (C) 2004-2017 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Make sure that Makefile.in is not overwritten on error.  Otherwise
# although the rebuilding rules would fail on error, they would
# succeed if make is run a second time immediately afterwards.
# Report from Harlan Stenn.

. test-init.sh

echo AC_OUTPUT>>configure.ac

: > Makefile.am

$ACLOCAL
# Create Makefile.in before configure.  configure ensures files
# generated by it or later are newer than configure, so this allows
# us to avoid a $sleep before updating Makefile.am below.
$AUTOMAKE
$AUTOCONF

./configure
$MAKE

cat >Makefile.am <<END
AUTOMAKE_OPTIONS = -Werror
INCLUDES = -Ifoo
foo_SOURCES = unused
END

# This repetition is deliberate; see heading comments.
$MAKE && exit 1
$MAKE && exit 1

: