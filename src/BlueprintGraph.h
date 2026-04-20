#pragma once
#include <filesystem>
#include "BlueprintData.h"

class BlueprintGraph
{
public:
	bool loadFromJson(const std::filesystem::path& path);
	const BlueprintData& getBlueprintData() const;
private:
	BlueprintData mBlueprintData;
};