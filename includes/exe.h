/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krwongwa <krwongwa@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 22:08:15 by jikarunw          #+#    #+#             */
/*   Updated: 2024/07/14 21:27:02 by krwongwa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_H
# define EXE_H

typedef struct tuple
{
	char		*key;
	char		*value;
	struct	tuple *next;
	struct	tuple *tail;
} t_tuple;

typedef struct p_pipe
{
	int		*process_pid;
	int		status_pid;
	char	**env;
	char	**path;
	char	*cmd;
	int		fd_in;
	int		fd_out;
}	t_p;

typedef struct	e_env
{
	t_tuple	*tuple;
	char	**env;
	char	*home_path;
	int		code;
}	t_env;

#endif
