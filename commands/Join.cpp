#include "../Server.hpp"

void Server::Join(Client client, std::vector<std::string> input)
{
	if (input.size() == 1)
	{
		sendReply(client.getClientSocketfd(), ERR_NEEDMOREPARAMS(input[0]));
		return ;
	}
	std::vector<std::string> str_names;
	std::vector<std::string> str_keys;
	for (size_t i = 0; input[1][i] != '\0'; i++)
	{
		std::string chan_name;
		if (input[1][i] == '#')
		{
			for (; input[1][i] && input[1][i] != ','; i++)
				chan_name.push_back(input[1][i]);
		}
		if (chan_name.size() != 0)
		{
			if (chan_name[0] == '#' && chan_name.size() != 1)
				str_names.insert(str_keys.begin() + 0, chan_name);
			else
				sendReply(client.getClientSocketfd(), "cannot add the channel");
		}
	}
	if (input.size() != 2)
	{
		std::string buffer;
		for (size_t i = 0; input[2][i]; i++)
			buffer.push_back(input[2][i]);
		
		for (size_t i = 0; buffer[i]; i++)
		{
			std::string key;
			for (; buffer[i] && buffer[i] != ','; i++)
				key.push_back(buffer[i]);
			str_keys.insert(str_keys.begin() + 0, key);
		}
	}
	std::vector<std::string>::iterator it_name;
	std::vector<std::string>::iterator it_key;
	it_name = str_names.begin();
	it_key = str_keys.begin();
	std::vector<Channel>::iterator it = Channels.begin();
	(void)it;
	for (; it_name != str_names.end(); it_name++)
	{
		it = Channels.begin();
		for (; it != Channels.end(); it++)
		{
			if (it->getName() == *it_name)
			{
				break ;
			}
		}
		if (it != Channels.end())
		{
			std::vector<Client>::iterator it = Channels.begin()->admines.begin();
			for (; it != Channels.begin()->admines.end(); it++)
			{
				if (client.getName() == it->getName())
					break ;
			}
			if (it != Channels.begin()->admines.end())
			{
				str_names.pop_back();
				if (it->gethasPass())
					it->setPass(str_keys.back());
				str_keys.pop_back();
				it->
			}
		}
		else
		{
		}
	}
}