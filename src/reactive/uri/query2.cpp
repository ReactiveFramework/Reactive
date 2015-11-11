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

t_list *query::parse(const char *query)
{
    t_list          *begin;
    char            *path; 
    const char      *error;
    bool            flag;


    if (!ft_strlen(query))
        ft_quit("La chaine est vide", 2, EXIT_FAILURE);

    error = "Bad syntax";
    path = ft_strdup("");
    
    while (*query)
    { 
        flag = false;
        if (*query != '[')
            path = ft_append_char(path, *query);
        if (*query == '[')
        {
            if (*(query + 1) == ']')
                ft_strjoin(path, ":index");
            else
            {
                if (*(query + 1) != '\'' && *(query + 1) != '\"')
                    ft_quit(error, 2, EXIT_FAILURE);
                else
                {
                    query+=2;
                    path = ft_append_char(path, ':');
                    while (*query && *query != ']')
                    {
                        path = ft_append_char(path, *query);
                        query++;
                    }
                    if (!*query || (*(query - 1) != '\'' && *(query - 1) != '\"'))
                        ft_quit(error, 2, EXIT_FAILURE);
                    path[ft_strlen(path) - 1] = '\0';
                } 
            }
            flag = true;
        }
        if (*query && flag)
            query++;
        if (*query && !flag)
            query++;
    } 

    ft_putendl(path);
    return (begin);
}


