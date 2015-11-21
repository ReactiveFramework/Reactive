/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   query2.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gwells <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/11 12:40:21 by gwells            #+#    #+#             */
/*   Updated: 2015/11/11 17:47:49 by gwells           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "query2.hpp"
#include <stdlib.h>
#include <stdio.h>

void            query::printlist(t_list *lst)
{
    t_content   *content;

    while(lst)
    {
        content = (t_content*)lst->content;
        printf("Voici le path %s\n", content->path);
        printf("Voici la value %s\n",content->value);
        lst = lst->next;
    }
}

t_list *query::parse(const char *query)
{
    t_list          *begin;
    t_list          *node; 
    char            *path; 
    char            *value;
    const char      *error;
    bool            flag;
    bool            count;
    t_content       content;


    error = "Bad syntax";
    count = false;
    begin = NULL; 
    path = ft_strdup("");
    value = ft_strdup("");

    while (*query)
    { 
        flag = false;
        if (*query != '[' && *query != '=')
            path = changeString(path, ft_append_char(path, *query));
        if (*query == '[')
        {
            if (*(query + 1) == ']')
            {
                path = changeString(path, ft_strjoin(path, ":index"));
                query+=2;
                if (*query != '=')
                    ft_quit(error, 2, EXIT_FAILURE);
            }
            else
            {
                if (*(query + 1) != '\'' && *(query + 1) != '\"')
                    ft_quit(error, 2, EXIT_FAILURE);
                else
                {
                    query+=2;
                    path = changeString(path, ft_append_char(path, ':'));
                    while (*query && *query != ']')
                    {
                        path = changeString(path, ft_append_char(path, *query));
                        query++;
                    }
                    if (!*query || (*(query - 1) != '\'' && *(query - 1) != '\"')
                            || (*(query + 1) != '=' && *(query + 1) != '[' && *(query + 1)))
                        ft_quit(error, 2, EXIT_FAILURE);
                    path[ft_strlen(path) - 1] = '\0';
                    path = changeString(path, ft_strdup(path));
                } 
            }
            flag = true;
        }
        if (*query == '=')
        {
            query++;
            while(*query  && *query != '&')
            {
                value = changeString(value, ft_append_char(value, *query));
                query++;
            }
            content.path = ft_strdup(path);
            content.value = ft_strdup(value);
            node = ft_lstnew(&content, sizeof(content));
            ft_lstpushback(&begin, node);
            ft_strdel(&path);
            ft_strdel(&value);
            path = ft_strdup("");
            value = ft_strdup("");
        }
        if (*query && flag)
            query++;
        if (*query && !flag)
            query++;
    } 
    printlist(begin);
    ft_strdel(&path);
    ft_strdel(&value);
    return (begin);
}

char        *query::changeString(char *old, char *ret)
{
    ft_strdel(&old);
    return(ret);
}
