#pragma once
#include <SFML/Graphics/Shader.hpp>
#include <memory>
#include "GameTime.h"
#include "Shader.h"

class MindBend : public Shader
{
public:
	MindBend() : Shader("", "..\\Assets\\Shaders\\Mindbend.frag")
	{
		pShader->setParameter("tex0", sf::Shader::CurrentTexture);
		pShader->setParameter("resolution", sf::Vector2f(512, 512));
		pShader->setParameter("time", ShaderTimer);
	}

	void Update()
	{
		ShaderTimer += GameTime::DeltaTime * 1.5F;
		pShader->setParameter("time", ShaderTimer);
	}
};
