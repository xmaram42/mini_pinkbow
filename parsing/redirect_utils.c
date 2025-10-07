/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maabdulr <maabdulr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 17:55:27 by maram             #+#    #+#             */
/*   Updated: 2025/10/07 16:44:14 by maabdulr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_read_fd(t_cmd *cmd, const char *filename)
{
	if (cmd->infile != -1)
		close(cmd->infile);
	cmd->infile = open(filename, O_RDONLY);
	if (cmd->infile == -1)
	{
		errno_msg(filename);
		cmd->redir_error = 1;
		return (0);
	}
	return (1);
}

void	handle_redir_in(t_cmd *cmd, t_token **token_ptr)
{
	char	*filename;

	if (!*token_ptr || !(*token_ptr)->next)
		return ;
	*token_ptr = (*token_ptr)->next;
	if ((*token_ptr)->ambiguous)
	{
		cmd->redir_error = 1;
		*token_ptr = (*token_ptr)->next;
		return ;
	}
	filename = ft_strdup((*token_ptr)->value);
	if (!filename)
	{
		cmd->redir_error = 1;
		return ;
	}
	if (!open_read_fd(cmd, filename))
	{
		free(filename);
		*token_ptr = (*token_ptr)->next;
		return ;
	}
	free(filename);
	*token_ptr = (*token_ptr)->next;
}

static int	open_outfile(t_cmd *cmd, char *file)
{
	if (cmd->outfile != -1)
		close(cmd->outfile);
	cmd->outfile = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (cmd->outfile == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
		cmd->redir_error = 1;
		return (0);
	}
	return (1);
}

void	handle_redir_out(t_cmd *cmd, t_token **tok)
{
	char	*filename;

	if (!(*tok) || !(*tok)->next)
		return ;
	*tok = (*tok)->next;
	if ((*tok)->ambiguous)
	{
		report_ambiguous_redirect(*tok);
		cmd->redir_error = 1;
		*tok = (*tok)->next;
		return ;
	}
	filename = ft_strdup((*tok)->value);
	if (!filename)
	{
		cmd->redir_error = 1;
		return ;
	}
	open_outfile(cmd, filename);
	free(filename);
	*tok = (*tok)->next;
}

int	open_append_fd(t_cmd *cmd, const char *filename)
{
	if (cmd->outfile != -1)
		close(cmd->outfile);
	cmd->append = 1;
	cmd->outfile = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (cmd->outfile == -1)
	{
		errno_msg(filename);
		cmd->redir_error = 1;
		return (0);
	}
	return (1);
}
