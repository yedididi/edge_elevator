#include "../incs/main.h"

int	wd_cnt(char const *s, char c)
{
	int	wd;

	wd = 0;
	while (*s)
	{
		while ((*s == c) && *s)
			s++;
		if (*s)
			wd++;
		while ((*s != c) && *s)
			s++;
	}
	return (wd);
}

int	free_single(char **fin, char *one)
{
	if (one == 0)
	{
		while (*fin)
		{
			free(*fin);
			fin++;
		}
		free(fin);
		return (0);
	}
	return (1);
}

char	*ft_splstrdup(char **final_str, char const *s, char c)
{
	char	*return_str;
	int		len;

	len = 0;
	while ((s[len] != c) && s[len])
		len++;
	return_str = (char *)malloc(sizeof(char) * (len + 1));
	if (free_single(final_str, return_str) == 0)
		return (0);
	len = 0;
	while ((*s != c) && *s)
		return_str[len++] = *s++;
	return_str[len] = '\0';
	return (return_str);
}

char	**ft_split(char const *s, char c)
{
	char	**final_str;
	int		how_many_wd;
	int		i;

	i = 0;
	how_many_wd = wd_cnt(s, c) + 1;
	final_str = (char **)malloc(sizeof(char *) * how_many_wd);
	if (final_str == 0)
		return (0);
	while (*s)
	{
		while ((*s == c) && *s)
			s++;
		if (*s)
		{
			final_str[i] = ft_splstrdup(final_str, s, c);
			if (final_str[i] == 0)
				return (0);
			i++;
		}
		while ((*s != c) && *s)
			s++;
	}
	final_str[i] = 0;
	return (final_str);
}