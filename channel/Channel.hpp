// Channel.hpp file
#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include "../Client/Client.hpp"

#define CH_CLIENT_LIMIT 1		//+l
#define CH_INVITE_ONLY 2		//+i
#define CH_KEY 4				//+k
#define CH_PROTECTED_TOPIC 8	//+t
#define	CH_OPERATOR_PRIV 16		//+o

class	Channel
{
	public:
		Channel(std::string = "", std::string = "");
		~Channel();

		//getters
		std::string	get_name() const;
		std::string	get_passwd() const;
		std::string	get_topic() const;
		size_t		get_nbr_members() const;
		size_t		get_max_members() const;
		bool		is_mode_enabled(mode_t) const;
		std::string get_mode();
		
		//setters
		void	set_name(std::string &);
		void	set_passwd(std::string);
		void	set_topic(std::string &);
		void	set_nbr_members(size_t);
		void	set_max_members(size_t);

		// mode controle
		void	enable_mode(mode_t mode);
		void	disable_mode(mode_t mode);

		// members methode
		void	add_member(Client &, std::string);
		void	add_operator(Client &);
		void	remove_member(Client &);
		void	remove_member_from_invited_list(Client &);
		void	promote_member(Client &);
		void	promote_member(std::string &);
		void	unpromote_member(Client &);
		void	unpromote_member(std::string &);
		void	invite_member(Client &);
		int		is_member(Client &); // -1 : not a memeber , 0 : simple user , 1 : operator
		int		is_member(std::string &); // -1 : not a memeber , 0 : simple user , 1 : operator
		const Client	&get_member_by_name(std::string &);
		std::map <Client, int> get_members() const;
		void	broadcast(const std::string &, std::string);
		std::string	get_list_of_names();

		bool				operator==(const Channel& cl) const;

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
