/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mimetypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilefhail <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 15:30:08 by ilefhail          #+#    #+#             */
/*   Updated: 2023/03/09 15:30:09 by ilefhail         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#ifndef MIMETYPES_H_
#define MIMETYPES_H_
#include <iostream>
#include <map>

class MimeTypes {
    private:
        std::map<std::string, std::string> _ex_type;
    public:
        MimeTypes();
        std::string get_type(std::string path);
        std::string get_extention(std::string type);
};

#endif