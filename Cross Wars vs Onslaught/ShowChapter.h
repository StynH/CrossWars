#pragma once
#include "GameState.h"

class ShowChapter : public GameState
{
public:
	ShowChapter(std::string _ChapterName);
	~ShowChapter();

	void Run() override;

private:
	std::string ChapterName;
};

