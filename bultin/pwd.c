/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 18:04:39 by maram             #+#    #+#             */
/*   Updated: 2025/10/02 10:01:41 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     exec_pwd(char **av, t_shell *shell)
{
        char    *cwd;
        char    *logical_pwd;

        if (av[1] != NULL && av[1][0] == '-')
        {
                ft_putstr_fd("minishell: pwd: ", 2);
                ft_putstr_fd(av[1], 2);
                ft_putstr_fd(": invalid option\n", 2);
                return (1);
        }
        cwd = getcwd(NULL, 0);
        if (cwd == NULL)
        {
                logical_pwd = dup_env_pwd(shell);
                if (logical_pwd == NULL)
                {
                        perror("minishell: pwd");
                        return (1);
                }
                ft_putendl_fd(logical_pwd, STDOUT_FILENO);
                free(logical_pwd);
                return (0);
        }
        ft_putendl_fd(cwd, STDOUT_FILENO);
        free(cwd);
        return (0);
}
