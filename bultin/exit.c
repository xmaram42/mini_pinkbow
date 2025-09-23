/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:28:23 by ashaheen          #+#    #+#             */
/*   Updated: 2025/09/23 08:42:40 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_within_long_long(const char *s)
{
	size_t				i;
	int				sign;
	unsigned long long	n;
	unsigned long long	limit;
	unsigned long long	digit;

	i = 0;
	sign = 1;
	n = 0;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	if (sign == 1)
		limit = (unsigned long long)LLONG_MAX;
	else
		limit = (unsigned long long)LLONG_MAX + 1ULL;
	while (s[i] && ft_isdigit((unsigned char)s[i]))
	{
		digit = (unsigned long long)(s[i] - '0');
		if (n > limit / 10 || (n == limit / 10 && digit > limit % 10))
			return (0);
		n = (n * 10) + digit;
		i++;
	}
	return (1);
}
int	is_numeric_str(char *s)
{
	int	i;

	i = 0;
	if (!s || s[i] == '\0')
		return (0);
	if (s[i] == '+' || s[i] == '-')
	{
		i++;
		if (s[i] == '\0')
			return (0);
	}
	while (s[i])
	{
		if (ft_isdigit((unsigned char)s[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

unsigned char	normalize_exit_code(long long n)
{
	n %= 256;
	if (n < 0)
		n += 256;
	return ((unsigned char)n);
}

void    cleanup_and_exit(t_shell *shell, t_cmd *cmd, unsigned char status)
{
        free_cmd_list(cmd);
        free_envp(shell->envp);
        free_arr(shell->exp);
        clear_history();
        exit(status);
}

static int	exit_error(int kind, char *arg, t_shell *shell, t_cmd *cmd)
{
	if (kind == 0)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd(": numeric argument required", 2);
		cleanup_and_exit(shell, cmd, 255);
		return (0);
	}
	ft_putendl_fd("minishell: exit: too many arguments", 2);
	shell->exit_code = 1;
	return (1);
}
int	exec_exit(t_cmd *cmd, t_shell *shell, int interactive)
{
	char		**argv;
	long long	n;

	argv = cmd->argv;
	if (interactive)
		ft_putendl_fd("exit", 2);
	if (argv[1] && (!is_numeric_str(argv[1])
			|| !is_within_long_long(argv[1])))
		return (exit_error(0, argv[1], shell, cmd));
	if (argv[1] && argv[2])
		return (exit_error(1, NULL, shell, NULL));
	if (!argv[1])
	{
		cleanup_and_exit(shell, cmd, shell->exit_code);
		return (0);
	}
	n = ft_atoll(argv[1]);
	cleanup_and_exit(shell, cmd, normalize_exit_code(n));
	return (0);
}