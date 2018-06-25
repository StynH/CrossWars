#pragma once
#include <SFML/Graphics/Shader.hpp>
#include <memory>
#include "GameTime.h"

class Shader
{
public:
	Shader(std::string _VertName = "", std::string _FragName = "")
	{
		pShader = std::make_shared<sf::Shader>();
		
		if(!_VertName.empty() && !_FragName.empty())
		{
			pShader->loadFromFile(_VertName, _FragName);
		}
		else if(!_VertName.empty())
		{
			pShader->loadFromFile(_VertName, sf::Shader::Vertex);
		}
		else if (!_FragName.empty())
		{
			pShader->loadFromFile(_FragName, sf::Shader::Fragment);
		}

		ShaderTimer = 0;
	}

	virtual ~Shader()
	{
	}

	virtual void Update() = 0;

	virtual void Reset()
	{
		ShaderTimer = 0;
	}

	sf::Shader* GetShader()
	{
		return pShader.get();
	}

protected:
	float ShaderTimer;
	std::shared_ptr<sf::Shader> pShader;
};
