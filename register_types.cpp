/* register_types.cpp */

#include "register_types.h"
#include "core/object/class_db.h"
#include "core/config/project_settings.h"
#include "stgbullet.h"

void initialize_stgbullet_module(ModuleInitializationLevel p_level) {
   if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
            return;
    }
   ClassDB::register_class<STGBulletFactory>();
   if (!ProjectSettings::get_singleton()->has_setting("stgbullet/clip_type")) {
      PropertyInfo pinfo = PropertyInfo(Variant::INT, "stgbullet/clip_type", PROPERTY_HINT_ENUM, "Viewport, None");
		//ProjectSettings::get_singleton()->set_setting("stgbullet/clip_margin", 64.0);
      ProjectSettings::get_singleton()->set_setting("stgbullet/clip_type", 0);
      ProjectSettings::get_singleton()->set_custom_property_info(pinfo);
      ProjectSettings::get_singleton()->set_initial_value("stgbullet/clip_type", 0);
	}
    if (!ProjectSettings::get_singleton()->has_setting("stgbullet/clip_viewport_margin")) {
      PropertyInfo pinfo = PropertyInfo(Variant::FLOAT, "stgbullet/clip_viewport_margin", PROPERTY_HINT_RANGE, "0, 1000, 1,  or_greater");
		ProjectSettings::get_singleton()->set_setting("stgbullet/clip_viewport_margin", 64.0);
      ProjectSettings::get_singleton()->set_custom_property_info(pinfo);
      ProjectSettings::get_singleton()->set_initial_value("stgbullet/clip_viewport_margin", 64.0);
      
	}
}

void uninitialize_stgbullet_module(ModuleInitializationLevel p_level) {
   //nothing to do here
}