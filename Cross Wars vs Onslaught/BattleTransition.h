#pragma once
#include <SFML/Graphics/Shader.hpp>
#include <memory>
#include "GameTime.h"
#include "Shader.h"
#include "ResourceManager.h"

class BattleTransition : public Shader
{
public:
	BattleTransition(bool _IsReverse, sf::Vector2f _Resolution) : Shader("..\\Assets\\Shaders\\BattleTransition.vert", "..\\Assets\\Shaders\\BattleTransition.frag")
	{
		pShader->setParameter("tex0", sf::Shader::CurrentTexture);
		pShader->setParameter("time", ShaderTimer);
		pShader->setParameter("resolution", _Resolution);
		pShader->setParameter("isreverse", _IsReverse);
		SecondTexture = nullptr;
	}

	void Update()
	{
		ShaderTimer += GameTime::DeltaTime;
		pShader->setParameter("time", ShaderTimer * 0.7);
	}

	void SetResolution(sf::Vector2f _Resolution)
	{
		pShader->setParameter("resolution", _Resolution);
	}

	void SetSecondTexture(sf::Texture* _pTexture)
	{	
		pShader->setParameter("tex1", *_pTexture);
		SecondTexture = _pTexture;
	}

private:
	sf::Texture* SecondTexture;
};
