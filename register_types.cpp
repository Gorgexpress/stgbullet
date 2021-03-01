/* register_types.cpp */

#include "register_types.h"
#include "core/class_db.h"
#include "core/project_settings.h"
#include "stgbullet.h"

void register_stgbullet_types() {
   ClassDB::register_class<STGBulletFactory>();
   if (!ProjectSettings::get_singleton()->has_setting("stgbullet/clip_type")) {
      PropertyInfo pinfo = PropertyInfo(Variant::INT, "clip_type", PROPERTY_HINT_ENUM, "Viewport, None");
		//ProjectSettings::get_singleton()->set_setting("stgbullet/clip_margin", 64.0);
      ProjectSettings::get_singleton()->set_setting("stgbullet/clip_type", 0);
      ProjectSettings::get_singleton()->set_custom_property_info("stgbullet/clip_type", pinfo);
      ProjectSettings::get_singleton()->set_initial_value("stgbullet/clip_type", 0);
	}
    if (!ProjectSettings::get_singleton()->has_setting("stgbullet/clip_viewport_margin")) {
      PropertyInfo pinfo = PropertyInfo(Variant::REAL, "clip_viewport_margin", PROPERTY_HINT_RANGE, "0, 1000, 1,  or_greater");
		ProjectSettings::get_singleton()->set_setting("stgbullet/clip_viewport_margin", 64.0);
      ProjectSettings::get_singleton()->set_custom_property_info("stgbullet/clip_viewport_margin", pinfo);
      ProjectSettings::get_singleton()->set_initial_value("stgbullet/clip_viewport_margin", 64.0);
      
	}
}

void unregister_stgbullet_types() {
   //nothing to do here
}