/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obahi <obahi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 20:51:16 by obahi             #+#    #+#             */
/*   Updated: 2024/01/08 17:48:33 by obahi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Client.hpp"
#include <iostream>
#include <vector>

#define CH_CLIENT_LIMIT 1
#define CH_INVITE_ONLY 2
#define CH_KEY 4
#define CH_PROTECTED_TOPIC 8

class	Channel
{
	public:
		Channel(std::string = "", std::string = "", std::string = "", mode_t = 0);
		Channel(const Channel&);
		Channel& operator=(const Channel&);
		~Channel();

		//getters
		std::string	get_name() const;
		std::string	get_passwd() const;
		std::string	get_topic() const;
		size_t		get_nbr_memebers() const;
		
		//setters
		void	set_name(std::string &);
		void	set_passwd(std::string &);
		void	set_topic(std::string &);
		void	set_nbr_memebers(size_t);

		// mode control
		bool	client_limit_mode() const;
		bool	invite_only_mode() const;
		bool	key_mode() const;
		bool	protected_topic_mode() const;

		void	enable_mode(mode_t mode);
		void	disable_mode(mode_t mode);

		bool	is_member(std::string) const;
		bool	is_operator(std::string) const;
		bool	is_baned(std::string) const;
		bool	is_invited(std::string) const;

		
		void	add_member(std::string);
		void	remove_memeber(std::string);
		void	ban_memeber(std::string);
		void	unban_memeber(std::string);
		void	exempt_ban_memeber(std::string);
		void	unexempt_ban_memeber(std::string);
		void	exempt_invite_memeber(std::string);
		void	unexempt_invite_memeber(std::string);
		
	private :
		std::string					_name;
		std::string					_passwd;
		std::string					_topic;
		size_t						_nbr_members;
		std::vector<Client>			_members;
		std::vector<Client>			_operators;

		mode_t										_mode;
};

template <typename T>
typename std::vector<T>::iterator find(std::vector<T> v, const T& e)
{
	std::vector<T>::iterator itr = v.begin();
	while (itr != v.end())
	{
		if (*itr == e)
			break;
		itr++;
	}
	return (itr);
}
