/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maram <maram@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:59:14 by maram             #+#    #+#             */
/*   Updated: 2025/08/31 10:30:58 by maram            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_env(char **av, t_shell *shell)
{
	int	i;

	if (av[1] != NULL)
	{
		if (av[1][0] == '-')
		{
			ft_putstr_fd("minishell: env: ", 2);
			ft_putstr_fd(av[1], 2);
			ft_putstr_fd(": invalid option\n", 2);
			return (125);
		}
		ft_putstr_fd("minishell: env: ", 2);
		ft_putstr_fd(av[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (127);
	}
	i = 0;
	while (shell->envp[i])
	{
		printf("%s\n", shell->envp[i]);
		i++;
	}
	return (0);
}

char	*get_env_value(char *name, t_shell *shell)
{
	size_t	name_len;
	int		i;

	name_len = ft_strlen(name);
	i = 0;
	while (shell->envp && shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], name, name_len) == 0
			&& shell->envp[i][name_len] == '=')
			return (shell->envp[i] + name_len + 1);
		i++;
	}
	return (NULL);
}

int	env_count(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

void	free_envp(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}