#pragma once

#include <iostream>
#include <vector>
#include "client.hpp"


class channel
{
	private:
		std::vector<client>			clients;

		std::string					channelName;

		std::string					password;
		client						moderator;
		int							size;

	public:
		bool						password_set;
		bool						invite_only;

		channel();
		~channel();

		void	addClient(client cl);
		void	removeClient(client cl);

		void	setMod(client moderator);
		void	setSize(int size);

		client	getMod();
		int		getSize();

		channel operator = (const channel& other);
		bool	operator==(const channel& other) const;
		
};
