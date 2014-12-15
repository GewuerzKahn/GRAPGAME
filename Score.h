#pragma once

#include "Globals.h"


class Score {

public:

	Score();
	~Score();

	static void setScore(GLuint  newScore);
	static void addScore(GLuint  addScore);
	static int getScore();

private:
	static GLuint  currentScore;
};

