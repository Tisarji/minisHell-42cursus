/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_parsing_support.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jikarunw <jikarunw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 01:18:08 by jikarunw          #+#    #+#             */
/*   Updated: 2025/03/31 02:44:35 by jikarunw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	allocate_cmd_args(t_ast *command_node, int arg_count)
{
	if (!command_node)
		return (0);
	command_node->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!command_node->args)
		return (0);
	return (1);
}

int copy_command_args(t_ast *command_node, t_token **tokens)
{
	int		i;
	t_token	*current;

	i = 0;
	current = *tokens;
	while (current && current->type != HEREDOC)
	{
		if (current->type == CMD || current->type == ARG)
		{
			command_node->args[i] = ft_strdup(current->cmd);
			if (!command_node->args[i])
			{
				free_cmd_args(command_node);
				return (0);
			}
			i++;
		}
		current = current->next;
	}
	command_node->args[i] = NULL;
	*tokens = current;
	return (1);
}

int	init_cmd_args(t_ast *cmd_node, t_token **tokens)
{
	int	arg_count;

	arg_count = count_cmd_arg(*tokens);
	if (!allocate_cmd_args(cmd_node, arg_count))
		return (0);
	return (1);
}

int	copy_cmd_args(t_ast *cmd_node, t_token **tokens)
{
	if (!copy_command_args(cmd_node, tokens))
	{
		free_cmd_args(cmd_node);
		return (0);
	}
	return (1);
}

void	fill_command_args(t_ast *command_node, t_token *current)
{
	int	i;

	i = 0;
	while (current && current->type != PIPE)
	{
		if (current->type == ENV_VAR)
			command_node->left = create_env_var_node(current);
		else
			command_node->args[i++] = ft_strdup(current->cmd);
		current = current->next;
	}
	command_node->args[i] = NULL;
}
