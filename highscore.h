              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#ifndef HIGH_SCORE_H                 // Prevent multiple definitions if this 
#define HIGH_SCORE_H 

#include <fstream>
#include <iostream>
#include "constants.h"
#include <string>
using namespace std;

class highscore
{
public:
	highscore();
	highscore(string f_name);
	void insertHighScore(string name, int score);
	void changeFileName(string f_name){file_name = f_name;};
	struct entry
	{
		entry(){};
		entry(string n,int s)
		{
			name = n;
			score = s;
		}
		string name;
		int score;
	};
	entry getEntry(int index){return scores[index];};
	void sortScores()
	{
		entry temp; //for swapping
		for (int i = 0 ; i < MAX_HIGH_SCORE ; i++)
		{
			for (int j = 0 ; j < MAX_HIGH_SCORE ; j++)
			{
				if ( scores[j].score < scores[j + 1].score )
				{
					temp = scores[j];
					scores[j]=scores[j + 1];
					scores[j + 1] = temp;
				}
			}
		}
	}
	string getHighScoreString();
private:
	string file_name;
	entry scores[MAX_HIGH_SCORE+1];
};

#endif