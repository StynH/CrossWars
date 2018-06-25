#include "ShowChapter.h"

ShowChapter::ShowChapter(std::string _ChapterName)
{
	ChapterName = _ChapterName;
}


ShowChapter::~ShowChapter()
{
}

void ShowChapter::Run()
{

	ScreenManager::GetInstance()->GetChapterScreen()->SetChapterName(ChapterName);
	ScreenManager::GetInstance()->LoadScreen(ScreenManager::GetInstance()->GetChapterScreen());
}
