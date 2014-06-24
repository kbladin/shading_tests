#include "../include/SettingsManager.h"

SettingsManager* SettingsManager::instance_;

SettingsManager* SettingsManager::Instance()
{
	if (!instance_)
	{
		instance_ = new SettingsManager;
	}
	return instance_;
}

SettingsManager::SettingsManager() : N_LIGHTSOURCES(1)
{
	n_blur_loops = 5;
	filter_size = 29;
	multiplier1 = 1.0f;
	multiplier2 = 1.0f;
}