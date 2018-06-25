#include "Entity.h"
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>

Entity::Entity()
{
	generateGUID();
}


Entity::~Entity()
{

}

void Entity::generateGUID()
{
	boost::uuids::uuid UUID = boost::uuids::random_generator()();
	GUID = boost::lexical_cast<std::string>(UUID);
}

bool operator==(const Entity& Left, const Entity& Right)
{
	return Left.getGUID() == Right.getGUID();
}

bool operator!=(const Entity& Left, const Entity& Right)
{
	return Left.getGUID() != Right.getGUID();
}
