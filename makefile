# ----------------------------
# Makefile Options
# ----------------------------

NAME = USBT
ICON = icon.png
DESCRIPTION = "CE C Toolchain Demo"
COMPRESSED = NO
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Werror=return-type -Oz
CXXFLAGS = -Wall -Wextra -Werror=return-type -Oz

# ----------------------------

include $(shell cedev-config --makefile)
