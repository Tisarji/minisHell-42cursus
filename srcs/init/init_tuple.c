/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tuple.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krwongwa <krwongwa@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 14:24:33 by krwongwa          #+#    #+#             */
/*   Updated: 2025/03/17 15:40:31 by krwongwa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*make_tuple(t_tuple *new_node, char *str, char c)
{
	size_t	i;
	size_t	n;
	char	*temp;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	temp = malloc(sizeof(char) * (i + 1));
	if (temp == NULL)
		return (NULL);
	temp[0] = '\0';
	new_node->key = temp;
	ft_strlcat(temp, str, i + 1);
	if (!str[i])
		n = 0;
	else
		n = ft_strlen(str) - i;
	temp = malloc(sizeof(char) * (n + 1));
	if (temp == NULL)
		return (NULL);
	temp[0] = '\0';
	if (str[i])
		ft_strlcat(temp, (str + i + 1), n);
	new_node->value = temp;
	return ((void *)(1));
}

int	init_tuple(t_msh *data, char **env)
{
	size_t	i;
	t_tuple	*new_node;

	i = -1;
	while (env[++i])
	{
		new_node = malloc(sizeof(t_tuple));
		if (new_node == NULL)
			return (0);
		new_node->next = NULL;
		if (make_tuple(new_node, env[i], '=') == NULL)
			return (0);
		if (!data->tuple)
		{
			data->tuple = new_node;
			data->tuple->tail = new_node;
		}
		else
		{
			data->tuple->tail->next = new_node;
			data->tuple->tail = new_node;
		}
	}
	return (1);
}
