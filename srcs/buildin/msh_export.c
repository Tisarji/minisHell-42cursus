/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krwongwa <krwongwa@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:26:50 by jikarunw          #+#    #+#             */
/*   Updated: 2025/03/17 20:20:01 by krwongwa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	sort_env(t_tuple **env)
{
	t_tuple	*current;
	t_tuple	*next;
	char	*temp;

	current = *env;
	while (current)
	{
		next = current->next;
		while (next)
		{
			if (ft_strcmp(current->key, next->key) > 0)
			{
				temp = current->key;
				current->key = next->key;
				next->key = temp;
				temp = current->value;
				current->value = next->value;
				next->value = temp;
			}
			next = next->next;
		}
		current = current->next;
	}
}

int	export_no_args(t_p *list)
{
	t_tuple	*current;
	int		saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	current = list->msh->tuple;
	sort_env(&current);
	handle_fd(list);
	print_tuple(current);
	if (list->fd_out != 1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	return (EXIT_SUCCESS);
}

int	check_export(t_p *list)
{
	char	**temp;

	temp = myft_split(list->args[1], '=');
	if (!temp[0] || !temp[1])
	{
		free2d(temp);
		return (1);
	}
	free2d(temp);
	return (0);
}

int	msh_export(t_p *list)
{
	t_tuple	*new_node;
	t_tuple	**data;
	
	if (list->args[1] == NULL)
		return (export_no_args(list));
	data = &list->msh->tuple;
	if (check_export(list))
		return (EXIT_FAILURE);
	new_node = malloc(sizeof(t_tuple));
	if (new_node == NULL || make_tuple(new_node, list->args[1], '=') == NULL)
		return (EXIT_FAILURE);
	new_node->next = NULL;
	if (!*data)
	{
		*data = new_node;
		(*data)->tail = new_node;
	}
	else
	{
		(*data)->tail->next = new_node;
		(*data)->tail = new_node;
	}
	return (EXIT_SUCCESS);
}
