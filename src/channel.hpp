# ifndef CHANNEL_HPP
# define CHANNEL_HPP

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
		bool	operator==(const channel& cl) const;
		// bool	operator!=(const channel& cl) const;
		// bool	operator<(const channel& cl) const;
		// bool	operator<=(const channel& cl) const;
		// bool	operator>=(const channel& cl) const;
		// bool	operator>(const channel& cl) const;
		
};

#endif