##
## EPITECH PROJECT, 2018
## JFEntityComponentSystem
## File description:
## Makefile for the project JFEntityComponentSystem
##

SRC_DIR		=	$(realpath sources)

SRC_FILES	=	\
				Component.cpp \
				ECSWrapper.cpp \
				Entity.cpp \
				EntityManager.cpp \
				EventManager.cpp \
				Exceptions.cpp \
				ID.cpp \
				SystemManager.cpp \

SRC			=	$(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJ			=	$(SRC:%.cpp=%.o)

NAME		=	jfces

STATIC		=	lib$(NAME).a

SHARED		=	lib$(NAME).so

INCLUDE		=	-I $(realpath sources)

CXXFLAGS	=	-W -Wall -Wextra -Werror -Wno-unknown-pragmas -Wunused -Wsynth \
				-Wreorder -Wpacked -Wcast-align -Wenum-compare -Wpointer-arith \
				-Wunused -Wuninitialized -Winit-self -Winvalid-pch -Woverlength-strings  \
				-Woverflow -Wsequence-point -Wno-unknown-pragmas -finput-charset=UTF-8 -Wlogical-op \
				-Wsync-nand -Wstrict-null-sentinel -Wnoexcept -Wunreachable-code \
				-Winit-self -Wfloat-equal -Wtype-limits -Wcast-align -Wredundant-decls \
				-Wno-parentheses

CXXFLAGS	+=	$(INCLUDE)

LDFLAGS		=

CXX			=	g++

DOXYGEN		:=	$(shell command -v doxygen 2> /dev/null)

DOC_PATH	=	./docs

.PHONY: help static shared clean fclean re re_shared gen_doc clean_doc open_doc install uninstall
.DEFAULT_GOAL := help

help:
			@echo "JFECS Makefile Help\n"
			@echo "Available commands:"
			@echo "static:\t\tbuild the static version of the library"
			@echo "$(STATIC):\tbuild the static version of the library"
			@echo "shared:\t\tbuild the dynamic version of the library"
			@echo "$(SHARED):\tbuild the dynamic version of the library"
			@echo "clean:\t\tclean the objects"
			@echo "fclean:\t\tclean the objects and libraries"
			@echo "re:\t\tclean the objects and libraries then rebuild the static library"
			@echo "re_shared:\tclean the objects and libraries then rebuild the dynamic library"
			@echo "gen_doc:\tgenerate the doxygen documentation"
			@echo "clean_doc:\tdelete the doxygen documentation"
			@echo "open_doc:\topen the doxygen documentation in the default browser"
			@echo "install:\tinstall the dynamic library on the system"
			@echo "uninstall:\tuninstall the dynamic library from the system"

static:		$(STATIC)

$(STATIC):	$(OBJ)
			$(AR) -rc $(STATIC) $(OBJ)

shared:		$(SHARED)

$(SHARED):	CXXFLAGS += -fPIC
$(SHARED):	$(OBJ)
			$(CXX) -shared $(OBJ) -o $(SHARED)

clean:
			$(RM) $(OBJ)

fclean:		clean
			$(RM) $(STATIC)
			$(RM) $(SHARED)

re:			fclean $(STATIC)

re_shared:	fclean $(SHARED)

gen_doc:	clean_doc
ifndef DOXYGEN
			$(error "Doxygen wasn't found on your computer!")
endif
			@doxygen Doxyfile

clean_doc:
			$(RM) -r $(DOC_PATH)

open_doc:	gen_doc
			@sensible-browser $(DOC_PATH)/html/index.html

install:	$(SHARED)
			@cp $(SHARED) /usr/local/lib/$(SHARED)
			@mkdir /usr/local/include/jfecs
			@cp -r $(SRC_DIR)/*.hpp /usr/local/include/jfecs/

uninstall:
			$(RM) /usr/local/lib/$(SHARED)
			$(RM) -r /usr/local/include/jfecs/