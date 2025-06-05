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
	for (size_t i = 0; i < input[1].size(); i++)
	{
		std::string chan_name;
		for (; input[1][i] && input[1][i] != ','; i++)
			chan_name.push_back(input[1][i]);
		str_names.push_back(chan_name);
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
			str_keys.push_back(key);
		}
	}

	if (str_names.size() > str_keys.size())
	{
		size_t deff = str_names.size() - str_keys.size();
		size_t i = 0;
		for(; i < str_keys.size(); i++){}
		size_t j = 0;
		for (; j < deff; j++)
		{
			str_keys.push_back("NO_PASS");
		}
	}

	std::vector<std::string>::iterator it_name;
	std::vector<std::string>::iterator it_key;

	it_name = str_names.begin();
	it_key = str_keys.begin();
	std::vector<Channel>::iterator it = Channels.begin();
	for (; it_name != str_names.end(); it_name++, it_key++)
	{
		if (it_name->size() == 0)
		{
			sendReply(client.getClientSocketfd(), ERR_BADCHANMASK(*it_name));
			continue ;
		}
		if (it_name->size() == 1 && *it_name->begin() == '#')
		{
			sendReply(client.getClientSocketfd(), ERR_BADCHANMASK(*it_name));
			continue ;
		}
		if (it_name->size() != 0 && *it_name->begin() != '#')
		{
			sendReply(client.getClientSocketfd(), ERR_BADCHANMASK(*it_name));
			continue ;
		}
		else
		{
			std::string name = it_name->substr(1, it_name->size());
			it = Channels.begin();
			for (; it != Channels.end(); it++)
			{	
				if (it->getName() == name)
				{
					break ;
				}
			}
			if (it != Channels.end())
			{
				if (it->inChannel(client))
				{
					sendReply(client.getClientSocketfd(), ERR_USERONCHANNEL(client.getName(), it->getName()));
					return ;
				}
				if (it->get_pass_flag())
				{
					if (*it_key != "NO_PASS")
					{
						if (it->getPass() != *it_key)
						{
							sendReply(client.getClientSocketfd(), ERR_PASSWDMISMATCH(client.getName()));
							return ;
						}
						else if (it->getPass() == *it_key)
						{
							sendReply(client.getClientSocketfd(), ERR_USERONCHANNEL(client.getName(), it->getName()));
							return ;
						}
					}
				}
				
				if (it->get_UserLimitFlag())
				{
					if (it->getUserLimit() <= (int)it->members.size())
					{
						sendReply(client.getClientSocketfd(), ERR_CHANNELISFULL(client.getName(), name));
						return ;
					}
				}
				
				if (it->getInviteOnly())
				{
					sendReply(client.getClientSocketfd(), ERR_INVITEONLYCHAN(client.getName(), name));
					return ;
				}
			}
			else
			{
				Channel next;
				next.setName(name);
				if (*it_key != "NO_PASS")
				{
					next.setPass(*it_key);
					next.set_pass_flag(true);
				}
				else
				{
					next.set_pass_flag(false);
				}
				next.members.push_back(client);
				next.admines.push_back(client);
				Channels.push_back(next);
			}
		}
	}	
}