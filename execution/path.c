/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:28:33 by ashaheen          #+#    #+#             */
/*   Updated: 2025/08/30 17:21:34 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_path_variable(t_shell *shell)
{
	int	i = 0;

	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], "PATH=", 5) == 0)
			return (shell->envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*build_cmd_path(char **paths, char *cmd)
{
	int		i;
	char	*tmp;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd, t_shell *shell, t_exec *exec, t_cmd *cmd_list)
{
	char	**paths;
	char	*path_str;
	char	*full_path;
	struct stat		path_stat;

	if (!cmd || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) != 0)
		{
			errno = ENOENT;
			error_exit(cmd, exec, cmd_list, 127);
		}
		if (stat(cmd, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		{
			errno = EISDIR;
			error_exit(cmd, exec, cmd_list, 126);
		}
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			error_exit(cmd, exec, cmd_list, 126);
	}
	path_str = find_path_variable(shell);
	if (!path_str)
	{
		errno = ENOENT;
		error_exit(cmd, exec, cmd_list, 127);
	}
	paths = ft_split(path_str, ':');
	if (!paths)
		error_exit("malloc", exec, cmd_list, 1);
	full_path = build_cmd_path(paths, cmd);
	free_arr(paths);
	if (!full_path)
	{
		errno = ENOENT;
		error_exit(cmd, exec, cmd_list, 127);
	}
	return (full_path);
}
