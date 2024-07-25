NAME		:=	app
COMPILER	:=	c++
FLAGS		:= 	-Wall -Wextra -Werror -std=c++11 
LIBSDL		:=	./lib/SDL
SRC			:=  main.cpp 		\

SRCDIR 		:= 	./src
OBJDIR 		:= 	./obj
OBJ			:= 	$(addprefix $(OBJDIR)/,$(SRC:.cpp=.o))
SRC			:= 	$(addprefix $(SRCDIR)/,$(SRC))
HEADERS		:=	-I ./inc -I $(LIBSDL)/include/SDL3
LIBS		:=	-L$(LIBSDL)/build -lSDL3

DIRNAME		:= $(shell basename $(PWD))

ifdef DEBUG
	FLAGS += -g
endif

ifdef LEAKS
	FLAGS += -g -fsanitize=address
endif

all: libsdl $(NAME)

run: all
	LD_LIBRARY_PATH=$(LIBSDL)/build:$$LD_LIBRARY_PATH ./$(NAME)
	git submodule add https://github.com/libsdl-org/SDL.git lib/SDL

$(NAME): $(OBJ)
	@$(COMPILER) $^ $(LIBS) $(HEADERS) -o $(NAME)
	@echo $(Green) $(Bold) C++ $(DIRNAME) compiled ✅ $(Text_Off)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@mkdir -p $(@D) 
	@$(COMPILER) $(FLAGS) $(HEADERS) -c $< -o $@

$(OBJDIR):
	@mkdir $@

libsdl:
	@cmake $(LIBSDL) -B $(LIBSDL)/build && cmake --build $(LIBSDL)/build && cmake --install $(LIBSDL)/build

clean:
	@rm -rf $(OBJDIR)
	@echo $(Yellow) Cleaned object files! $(Text_Off)

fclean:	clean
	@rm -f $(NAME) 
	@rm -rf $(LIBSDL)/build
	@echo $(Yellow) Cleaned executable! $(Text_Off)

re:	fclean all

.PHONY:	all, clean, fclean, re

# Colors #############################################
Black		= "\033[0;30m"		# Black
Red			= "\033[0;31m"		# Red
Green		= "\033[0;32m"		# Green
Yellow		= "\033[0;33m"		# Yellow
Blue		= "\033[0;34m"		# Blue
Purple		= "\033[0;35m"		# Purple
Cyan		= "\033[0;36m"		# Cyan
White		= "\033[0;37m"		# White
Text_Off	= "\033[0m"			# Text Style Off
Bold		= "\033[1m"			# Text Style Bold
######################################################
