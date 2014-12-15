#include "Score.h"

GLuint  Score::currentScore = 0;

void Score::setScore(GLuint  newScore) {
	Score::currentScore = newScore;
}

void Score::addScore(GLuint  addScore){
	Score::currentScore += addScore;
}

int Score::getScore() {
	return Score::currentScore;
}

