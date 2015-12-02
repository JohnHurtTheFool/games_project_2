#include "highscore.h"

highscore::highscore()
{
	file_name = "";
	for(int i = 0;i<MAX_HIGH_SCORE+1;i++)
	{
		scores[i] = entry("",-1);
	}
}

string highscore::getHighScoreString()
{
	for(int i = 0;i<MAX_HIGH_SCORE+1;i++)
	{
		scores[i] = entry("",-1);
	}
	string highScoreText;
	ifstream myfile(file_name);
	string tempName;
	int tempScore;
	bool hsExists = false;
	int i = 0;
	if(myfile.is_open())
	{
		while(myfile >> tempName >> tempScore)
		{
			hsExists = true;
			scores[i] = entry(tempName,tempScore);
			i++;
		}
	}
	myfile.close();
	highScoreText += "HIGHSCORES\n";
	for(int i=0;i<MAX_HIGH_SCORE;i++)
	{
		if(scores[i].score>0)highScoreText += "#" + std::to_string(i+1) + " " + scores[i].name + " Score: " + std::to_string(scores[i].score) + "\n";
	}
	return highScoreText;
}


highscore::highscore(string f_name)
{
	file_name = f_name;
	for(int i = 0;i<MAX_HIGH_SCORE+1;i++)
	{
		scores[i] = entry("",-1);
	}
}

void highscore::insertHighScore(string name, int score)
{
	for(int i = 0;i<MAX_HIGH_SCORE+1;i++)
	{
		scores[i] = entry("",-1);
	}
	ifstream myfile(file_name);
	string tempName;
	int tempScore;
	bool hsExists = false;
	int i = 0;
	if(myfile.is_open())
	{
		while(myfile >> tempName >> tempScore)
		{
			hsExists = true;
			scores[i] = entry(tempName,tempScore);
			i++;
		}
		if(hsExists)
		{
			scores[i] = entry(name,score);
		}
	}
	myfile.close();
	sortScores();
	ofstream ofs(file_name);
	if(ofs.is_open())
	{
		if(!hsExists)
		{
			ofs << name << " " << score;
			return;
		}
		for(int j = 0;j<MAX_HIGH_SCORE;j++)
		{
			if(scores[j].score>0)ofs << scores[j].name << " " << scores[j].score << "\n";
		}
	}
	ofs.close();
}