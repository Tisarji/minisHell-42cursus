/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   promt.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krwongwa <krwongwa@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 21:17:02 by krwongwa          #+#    #+#             */
/*   Updated: 2025/01/08 10:16:02 by krwongwa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_promt(t_msh *data)
{
	char	*ans;
	char	*temp;
	char	*getpath;

	getpath = get_value_from_key(data->tuple, "PWD");
	if (getpath == NULL)
		getpath = ft_getcwd();
	ans = ft_strdup("minishell:");
	temp = ft_strjoin(ans, " ");
	ans = temp;
	temp = ft_itoa(data->code);
	ans = ft_strjoin(ans, temp);
	free(temp);
	temp = ft_strjoin(ans, "$ ");
	ans = temp;
	return (ans);
}
