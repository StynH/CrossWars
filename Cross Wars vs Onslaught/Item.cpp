#include "Item.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

Item::Item()
{
	GUID = GenerateRandomGUID();
}


Item::~Item()
{
	delete SPRITE;
}

std::string Item::GenerateRandomGUID()
{
	boost::uuids::uuid UUID = boost::uuids::random_generator()();
	return boost::lexical_cast<std::string>(UUID);
}