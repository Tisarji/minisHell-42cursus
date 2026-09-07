/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krwongwa <krwongwa@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:26:50 by jikarunw          #+#    #+#             */
/*   Updated: 2025/04/02 17:15:51 by krwongwa         ###   ########.fr       */
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

int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	print_export_error(char *arg)
{
	ft_puterrstr("minishell: export: `");
	ft_puterrstr(arg);
	ft_puterrstr("': not a valid identifier\n");
}

char	**check_export(t_p *list)
{
	char	**temp;

	temp = myft_split(list->args[1], '=');
	if (!temp)
		return (NULL);
	if (!temp[0])
	{
		free2d(temp);
		return (NULL);
	}
	return (temp);
}

int	add_node(t_tuple **data, t_p *list)
{
	t_tuple	*new_node;

	new_node = malloc(sizeof(t_tuple));
	if (new_node == NULL)
		return (EXIT_FAILURE);
	new_node->key = NULL;
	new_node->value = NULL;
	if (make_tuple(new_node, list->args[1], '=') == NULL)
	{
		if (new_node->key)
			free(new_node->key);
		if (new_node->value)
			free(new_node->value);
		free(new_node);
		return (EXIT_FAILURE);
	}
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

int	msh_export(t_p *list)
{
	t_tuple	**data;
	char	**temp;

	if (list->args[1] == NULL)
		return (export_no_args(list));
	data = &list->msh->tuple;
	temp = check_export(list);
	if (!temp)
		return (EXIT_FAILURE);
	if (!is_valid_identifier(temp[0]))
	{
		print_export_error(list->args[1]);
		free2d(temp);
		return (EXIT_FAILURE);
	}
	if (get_value_from_key(*data, temp[0]))
	{
		updata_value_from_key(*data, temp[0], copy(temp[1]));
		free2d(temp);
		return (EXIT_SUCCESS);
	}
	free2d(temp);
	return (add_node(data, list));
}
