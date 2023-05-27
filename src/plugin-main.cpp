/*
Tapgame Plugin
Copyright (C) 2023 Adsight b.reznik@adsight.tech

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <obs-module.h>
#include <obs-frontend-api.h>
#include <util/config-file.h>

#include <QMainWindow>

#include "plugin-macros.generated.h"
#include <DelayAgent.hpp>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

bool obs_module_load(void)
{
	const auto main_window = static_cast<QMainWindow *>(obs_frontend_get_main_window());
	new DelayAgent(main_window);

#if QT_CONFIG(ssl)
	blog(LOG_INFO, "TapGame plugin initiated with SSL");
#else
	blog(LOG_INFO, "TapGame plugin initiated without SSL");
#endif
	
	return true;
}

void obs_module_unload()
{
	blog(LOG_INFO, "TapGame plugin unloaded");
}
