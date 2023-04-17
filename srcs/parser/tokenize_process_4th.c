#include "minishell.h"

static int	open_heredoc_fd(char *tmp, t_redir *new)
{
	int		i;
	int		fd;
	char	*tmp2;

	i = -1;
	while (access(tmp, F_OK) == 0)
	{
		tmp2 = ft_strdup_s(tmp);
		free(tmp);
		tmp = ft_strjoin(tmp2, "1");
		free(tmp2);
	}
	fd = open(tmp, O_RDWR | O_CREAT, 0644);
	if (fd == -1)
		ft_open_error();
	while (tmp[++i])
		new->file_name[i] = tmp[i];
	free(tmp);
	return (fd);
}

static int	ft_here_doc_pretreatment(t_p_data *pdata, \
t_redir *new, t_word *word)
{
	char	*str;
	char	*tmp;
	char	temp;

	word->re_idx += 1;
	temp = word->word[word->re_idx];
	if (temp == '>' || temp == '<' || temp == '(' || temp == ')'
		|| temp == '`' || temp == '&' || temp == '\0')
	{
		ft_syntax_error(pdata, word);
		return (1);
	}
	str = ft_itoa(++(word->tmp_num));
	if (!str)
		ft_allocation_error();
	tmp = ft_strjoin_s("/tmp/.tmp", str);
	free(str);
	new->heredoc_fd = open_heredoc_fd(tmp, new);
	return (0);
}

static void	ft_here_doc_act(t_redir *new, char *tmp, t_env_node *node)
{
	char		*str;
	t_env_node	*temp;

	rl_catch_signals = 1;
	temp = node;
	while (1)
	{
		str = readline("> ");
		if (!str || ft_strcmp(tmp, str) == 0)
		{
			free(str);
			break ;
		}
		if (str[0] == '$')
		{
			while (node != NULL)
			{
				if (ft_strcmp(str + 1, node->key) == 0)
				{
					str = node->value;
					break ;
				}
				node = node->next;
			}
			if (node == NULL)
			{
				str = ft_strdup("");
			}
			node = temp;
		}
		write(new->heredoc_fd, str, ft_strlen(str));
		write(new->heredoc_fd, "\n", 1);
		free(str);
	}
}

int	ft_redirect_here_doc(t_p_data *pdata, t_redir *new, \
	t_word *word, t_env_node *node)
{
	char	*tmp;

	if (ft_here_doc_pretreatment(pdata, new, word) == 1)
		return (1);
	tmp = ft_strdup_s(word->word + word->re_idx);
	word->re_idx -= 2;
	while (word->word[word->re_idx])
		word->word[(word->re_idx)++] = '\0';
	ft_here_doc_act(new, tmp, node);
	close(new->heredoc_fd);
	new->heredoc_fd = open(new->file_name, O_RDONLY);
	if (new->heredoc_fd == -1)
		ft_open_error();
	unlink(new->file_name);
	free(tmp);
	return (0);
}
