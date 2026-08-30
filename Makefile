COLOR_RESET = \033[0m
COLOR_YELLOW = \033[1;33m
COLOR_CYAN = \033[1;36m
COLOR_RED = \033[91m
COLOR_GREEN = \033[92m
COLOR_PINK = \033[95m
COLOR_GRAY = \033[90m

NAME = minishell
PATH_HEADER = includes
PATH_LIBFT = libft
PATH_SRCS = srcs
OBJ_DIR = objs

# CC = cc
# CFLAGS = -Wall -Wextra -Werror -I$(PATH_HEADER) -I$(PATH_LIBFT) -g
CFLAGS = -g -I$(PATH_LIBFT) -I$(PATH_HEADER)
LDFLAGS = -L$(PATH_LIBFT) -lft -lreadline

RM = rm -rf

INIT_DIR = srcs/init/
INIT_FILE =	init_tuple.c	\
			init_parsing.c	\
			init_minishell.c
INIT_SRC = $(addprefix $(INIT_DIR), $(INIT_FILE))

UTILS_DIR = srcs/utils/
UTILS_FILE =	utils.c			\
				tuple_list.c	\
				tuple_list2.c	\
				promt.c
UTILS_SRC = $(addprefix $(UTILS_DIR), $(UTILS_FILE))

EXE_DIR = srcs/exe/
EXE_FILE =	check_buildin.c \
			error.c \
			exe_utils.c \
			exe_utils2.c \
			here_doc_utils.c  \
			ft_split.c \
			here_doc.c \
			list_utils.c \
			list_utils2.c \
			main_exe.c \
			pipe.c
EXE_SRC = $(addprefix $(EXE_DIR), $(EXE_FILE))

BUILDIN_DIR = srcs/buildin/
BUILDIN_FILE =	msh_buildin.c	\
				msh_pwd.c		\
				msh_echo.c		\
				msh_env.c		\
				msh_exit.c		\
				msh_export.c	\
				msh_unset.c		\
				msh_cd.c
BUILDIN_SRC = $(addprefix $(BUILDIN_DIR), $(BUILDIN_FILE))

EXPAND_DIR = srcs/expand/
EXPAND_FILE =	msh_expand.c		\
				expand_utils_01.c
EXPAND_SRC = $(addprefix $(EXPAND_DIR), $(EXPAND_FILE))

PARSER_DIR = srcs/parser/
PARSER_FILE =	msh_parsing.c			\
				msh_syntax.c			\
				syntax_utils_01.c		\
				parsing_heredoc.c		\
				parsing_pipe.c			\
				parsing_redir.c			\
				parsing_redir_utils.c	\
				parsing_utils.c			\
				msh_parsing_support.c	\
				msh_parsing_heredoc.c
PARSER_SRC = $(addprefix $(PARSER_DIR), $(PARSER_FILE))

TOKEN_DIR = srcs/token/
TOKEN_FILE =	token_utils_01.c	\
				token_utils_02.c	\
				msh_print_token.c	\
				msh_token.c
TOKEN_SRC = $(addprefix $(TOKEN_DIR), $(TOKEN_FILE))

SRC = $(UTILS_SRC) $(INIT_SRC) $(EXPAND_SRC) $(TOKEN_SRC) $(PARSER_SRC) $(BUILDIN_SRC) $(EXE_SRC) ./srcs/minishell.c

OBJ = $(SRC:$(PATH_SRCS)/%.c=$(OBJ_DIR)/%.o)
TOTAL_FILES := $(words $(filter %.o,$(OBJ)))

$(OBJ_DIR)/%.o: $(PATH_SRCS)/%.c $(PATH_HEADER)
	@mkdir -p $(@D)
	@$(CC) -c $(CFLAGS) $< -o $@
	@count=$$(find $(OBJ_DIR) -name "*.o" 2>/dev/null | wc -l | tr -d ' '); \
	printf "\033[2K\r$(COLOR_YELLOW)Compiling minishell... [%s/%s]$(COLOR_RESET) %s" \
		"$$count" "$(TOTAL_FILES)" "$<"

all : $(NAME)

$(NAME) : $(OBJ)
	@printf "\n"
	@printf "$(COLOR_GRAY)Entering directory '$(PATH_LIBFT)'$(COLOR_RESET)\n"
	@make -C $(PATH_LIBFT) --no-print-directory
	@printf "$(COLOR_GRAY)Leaving directory '$(PATH_LIBFT)'$(COLOR_RESET)\n"
	@$(CC) $(OBJ) $(CFLAGS) $(LDFLAGS) -o $(NAME)
	@printf "\n"
	@echo "[$(COLOR_YELLOW)$(NAME) --> OK$(COLOR_RESET)]\n ${COLOR_GREEN}Success!${COLOR_RESET}"
	@echo "$(COLOR_PINK)\tUsage: ./minishell$(COLOR_RESET)"

clean :
	@echo "$(COLOR_RED)Cleaning object minishell files...$(COLOR_RESET)"
	@make clean -C $(PATH_LIBFT)
	@$(RM) $(OBJ_DIR)

fclean : clean
	@echo "$(COLOR_RED)Cleaning Minishell$(COLOR_RESET)$(COLOR_PINK) $(NAME)!$(COLOR_RESET)"
	@make fclean -C $(PATH_LIBFT)
	@$(RM) $(NAME)

re : fclean all

.PHONY : re fclean clean all
