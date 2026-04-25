/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 12:03:44 by jikarunw          #+#    #+#             */
/*   Updated: 2025/04/02 22:45:14 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast	*create_env_var_node(t_token *current)
{
	t_ast	*env_var_node;

	if (!current || !current->cmd)
		return (NULL);
	env_var_node = msh_init_ast(ENV_VAR);
	if (!env_var_node)
		return (NULL);
	env_var_node->args = malloc(sizeof(char *) * 2);
	if (!env_var_node->args)
	{
		free(env_var_node);
		return (NULL);
	}
	env_var_node->args[0] = ft_strdup(current->cmd);
	if (!env_var_node->args[0])
	{
		free(env_var_node->args);
		free(env_var_node);
		return (NULL);
	}
	env_var_node->args[1] = NULL;
	return (env_var_node);
}

t_ast	*create_pipe_node(t_token **tokens, t_token *tmp, t_token *next_token)
{
	t_ast	*pipe_node;
	t_ast	*command_group;

	if (!tokens || !*tokens || !next_token)
		return (NULL);
	pipe_node = msh_init_ast(next_token->type);
	if (!pipe_node)
		return (NULL);
	(*tokens)->next = NULL;
	command_group = msh_init_ast(CMD_GROUP);
	if (!command_group)
	{
		free(pipe_node);
		return (NULL);
	}
	command_group->left = msh_get_redirect(&tmp);
	pipe_node->left = command_group;
	pipe_node->right = msh_get_pipe(&(next_token->next));
	free(next_token->cmd);
	free(next_token);
	return (pipe_node);
}

t_ast	*msh_get_pipe(t_token **tokens)
{
	t_token	*tmp;
	t_token	*next_token;
	t_ast	*command_group;

	if (!tokens || !*tokens)
		return (NULL);
	tmp = *tokens;
	while (*tokens && (*tokens)->next)
	{
		next_token = (*tokens)->next;
		if (next_token->type == PIPE && (!next_token->next || \
			next_token->next->type == PIPE))
			return (NULL);
		if (next_token->type == PIPE)
			return (create_pipe_node(tokens, tmp, next_token));
		*tokens = next_token;
	}
	command_group = msh_init_ast(CMD_GROUP);
	if (!command_group)
		return (NULL);
	command_group->left = msh_get_redirect(&tmp);
	return (command_group);
}
