/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   query2.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gwells <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/11 12:41:45 by gwells            #+#    #+#             */
/*   Updated: 2015/11/11 15:39:51 by gwells           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

class query
{
    private:
        typedef struct  s_content
        {
            char        *path;
            char        *value;
        }               t_content;

        /**
         * Debug function
         */
        void    printlist(t_list* lst);
        char    *changeString(char* old, char *ret);

    public:

        query() {}
        /**
         * Parse the content and put it in linked list
         */
        t_list  *parse(const char *query);

};
