#
# Set location of various cgtools
#
# These variables can be set here or on the command line.  Paths must not
# have spaces.
#
# The various *_ARMCOMPILER variables, in addition to pointing to
# their respective locations, also serve as "switches" for disabling a build
# using those cgtools. To disable a build using a specific cgtool, either set
# the cgtool's variable to empty or delete/comment-out its definition:
#     TICLANG_ARMCOMPILER ?=
# or
#     #TICLANG_ARMCOMPILER ?= ...
#
# If a cgtool's *_ARMCOMPILER variable is set (non-empty), various sub-makes
# in the installation will attempt to build with that cgtool.  This means
# that if multiple *_ARMCOMPILER cgtool variables are set, the sub-makes
# will build using each non-empty *_ARMCOMPILER cgtool.
#

SYSCONFIG_TOOL         ?= C:/ti/ccs1281/ccs/utils/sysconfig_1.22.0/sysconfig_cli.bat

CMAKE                  ?= C:/cmake-3.21.3/bin/cmake
PYTHON                 ?= python

TICLANG_ARMCOMPILER    ?= C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS-0
GCC_ARMCOMPILER        ?= C:/arm-none-eabi-gcc/12.3.Rel1-0-win32
IAR_ARMCOMPILER        ?= C:/iar/ewarm-9.60.3/arm

ifeq ("$(SHELL)","sh.exe")
# for Windows/DOS shell
    RM      = del
    RMDIR   = -rmdir /S /Q
    DEVNULL = NUL
    ECHOBLANKLINE = @cmd /c echo.
else
# for Linux-like shells
    RM      = rm -f
    RMDIR   = rm -rf
    DEVNULL = /dev/null
    ECHOBLANKLINE = echo
endif
