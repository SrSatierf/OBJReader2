#pragma once
#include"Material.h"
#include <string>
#include<map>

typedef std::map<std::string, Material> MaterialMapping;
class MTLReader
{
public:
	MTLReader();
	~MTLReader();
	static MaterialMapping read(std::string path);
};

