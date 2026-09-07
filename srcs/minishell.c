/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 08:25:02 by krwongwa          #+#    #+#             */
/*   Updated: 2026/09/07 14:36:47 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile sig_atomic_t	g_signal;

void	check_signal(int signal)
{
	if (signal == SIGINT && g_signal == 0)
	{
		rl_on_new_line();
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_redisplay();
		return ;
	}
	if (signal == SIGQUIT)
	{
		rl_redisplay();
		return ;
	}
}

void	setup_signal(void)
{
	struct sigaction	act;

	g_signal = 0;
	ft_bzero(&act, sizeof(struct sigaction));
	act.sa_handler = &check_signal;
	act.sa_flags = SA_RESTART;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
	ft_bzero(&act, sizeof(struct sigaction));
}

char	*ft_readline(void)
{
	char	*input;
	char	*promt;

	promt = get_promt();
	input = readline(promt);
	if (promt)
		free(promt);
	if (input != NULL && *input != '\0')
		add_history(input);
	return (input);
}

void	msh_loop(t_msh *msh)
{
	t_token	*token_cursor;

	while (1)
	{
		msh->input = ft_readline();
		if (!msh->input)
		{
			// printf("%sEXIT!%s💥\n", RED, RESET);
			printf("exit\n");
			cleanup_and_exit(msh, 0);
		}
		msh->token = msh_parsing_input(msh);
		if (msh->token)
		{
			if (!validate_tokens(msh->token))
			{
				free_cmd_tokens(&msh->token);
				continue ;
			}
			process_expansion(msh);
			if (!msh->token)
				continue ;
			token_cursor = msh->token;
			msh->ast = msh_get_tokens(&token_cursor);
			msh->token = NULL;
			main_exe(msh);
		}
		// display_ast_table(msh->ast, 0);
	}
}

int	main(int ac, char **av, char **env)
{
	t_msh	*msh;

	(void) ac;
	(void) av;
	msh = malloc(sizeof(t_msh));
	if (!msh)
		return (1);
	init_minishell(msh, env);
	setup_signal();
	msh_loop(msh);
	cleanup_and_exit(msh, 0);
	return (0);
}
