/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:48:01 by krwongwa          #+#    #+#             */
/*   Updated: 2025/03/17 20:37:13 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile sig_atomic_t	g_signal;

int	clear_read_line(void)
{
	if (g_signal == 1)
	{
		rl_on_new_line();
		rl_replace_line("\n", 1);
		rl_redisplay();
		rl_done = 1;
	}
	return (0);
}

void	here_doc_check_signal(int sig)
{
	if (sig == SIGINT)
		g_signal = 1;
}

void	end_here_doc(t_p *list)
{
	if (g_signal == 1)
		*list->code = 130;
	else
		*list->code = 0;
	g_signal = 0;
	signal(SIGINT, &check_signal);
	rl_event_hook = NULL;
}

int	init_here_doc(t_ast *ast, t_ast *temp, t_p *list)
{
	if (ast->type == HEREDOC_WORD)
		list->here_doc_cut = ast->args[0];
	if (pipe(list->pipe) == -1)
	{
		ft_puterrstr("PIPE ERROR");
		return (-1);
	}
	if (temp != NULL)
	{
		if (temp->type == REDIRECT)
			list->fd_out = open(temp->right->args[0],
					O_RDWR | O_TRUNC | O_CREAT, 0644);
		else if (temp->type == APPEND)
			list->fd_out = open(temp->right->args[0],
					O_RDWR | O_APPEND | O_TRUNC | O_CREAT, 0644);
	}
	g_signal = 0;
	signal(SIGINT, &here_doc_check_signal);
	rl_event_hook = &clear_read_line;
	if (list->fd_out != -1)
	{
		dup2(list->fd_out, list->pipe[1]);
		safe_fd(list, 1);
	}
	return (0);
}

int	do_here_doc(t_ast *ast, t_ast *temp, t_p *list)
{
	char	*getline;
	int		real_fd_in;

	if (init_here_doc(ast, temp, list) == -1)
		return (-1);
	while (1)
	{
		getline = readline(">");
		if (getline == NULL || g_signal == 1
			|| ft_strncmp(getline, list->here_doc_cut,
				ft_strlen(list->here_doc_cut)) == 0)
			break ;
		write(list->pipe[1], getline, ft_strlen(getline));
		write(list->pipe[1], "\n", 1);
		free(getline);
	}
	end_here_doc(list);
	if (getline)
		free(getline);
	safe_close(&list->pipe[1]);
	real_fd_in = dup(list->pipe[0]);
	safe_close(&list->pipe[0]);
	return (real_fd_in);
}
