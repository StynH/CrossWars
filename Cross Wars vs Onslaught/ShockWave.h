#pragma once
#include <SFML/Graphics/Shader.hpp>
#include <memory>
#include "GameTime.h"
#include "Shader.h"

class ShockWave : public Shader
{
public:
	ShockWave(sf::Vector2f _Pos) : Shader("..\\Assets\\Shaders\\Shockwave.vert", "..\\Assets\\Shaders\\Shockwave.frag")
	{
		pShader->setParameter("sceneTex", sf::Shader::CurrentTexture);
		pShader->setParameter("center", _Pos);
		pShader->setParameter("time", ShaderTimer);
		pShader->setParameter("shockParams", sf::Vector3f(30.0, 0.8, 0.1));
	}

	void SetPosition(sf::Vector2f _Pos)
	{
		pShader->setParameter("center", _Pos);
	}

	void Update()
	{
		ShaderTimer += GameTime::DeltaTime * 1.5F;
		pShader->setParameter("time", ShaderTimer);
	}
};
