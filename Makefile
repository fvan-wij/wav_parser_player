NAME		:=	app
COMPILER	:=	c++
FLAGS		:= 	-Wall -Wextra -Werror -std=c++11 
SRC			:=  main.cpp 		\

SRCDIR 		:= 	./src
OBJDIR 		:= 	./obj
OBJ			:= 	$(addprefix $(OBJDIR)/,$(SRC:.cpp=.o))
SRC			:= 	$(addprefix $(SRCDIR)/,$(SRC))

EXERCISE	:= $(shell basename $(PWD))

ifdef DEBUG
	FLAGS += -g
endif

ifdef LEAKS
	FLAGS += -g -fsanitize=address
endif

all: $(NAME)

run: all

$(NAME): $(OBJ)
	@$(COMPILER) $^ -o $(NAME)
	@echo $(Green) $(Bold) C++ $(EXERCISE) compiled ✅ $(Text_Off)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@mkdir -p $(@D) 
	@$(COMPILER) $(FLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir $@

clean:
	@rm -rf $(OBJDIR)
	@echo $(Yellow) Cleaned object files! $(Text_Off)

fclean:	clean
	@rm -f $(NAME) 
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
