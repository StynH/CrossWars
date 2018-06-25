#pragma once
#include <SFML/Graphics/Shader.hpp>
#include <memory>
#include "GameTime.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "TextureManager.h"

class CRTV : public Shader
{
public:
	CRTV() : Shader("..\\Assets\\Shaders\\CRTV.vert", "..\\Assets\\Shaders\\CRTV.frag")
	{
		pShader->setParameter("tex0", sf::Shader::CurrentTexture);
		pShader->setParameter("tex1", *TextureManager::GetInstance()->LoadTexture("noise.png", Folders::MENU));
		pShader->setParameter("resolution", sf::Vector2f(512, 512));
		pShader->setParameter("time", ShaderTimer);
	}

	void Update()
	{
		ShaderTimer += GameTime::DeltaTime;
		pShader->setParameter("time", ShaderTimer);
	}
};
