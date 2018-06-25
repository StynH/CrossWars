#pragma once
#include "Singleton.h"
#include <memory>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <map>

class SoundPlayer : public TSingleton<SoundPlayer>
{
public:
	SoundPlayer();
	~SoundPlayer();

	void Update();

	void Transition(sf::Music* _pMusic, int _TransitioningSpeed);
	void Transition(std::string _File, int _TransitioningSpeed, bool _MustLoop);
	void PlayMusic(std::string _File, bool _MustLoop);
	void SetLevelMusic(std::string _File, bool _MustLoop);
	void SetBattleMusic(std::string _File, bool _MustLoop);
	void SetBossMusic(std::string _File, bool _MustLoop);
	void PlaySound(std::string _File, bool _MustLoop);
	void PlaySoundFromBuffer(sf::SoundBuffer* _pBuffer, bool _MustLoop);
	void PlaySpecificMusic(sf::Music* _pMusic);
	void FadeOutMusic(float _Speed);
	void StopMusic();

	sf::SoundBuffer* CursorSound;
	sf::SoundBuffer* CancelSound;
	sf::SoundBuffer* RecieveSound;
	sf::SoundBuffer* ErrorSound;
	sf::SoundBuffer* SelectSound;
	sf::SoundBuffer* TalkSound;
	sf::SoundBuffer* ItemUseSound;
	sf::SoundBuffer* TurnTransition;
	sf::SoundBuffer* BattleTransition;
	sf::SoundBuffer* HitSound;
	sf::SoundBuffer* CritSound;

	std::shared_ptr<sf::Music> LevelMusic;
	std::shared_ptr<sf::Music> BattleMusic;
	std::shared_ptr<sf::Music> BossMusic;

	std::shared_ptr<sf::Music> OtherMusic;

	sf::Music* PlayingMusic;
	sf::Music* QueuedMusic;

private:
	std::map<std::string, sf::SoundBuffer*> MusicBank;

	float Volume;
	float FadeOutSpeed;

	sf::SoundBuffer* SoundBuffer;
	sf::Sound* CurrentSound;

	sf::Music* FadingOutMusic;
	bool FadingOut;

	int TransitioningSpeed;
	bool TransitioningTo, TransitioningFrom;
};

