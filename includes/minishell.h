/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:29:56 by jikarunw          #+#    #+#             */
/*   Updated: 2025/04/02 22:34:28 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define GREEN "\033[1;32m"
# define RED "\033[1;31m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[1;34m"
# define RESET "\033[0m"

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdint.h>
# include <signal.h>
# include <string.h>
# include "../libft/includes/libft.h"
# include <stdbool.h>
# include <errno.h>

# include <sys/wait.h>
# include <fcntl.h>

# include "../libft/includes/libft.h"
# include "../libft/includes/get_next_line.h"
# include "./struct.h"

# define STDERR_FILENO 2
# define WHITESPACE " \t\n\v\f\r"
# define LETTERS_DIGITS "abcdefghijklmnopqrstuvwxyzABCDEFGH \
	IJKLMNOPQRSTUVWXYZ0123456789_"

/**************
 * SRCS/UTILS *
 **************/
void		print_tuple(t_tuple *data);
char		*get_value_from_key(t_tuple *data, char *key);
void		updata_value_from_key(t_tuple *data, char *key, char *new_value);
void		remove_tuple(t_tuple **data, char *key);
void		clear_tuple(t_tuple **data);

void		free_ast(t_ast *ast);
void		ft_free(t_msh *data);
void		ft_error(t_msh *data, char *word);
void		cleanup_and_exit(t_msh *msh, int exit_code);
char		*get_promt(void);

void		clean_up_token(t_token **tokens);

/*************
 * SRCS/INIT *
 *************/
int			init_tuple(t_msh *data, char **env);
void		*make_tuple(t_tuple *new_node, char *str, char c);

void		init_minishell(t_msh *data, char **env);

/** init_parsing */
t_ast		*file_ast_node(t_token *token);
t_ast		*msh_init_ast(t_type type);
t_token		*msh_create_token(t_type type, char *value, size_t len);

/****************
 * SRCS/BUILTIN *
 ****************/
void		handle_fd(t_p *list);
int			msh_execute_builtin(t_p *list);
char		*copy(const char *s);
int			msh_exit(t_p *list);
char		*ft_getcwd(void);
int			msh_pwd(t_p *list);
int			msh_echo(t_p *list);
int			msh_cd(t_p *list);
int			msh_env(t_p *list);
int			msh_export(t_p *list);
int			msh_unset(t_p *list);

/***************
 * SRCS/EXPAND *
 ***************/
/** expand_utils_01 */
// char		*handle_single_quotes(char **str);
char		*handle_single_quotes(t_msh *shell, char **str, int expand_vars);
char		*handle_double_quotes(t_msh *shell, char **str);
char		*handle_plain_text(char **str);
char		*get_special_variable_value(t_msh *shell, char **str);
char		*get_normal_variable_value(t_msh *shell, char **str);

/** msh_expand */
char		*get_env_value(t_msh *shell, char *key);
char		*process_segment(t_msh *shell, char **str, int expand_vars);

char		*expand_variable_value(t_msh *shell, char **str);
char		*expand_string(t_msh *shell, char *str, int expand_vars);
void		process_expansion(t_msh *shell);

/***************
 * SRCS/PARSER *
 ***************/
/** parsing_utils_01 */
int			validate_tokens(t_token *tokens);
int			count_cmd_arg(t_token *current);
void		add_cmd_arg(t_ast *cmd_node, t_token **tokens, int arg_count);
void		free_cmd_tokens(t_token **tokens);
void		free_cmd_args(t_ast *cmd_node);

/** parsing_heredoc */
t_ast		*create_heredoc_node_command(t_token **tokens);
t_ast		*handle_heredoc(t_token **tokens, t_ast *cmd_node);
// t_ast		*handle_heredoc(t_token **tokens);
int			init_cmd_node_args(t_ast *cmd_node, t_token **tokens);
void		cleanup_cmd_node(t_ast *cmd_node);

/** msh_parsing_suport */
int			init_cmd_args(t_ast *cmd_node, t_token **tokens);
int			copy_cmd_args(t_ast *cmd_node, t_token **tokens);
void		process_heredoc_if_needed(t_token **tokens, t_ast *cmd_node);

/** parsing_pipe */
t_ast		*create_pipe_node(t_token **tokens, \
		t_token *tmp, t_token *next_token);
t_ast		*msh_get_pipe(t_token **tokens);
t_ast		*create_env_var_node(t_token *current);

/** parsing_heredoc */
void		free_heredoc_nodes(t_ast *node);
t_ast		*create_heredoc_node(t_token **token);
t_ast		*create_heredoc_word_node(t_token **token);
t_ast		*msh_get_heredoc_word(t_token **token);

/** parsing_redir */
t_ast		*create_file_list_redir(t_token **tokens, t_token *tmp);
t_ast		*handle_redirect(t_token **tokens, t_token *tmp);
t_ast		*process_redirection_tokens(t_token **tokens, t_token *tmp);
t_ast		*msh_get_redirect(t_token **tokens);

/** msh_parsing */
int			allocate_cmd_args(t_ast *command_node, int arg_count);
int			copy_command_args(t_ast *command_node, t_token **tokens);
void		fill_command_args(t_ast *command_node, t_token **tokens);
t_ast		*msh_get_cmd(t_token **tokens);
t_ast		*msh_get_tokens(t_token **tokens);

/** msh_syntax */
int			has_unclosed_quotes(const char *input);
int			has_invalid_redirections(const char *input);
int			has_misplaced_operators(const char *input);
int			has_logical_operators(const char *input);
int			syntax_error_checker(const char *input);

/** syntax_utils_01 */
void		update_quote_counts(char c, int *s_q_count, int *d_q_count);
int			is_invalid_operator(const char **input);
int			has_valid_heredoc(const char *input);
int			check_redirection_end(const char *input);
int			handle_special_redirection(const char *input);

/**************
 * SRCS/TOKEN *
 **************/
bool		check_tokens(t_token *tokens);
void		free_token(t_token **token);
t_token		*msh_parsing_input(t_msh *msh);
t_token		*token_input(char *input);
void		msh_count_pipe(t_msh *msh);

t_token		*new_token(t_type type, char *value);
void		add_token_to_list(t_token **tokens, t_token *new_token);
void		add_word_token_if_valid(char **start, \
	char **input, t_token **tokens);
void		parse_cmd(char **input, t_token **tokens);
void		parse_type(char **input, t_token **tokens);

void		update_quote_status(char c, int *in_quote, char *quote_char);
// void		parse_type(char **input, t_token **tokens, t_msh *msh);
void		free_cmd_args(t_ast *cmd_node);

/** File: Test Function for show Display */
const char	*msh_name_type(t_type type);
void		test_delete_heredoc(t_token **tokens);
void		display_ast_table(t_ast *ast, int level);
void		display_tokens(t_token *tokens);
void		display_ast(t_ast *ast, int idx);
void		print_token_list(t_token *token);

/************
 * SRCS/EXE *
 ************/
int			find_slash(char *command);
int			do_here_doc(t_ast *ast, t_ast *temp, t_p *list);
void		is_build_in_command(t_ast *ast, int *isBuildIn);
void		check_build_in_command(char *word, int *isBuildIn);
// int 		check_build_in_command(char *word);
void		do_here_doc_task(t_ast *ast, t_p *list, int *isBuildIn);
void		free2d(char **str);
void		main_exe(t_msh *msh);
void		safe_close(int *fd);
void		safe_fd(t_p *list, int flag);
void		prepare_cmd(t_ast *ast, t_p *list, int *status);
void		open_in_file(char *argv, t_p *list, int *status);
void		open_out_file(char *argv, t_p *list, int flag, int *status);
void		pipe_write(t_p *list);
char		*find_path(char *cmd, char **path);
void		pipe_task(t_ast *ast, t_p *list);
void		clear_list(t_p *list);
void		free_list(t_p *list);
void		ft_puterrstr(char *s);
int			ft_puterrorcmd(char *s, int errnum);
void		check_signal(int signal);
void		mode_signal_exe(int mode);
void		wait_all_process(t_p *list);
void		exe_single_cmd(t_ast *ast, t_p *list);
char		**myft_split(char const *s, char c);
char		*myft_strjoin(char const *s1, char const *s2);

#endif
