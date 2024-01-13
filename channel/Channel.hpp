/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obahi <obahi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 20:51:16 by obahi             #+#    #+#             */
/*   Updated: 2024/01/13 10:22:16 by obahi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Client.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

#define CH_CLIENT_LIMIT 1
#define CH_INVITE_ONLY 2
#define CH_KEY 4
#define CH_PROTECTED_TOPIC 8

class	Channel
{
	public:
		Channel(std::string = "", std::string = "", std::string = "", mode_t = 0, size_t = 0);
		~Channel();

		//getters
		std::string	get_name() const;
		std::string	get_passwd() const;
		std::string	get_topic() const;
		size_t		get_nbr_members() const;
		size_t		get_max_members() const;
		
		//setters
		void	set_name(std::string &);
		void	set_passwd(std::string &);
		void	set_topic(std::string &);
		void	set_nbr_members(size_t);
		void	set_max_members(size_t);

		// mode control
		bool	client_limit_mode() const;
		bool	invite_only_mode() const;
		bool	key_mode() const;
		bool	protected_topic_mode() const;

		void	enable_mode(mode_t mode);
		void	disable_mode(mode_t mode);
		// members methode
		void	add_member(Client &, std::string);
		void	add_operator(Client &);
		void	remove_member(Client &);
		void	remove_member(std::string &);
		void	promote_member(Client &);
		void	unpromote_member(Client &);
		void	invite_member(Client &);

		int	is_member(Client &) const; // -1 : not a memeber , 0 : simple user , 1 : operator
		int	is_member(std::string &) const; // -1 : not a memeber , 0 : simple user , 1 : operator
	private :
		std::string					_name;
		std::string					_passwd;
		std::string					_topic;
		size_t						_nbr_members;
		size_t						_max_members;
		std::map <Client, int>		_members; // 0 : user 1 : operstor
		std::vector <Client>		_invited;

		mode_t						_mode;
};
