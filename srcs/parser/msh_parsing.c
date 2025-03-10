/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 02:01:05 by jikarunw          #+#    #+#             */
/*   Updated: 2025/03/10 15:58:19 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast	*msh_get_redirect(t_token **tokens)
{
	t_token	*tmp;
	t_token	*next_token;
	t_ast	*result;

	if (!*tokens)
		return (NULL);
	tmp = *tokens;
	if ((*tokens)->type >= INDIRECT && (*tokens)->type <= HEREDOC)
	{
		if ((*tokens)->type == HEREDOC)
			result = msh_get_heredoc_word(tokens);
		else
			result = create_file_list_redir(tokens, tmp);
		free(tmp->cmd);
		free(tmp);
		tmp = NULL;
		return (result);
	}
	while (*tokens && (*tokens)->next)
	{
		next_token = (*tokens)->next;
		if (next_token->type >= INDIRECT && next_token->type <= HEREDOC)
		{
			if (next_token->type == HEREDOC)
				result = handle_heredoc(tokens, tmp);
			else
				result = handle_redirect(tokens, tmp);
			free(tmp->cmd);
			free(tmp);
			tmp = NULL;
			return (result);
		}
		*tokens = next_token;
	}
	result = msh_get_cmd(&tmp);
	free(tmp->cmd);
	free(tmp);
	return (result);
}

t_ast	*msh_get_cmd(t_token **tokens)
{
	t_ast	*command_node;
	t_token	*current;
	int		arg_count;

	command_node = msh_init_ast(CMD);
	arg_count = count_cmd_arg(*tokens);
	command_node->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!command_node->args)
	{
		free(command_node);
		return (NULL);
	}
	current = *tokens;
	fill_command_args(command_node, current);
	return (command_node);
}

t_ast	*msh_get_pipe(t_token **tokens)
{
	t_token	*tmp;
	t_token	*next_token;
	t_ast	*command_group;

	tmp = *tokens;
	while (*tokens && (*tokens)->next)
	{
		next_token = (*tokens)->next;
		if (next_token->type == PIPE)
			return (create_pipe_node(tokens, tmp, next_token));
		*tokens = next_token;
	}
	command_group = msh_init_ast(CMD_GROUP);
	command_group->left = msh_get_redirect(&tmp);
	return (command_group);
}

t_ast	*msh_get_tokens(t_token **tokens)
{
	t_msh	*msh;

	if (!tokens || !*tokens)
		return (NULL);
	return (msh_get_pipe(tokens));
}
