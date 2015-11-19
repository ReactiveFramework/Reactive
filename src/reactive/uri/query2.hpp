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

        void    printlist(t_list *lst);

    public:
        /**
         * Parse the content an put it in linked list
         */

        query() {}
        t_dlist  *parse(const char *query);
        void    printlist(t_dlist*);

};
