/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tuple_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krwongwa <krwongwa@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 14:51:08 by krwongwa          #+#    #+#             */
/*   Updated: 2025/03/17 17:30:52 by krwongwa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tuple(t_tuple *data)
{
	while (data != NULL)
	{
		ft_putstr_fd(data->key, 1);
		ft_putstr_fd("=", 1);
		ft_putstr_fd(data->value, 1);
		write(1, "\n", 1);
		data = data->next;
	}
}

char	*get_value_from_key(t_tuple *data, char *key)
{
	char	*value;

	value = NULL;
	while (data)
	{
		if (!ft_strncmp(data->key, key, ft_strlen(key)))
			value = data->value;
		data = data->next;
	}
	return (value);
}

void	append_tuple_node(t_tuple *head, char *key, char *value)
{
	t_tuple	*new_node;

	new_node = malloc(sizeof(t_tuple));
	if (!new_node)
	{
		if (value)
			free(value);
		return ;
	}
	new_node->key = ft_strdup(key);
	new_node->value = value;
	new_node->next = NULL;
	new_node->tail = NULL;
	if (!head)
		return ;
	head->tail->next = new_node;
	head->tail = new_node;
}

void	updata_value_from_key(t_tuple *data, char *key, char *new_value)
{
	t_tuple	*head;

	head = data;
	while (data)
	{
		if (!ft_strncmp(data->key, key, ft_strlen(key)))
		{
			free(data->value);
			data->value = new_value;
			return ;
		}
		data = data->next;
	}
	append_tuple_node(head, key, new_value);
}

void	clear_tuple(t_tuple **data)
{
	t_tuple	*cur;
	t_tuple	*temp;

	cur = *data;
	while (cur)
	{
		temp = cur;
		cur = cur->next;
		if (temp->key)
			free(temp->key);
		if (temp->value)
			free(temp->value);
		if (temp)
			free(temp);
	}
	*data = NULL;
}
