#include "MusicPlayer.h"
#include "SFML/Audio.hpp"
#include "Folders.h"

#include "boost/filesystem.hpp"
#include "EventLogger.h"
#include "GameTime.h"
using namespace boost::filesystem;

SoundPlayer::SoundPlayer()
{
	SoundBuffer = new sf::SoundBuffer();
	CurrentSound = new sf::Sound();

	CursorSound = new sf::SoundBuffer();
	CursorSound->loadFromFile(Folders::ROOT_FOLDER + Folders::SOUNDS_FOLDER + "Cursor.wav");
	CancelSound = new sf::SoundBuffer();
	CancelSound->loadFromFile(Folders::ROOT_FOLDER + Folders::SOUNDS_FOLDER + "Cancel.wav");
	ErrorSound = new sf::SoundBuffer();
	ErrorSound->loadFromFile(Folders::ROOT_FOLDER + Folders::SOUNDS_FOLDER + "Error.wav");
	RecieveSound = new sf::SoundBuffer();
	RecieveSound->loadFromFile(Folders::ROOT_FOLDER + Folders::SOUNDS_FOLDER + "Recieve.wav");
	SelectSound = new sf::SoundBuffer();
	SelectSound->loadFromFile(Folders::ROOT_FOLDER + Folders::SOUNDS_FOLDER + "Select.wav");
	TalkSound = new sf::SoundBuffer();
	TalkSound->loadFromFile(Folders::ROOT_FOLDER + Folders::SOUNDS_FOLDER + "Talk.wav");
	ItemUseSound = new sf::SoundBuffer();
	ItemUseSound->loadFromFile(Folders::ROOT_FOLDER + Folders::SOUNDS_FOLDER + "ItemUse.wav");
	TurnTransition = new sf::SoundBuffer();
	TurnTransition->loadFromFile(Folders::ROOT_FOLDER + Folders::SOUNDS_FOLDER + "TurnTransition.wav");
	BattleTransition = new sf::SoundBuffer();
	BattleTransition->loadFromFile(Folders::ROOT_FOLDER + Folders::SOUNDS_FOLDER + "BattleTransition.wav");
	HitSound = new sf::SoundBuffer();
	HitSound->loadFromFile(Folders::ROOT_FOLDER + Folders::SOUNDS_FOLDER + "Hit.wav");
	CritSound = new sf::SoundBuffer();
	CritSound->loadFromFile(Folders::ROOT_FOLDER + Folders::SOUNDS_FOLDER + "Critical.wav");

	PlayingMusic = nullptr;
	QueuedMusic = nullptr;

	TransitioningTo = false;
	TransitioningFrom = false;
	FadingOut = false;

	TransitioningSpeed = 1;
}

SoundPlayer::~SoundPlayer()
{
}

void SoundPlayer::Update()
{
	if(TransitioningTo)
	{
		Volume -= GameTime::DeltaTime * TransitioningSpeed * 100;

		if(Volume <= 0)
		{
			Volume = 0;
			PlayingMusic->setVolume(Volume);
			PlayingMusic = QueuedMusic;
			TransitioningTo = false;
			TransitioningFrom = true;

			PlayingMusic->play();
		}
		else
		{
			if (PlayingMusic)
			{
				PlayingMusic->setVolume(Volume);
			}
		}
	}
	else if(TransitioningFrom)
	{
		Volume += GameTime::DeltaTime * TransitioningSpeed * 100;

		if (Volume >= 100)
		{
			Volume = 100;
			PlayingMusic->setVolume(Volume);
			TransitioningFrom = false;
		}
		else
		{
			if(PlayingMusic)
			{
				PlayingMusic->setVolume(Volume);
			}
		}
	}
	else if(FadingOut)
	{
		Volume -= GameTime::DeltaTime * FadeOutSpeed * 100;
		if (Volume <= 0)
		{
			Volume = 100;
			FadingOutMusic->setVolume(0);
			FadingOutMusic = nullptr;
			PlayingMusic = nullptr;
			FadingOut = false;
		}
		else
		{
			if(PlayingMusic)
			{
				PlayingMusic->setVolume(Volume);
			}
		}
	}
}

void SoundPlayer::Transition(sf::Music* _pMusic, int _TransitioningSpeed)
{
	if(PlayingMusic)
	{
		QueuedMusic = _pMusic;
		Volume = 100.0F;
		TransitioningTo = true;
		TransitioningSpeed = _TransitioningSpeed;
	}
	else
	{
		PlaySpecificMusic(_pMusic);
	}
}

void SoundPlayer::Transition(std::string _File, int _TransitioningSpeed, bool _MustLoop)
{
	std::shared_ptr<sf::Music> pMusic = std::make_shared<sf::Music>();
	if (!pMusic->openFromFile(Folders::ROOT_FOLDER + Folders::MUSIC_FOLDER + _File))
	{
		EventContext ec("Cannot find music file: ", _File.c_str());
		return;
	}
	pMusic->setLoop(_MustLoop);

	if (PlayingMusic)
	{
		QueuedMusic = pMusic.get();
		Volume = 100.0F;
		TransitioningTo = true;
		TransitioningSpeed = _TransitioningSpeed;
	}
	else
	{
		PlaySpecificMusic(pMusic.get());
	}
}

void SoundPlayer::PlayMusic(std::string _File, bool _MustLoop)
{
	OtherMusic = std::make_shared<sf::Music>();
	if (!OtherMusic->openFromFile(Folders::ROOT_FOLDER + Folders::MUSIC_FOLDER + _File))
	{
		EventContext ec("Cannot find music file: ", _File.c_str());
		return;
	}
	OtherMusic->play();
	OtherMusic->setLoop(_MustLoop);
}

void SoundPlayer::SetLevelMusic(std::string _File, bool _MustLoop)
{
	LevelMusic = std::make_shared<sf::Music>();
	if (!LevelMusic->openFromFile(Folders::ROOT_FOLDER + Folders::MUSIC_FOLDER + _File))
	{
		EventContext ec("Cannot find level music file: ", _File.c_str());
		return;
	}
	LevelMusic->setLoop(_MustLoop);
}

void SoundPlayer::SetBattleMusic(std::string _File, bool _MustLoop)
{
	BattleMusic = std::make_shared<sf::Music>();
	if (!BattleMusic->openFromFile(Folders::ROOT_FOLDER + Folders::MUSIC_FOLDER + _File))
	{
		EventContext ec("Cannot find battle music file: ", _File.c_str());
		return;
	}
	BattleMusic->setLoop(_MustLoop);
}

void SoundPlayer::SetBossMusic(std::string _File, bool _MustLoop)
{
	BossMusic = std::make_shared<sf::Music>();
	if (!BossMusic->openFromFile(Folders::ROOT_FOLDER + Folders::MUSIC_FOLDER + _File))
	{
		EventContext ec("Cannot find boss music file: ", _File.c_str());
		return;
	}
	BossMusic->setLoop(_MustLoop);
}

void SoundPlayer::PlaySound(std::string _File, bool _MustLoop)
{
	if (!SoundBuffer->loadFromFile(Folders::ROOT_FOLDER + Folders::SOUNDS_FOLDER + _File))
	{
		EventContext ec("Cannot find sound file: ", _File.c_str());
		return;
	}
	CurrentSound->setBuffer(*SoundBuffer);
	CurrentSound->setLoop(_MustLoop);
	CurrentSound->play();
}

void SoundPlayer::PlaySoundFromBuffer(sf::SoundBuffer* _pBuffer, bool _MustLoop)
{
	CurrentSound->setBuffer(*_pBuffer);
	CurrentSound->setLoop(_MustLoop);
	CurrentSound->play();
}

void SoundPlayer::PlaySpecificMusic(sf::Music* _pMusic)
{
	if(PlayingMusic)
	{
		PlayingMusic->pause();
	}

	if (OtherMusic)
	{
		OtherMusic->pause();
	}

	_pMusic->play();
	PlayingMusic = _pMusic;
}

void SoundPlayer::FadeOutMusic(float _Speed)
{
	FadingOutMusic = PlayingMusic;
	FadingOut = true;
	FadeOutSpeed = _Speed;
	Volume = 100.0f;
}

void SoundPlayer::StopMusic()
{
	if (PlayingMusic)
	{
		PlayingMusic->stop();
	}

	if(OtherMusic)
	{
		OtherMusic->stop();
	}
}
