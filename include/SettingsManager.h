#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#ifndef Q_MOC_RUN
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#endif

class SettingsManager
{
public:
	static SettingsManager* Instance();

	int n_blur_loops;
	int filter_size;
	float multiplier1;
	float multiplier2;

	glm::vec4 light_pos;

private:
	static SettingsManager* instance_;
	SettingsManager();
	~SettingsManager();	
};

#endif