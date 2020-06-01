#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <dirent.h>
#include <sys/types.h>

void updatePlaylist(const char *path, std::vector<std::string> &words)
{
	words.clear();
	dirent* entry;
    DIR* dir = opendir(path);
    if(dir == NULL)	return;
    while((entry = readdir(dir)) != NULL)
    {
    	std::string tmp = entry->d_name;
    	for(int i = 0; i < tmp.length(); i++)
    		if(tmp[i] == '.')
    			if(tmp[i+1] == 'm' && tmp[i+2] == 'p' && tmp[i+3] == '3')
    			{
    				std::string word = "'" + tmp + "'";
    				words.push_back(word);
    			}	
    }
   	closedir(dir);
}

void setRatings(std::vector<std::string> &words, std::vector<float> &ratings, std::vector<std::string> &queryVect, std::vector<std::vector<std::string>> &wordsVect, std::string query) // from 0 - 1
{
	/*for(int i = 0; i < words.size(); ++i) // gre cez usa imena pesmi
	{
		int matchNow = 0, numLettersSource = words.at(i).length();
		float totalMatch = 0, rating = 0.0;
		if (query == words.at(i))	ratings.push_back(1.0);
		else
		{
			for(int j = 0; j < queryVect.size(); ++j)
			{
			    for(int k = 0; k < (words.at(i)).size() && k < (queryVect.at(j)).size(); ++k)
                {
                    if((queryVect.at(j))[k] == (words.at(i))[k]) ++totalMatch;
                    else if(((queryVect.at(j))[k] >= 'A' && (queryVect.at(j))[k] <= 'Z') && ((queryVect.at(j))[k] + 32 == (words.at(i))[k])) totalMatch += 0.5;
                    else if(((queryVect.at(j))[k] >= 'a' && (queryVect.at(j))[k] <= 'z') && ((queryVect.at(j))[k] - 32 == (words.at(i))[k])) totalMatch += 0.5;
                }
			}
			rating = totalMatch / numLettersSource;
			ratings.push_back(rating);
		}
	}*/

	

	for(int i = 0; i < words.size(); i++)
	{
		int totalCompared = 0;
		float totalMatch = 0.0, rating = 0.0;
		if(words.at(i) == query) ratings.push_back(100000); // because ×D
		else
		{
			for(int j = 0; j < wordsVect[i].size(); j++) // gre čez eno vrsto besed
			{
				for(int k = 0; k < queryVect.size(); k++) // gre čez besedo v vrsti
				{
					for(int l = 0; l < wordsVect[i][j].size() && l < queryVect[k].size(); l++)
					{
						if(wordsVect[i][j][l] == queryVect[k][l]) totalMatch++;
						else if(((queryVect[k][l] >= 'A') && (queryVect[k][l] <= 'Z')) && (queryVect[k][l] + 32 == wordsVect[i][j][l])) totalMatch += 0.5;
                    	else if(((queryVect[k][l] >= 'a') && (queryVect[k][l] <= 'z')) && (queryVect[k][l] - 32 == wordsVect[i][j][l])) totalMatch += 0.5;
                    	totalCompared++;
					}
				}
			}
			rating = totalMatch;// / words[i].size();
			ratings.push_back(rating);
		}
	}
}

void printPlaylist(std::vector<std::string> &words)
{
    for(int i = 0; i < words.size(); ++i)
        std::cout << '[' << i+1 << "] " << words.at(i) << '\n';
}

void quikSortPlaylist(std::vector<std::string> &words)
{
	
}

void printRatings(std::vector<float> &ratings)
{
	for(int i = 0; i < ratings.size(); i++)
		std::cout << "Rating " << i << ": " << ratings.at(i) << '\n';
}

int main()
{
	//system("gnome-terminal");
	srand(time(NULL));
	std::vector<std::string> words;
	std::vector<std::vector<std::string>> wordsVect;
	std::vector<std::string> tempVect;
	std::vector<float> ratings;
	std::vector<std::string> queryVect;
	char inp;
	int playIndex = 0;
	updatePlaylist("/home/aljaztravnik/Desktop/travnikmp3", words);
	for(int i = 0; i < words.size(); i++)
	{
		std::string temp; 
		std::stringstream ss(words.at(i));
		while(ss >> temp) tempVect.push_back(temp);
		wordsVect.push_back(tempVect);
		tempVect.clear();
	}
	do
	{
		std::cout << "ENTER COMMAND:\n[p] - play\n[s] - stop\n[t] - search by name of song\n[>] - next song\n[<] - previous song\n[o] - print list of songs\n[e] - exit\n";
		std::cin >> inp;
		std::cin.ignore();
		if(inp == 'p')
		{
			system("^C");
			std::string playString = "ffplay -nodisp -autoexit " + words.at(playIndex);
			const char* playCommand = playString.c_str();
			system(playCommand);
		}
		else if(inp == 's') system("^C");
		else if(inp == 't')
		{
			std::string input, temp, query;
			getline(std::cin, input);
			query = "'" + input + "'";
			std::stringstream ss(query);
			while(ss >> temp)   queryVect.push_back(temp); // razcleni query
			setRatings(words, ratings, queryVect, wordsVect, query);
			printRatings(ratings);
			playIndex = std::distance(ratings.begin(), std::max_element(ratings.begin(), ratings.end()));
			std::string playString = "ffplay -nodisp -autoexit " + words.at(playIndex);
			const char* playCommand = playString.c_str();
			system(playCommand);
			ratings.clear();
			queryVect.clear();
		}
		else if(inp == '>')
		{
			if(playIndex+1 < words.size())	playIndex++;
			system("^C");
			std::string playString = "ffplay -nodisp -autoexit " + words.at(playIndex);
			const char* playCommand = playString.c_str();
			system(playCommand);
		}
		else if(inp == '<')
		{
			if(playIndex > 0)	playIndex--;
			system("^C");
			std::string playString = "ffplay -nodisp -autoexit " + words.at(playIndex);
			const char* playCommand = playString.c_str();
			system(playCommand);
		}
		else if(inp == 'o')	printPlaylist(words);
		updatePlaylist("/home/aljaztravnik/Desktop/travnikmp3", words);
		for(int i = 0; i < words.size(); i++)
		{
			std::string temp; 
			std::stringstream ss(words.at(i));
			while(ss >> temp) tempVect.push_back(temp);
			wordsVect.push_back(tempVect);
			tempVect.clear();
		}
		
	}while(inp != 'e');
	return 0;
}