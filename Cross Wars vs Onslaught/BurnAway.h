#pragma once
#include <SFML/Graphics/Shader.hpp>
#include <memory>
#include "GameTime.h"
#include "Shader.h"
#include "ResourceManager.h"

class BurnAway : public Shader
{
public:
	BurnAway() : Shader("", "..\\Assets\\Shaders\\BurnAway.frag")
	{
		pShader->setParameter("tex0", sf::Shader::CurrentTexture);
		pShader->setParameter("tex1", *ResourceManager::GetInstance()->GetBlackBackground());
		pShader->setParameter("resolution", sf::Vector2f(512, 512));
		pShader->setParameter("time", ShaderTimer);
	}

	void Update()
	{
		ShaderTimer += GameTime::DeltaTime;
		pShader->setParameter("time", ShaderTimer);
	}
};
