#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include "../libft/includes/libft.h"
# include "exe.h"
# include "parser.h"


/***************************
 * SRCS/BUILDIN/FT_PWD.C ***
 ***************************/
char	*ft_pwd(t_tuple *list);


/***************************
 * SRCS/utils/TUPLE_LIST.C *
 ***************************/
void	print_tuple(t_tuple *data);
char	*get_value_from_key(t_tuple *data,char *key);
void	updata_value_from_key(t_tuple *data, char *key, char *new_value);
void	remove_tuple(t_tuple **data, char *key);
void	clear_tuple(t_tuple **data);

/***************************
 * SRCS/utils/utils.C ******
 ***************************/
void	ft_free(t_env *data);
void	ft_error(t_env *data,char *word);

/***************************
 * SRCS/utils/PRIMT.C ******
 ***************************/
char	*get_promt(t_env *data);

/******************************
 * SRCS/INIT/INIT_MINISHELL.C *
 ******************************/
void	init_minishell(t_env *data, char **env);

/**************************
 * SRCS/INIT/INIT_TUPLE.C *
 **************************/
int	init_tuple(t_tuple **data,char **env);
void	*make_tuple(t_tuple *new_node,char *str,char c);

#endif
