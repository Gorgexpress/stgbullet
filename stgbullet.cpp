#include "STGBullet.h"
#include "core/engine.h"
#include "scene/main/viewport.h"


void STGBulletFactory::set_collision_shape(const Ref<Shape2D> &p_shape) {
	if (collision_shape.is_valid())
		collision_shape->disconnect("changed", this, "_shape_changed");
	collision_shape = p_shape;
	update();
	if (collision_shape.is_valid())
		collision_shape->connect("changed", this, "_shape_changed");
	//Doesn't effect shapes for bullets that have already been created for now.
	
}

Ref<Shape2D> STGBulletFactory::get_collision_shape() const {

	return collision_shape;
}

void STGBulletFactory::set_collision_mask(uint32_t p_mask) {

	collision_mask = p_mask;
	Physics2DServer::get_singleton()->area_set_collision_mask(_area, collision_mask);
}

uint32_t STGBulletFactory::get_collision_mask() const {

	return collision_mask;
}

void STGBulletFactory::set_collision_layer(uint32_t p_layer) {

	collision_layer = p_layer;
	Physics2DServer::get_singleton()->area_set_collision_layer(_area, collision_layer);
	
}

uint32_t STGBulletFactory::get_collision_layer() const {

	return collision_layer;
}

void STGBulletFactory::set_clip_viewport_margin(real_t p_clip_viewport_margin) {
	clip_viewport_margin = p_clip_viewport_margin;
}

real_t STGBulletFactory::get_clip_viewport_margin() const {
	return clip_viewport_margin;
}


void STGBulletFactory::set_texture(const Ref<Texture> &p_texture) {


	if (p_texture == texture)
		return;

	if (texture.is_valid())
		texture->remove_change_receptor(this);

	texture = p_texture;

	if (texture.is_valid())
		texture->add_change_receptor(this);

	update();
	emit_signal("texture_changed");
	item_rect_changed();
	_change_notify("texture");
}

Ref<Texture> STGBulletFactory::get_texture() const {

	return texture;
}


void STGBulletFactory::set_vframes(int p_amount) {

	ERR_FAIL_COND(p_amount < 1);
	vframes = p_amount;
	update();
	item_rect_changed();
	_change_notify();
}
int STGBulletFactory::get_vframes() const {

	return vframes;
}

void STGBulletFactory::set_hframes(int p_amount) {

	ERR_FAIL_COND(p_amount < 1);
	hframes = p_amount;
	update();
	item_rect_changed();
	_change_notify();
}
int STGBulletFactory::get_hframes() const {

	return hframes;
}

void STGBulletFactory::set_frame(int p_frame) {

	ERR_FAIL_INDEX(p_frame, vframes * hframes);

	if (frame != p_frame)
		item_rect_changed();

	frame = p_frame;

	_change_notify("frame");
	emit_signal("frame_changed");

}

int STGBulletFactory::get_frame() const {

	return frame;
}

void STGBulletFactory::set_centered(bool p_center) {
	centered = p_center;
	update();
	item_rect_changed();
}

bool STGBulletFactory::is_centered() const {
	return centered;
}

void STGBulletFactory::set_offset(const Point2 &p_offset) {
	offset = p_offset;
	update();
	item_rect_changed();
	_change_notify("offset");
}

Point2 STGBulletFactory::get_offset() const {
	return offset;
}

void STGBulletFactory::set_flip_h(bool p_flip) {
	hflip = p_flip;
	update();
}

bool STGBulletFactory::is_flipped_h() const {
	return hflip;
}

void STGBulletFactory::set_flip_v(bool p_flip) {
	vflip = p_flip;
	update();
}

bool STGBulletFactory::is_flipped_v() const {
	return vflip;
}

void STGBulletFactory::set_region(bool p_region) {
	if (p_region == region)
		return;

	region = p_region;
	update();
}

bool STGBulletFactory::is_region() const {
	return region;
}

void STGBulletFactory::set_region_rect(const Rect2 &p_region_rect) {
	if (region_rect == p_region_rect)
		return;

	region_rect = p_region_rect;

	if (region)
		item_rect_changed();

	_change_notify("region_rect");
}

Rect2 STGBulletFactory::get_region_rect() const {
	return region_rect;
}

void STGBulletFactory::set_region_filter_clip(bool p_enable) {
	region_filter_clip = p_enable;
	update();
}

bool STGBulletFactory::is_region_filter_clip_enabled() const {
	return region_filter_clip;
}

void STGBulletFactory::set_animated(bool p_animated) {
	if (p_animated == animated)
		return;

	region = p_animated;
	update();
}

bool STGBulletFactory::is_animated() const {
	return animated;
}

void STGBulletFactory::set_anim_vframes(int p_amount) {

	ERR_FAIL_COND(p_amount < 1);
	anim_vframes = p_amount;
	update();
	item_rect_changed();
	_change_notify();
}
int STGBulletFactory::get_anim_vframes() const {

	return anim_vframes;
}

void STGBulletFactory::set_anim_hframes(int p_amount) {

	ERR_FAIL_COND(p_amount < 1);
	anim_hframes = p_amount;
	update();
	item_rect_changed();
	_change_notify();
}
int STGBulletFactory::get_anim_hframes() const {

	return anim_hframes;
}

void STGBulletFactory::set_anim_frame_length(real_t p_length) {

	ERR_FAIL_COND(p_length <= 0.0);
	anim_frame_length = p_length;
	update();
	item_rect_changed();
	_change_notify();
}
real_t STGBulletFactory::get_anim_frame_length() const {

	return anim_frame_length;
}


void STGBulletFactory::_get_rects(Rect2 &r_src_rect, Rect2 &r_dst_rect, bool &r_filter_clip) const {
	Rect2 base_rect;

	if (region) {
		r_filter_clip = region_filter_clip;
		base_rect = region_rect;
	} else {
		r_filter_clip = false;
		base_rect = Rect2(0, 0, texture->get_width(), texture->get_height());
	}

	Size2 frame_size = base_rect.size / Size2(hframes, vframes);
	Point2 frame_offset = Point2(frame % hframes, frame / hframes);
	frame_offset *= frame_size;

	r_src_rect.size = frame_size;
	r_src_rect.position = base_rect.position + frame_offset;

	Point2 dest_offset = offset;
	if (centered) {
		dest_offset -= frame_size / 2;
	}
	if (Engine::get_singleton()->get_use_pixel_snap()) {
		dest_offset = dest_offset.floor();
	}

	r_dst_rect = Rect2(dest_offset, frame_size);

	if (hflip) {
		r_dst_rect.size.x = -r_dst_rect.size.x;
	}
	if (vflip) {
		r_dst_rect.size.y = -r_dst_rect.size.y;
	}
}

void STGBulletFactory::_update_bullets() {
	//Physics2DServer *ps = Physics2DServer::get_singleton();
	real_t delta = get_physics_process_delta_time();
	Rect2 visible_rect;
	_get_visible_rect(visible_rect);
	if (clip_type == CLIP_TYPE_VIEWPORT) 
		visible_rect = visible_rect.grow(clip_viewport_margin);
	else if(clip_type == CLIP_TYPE_MANUAL)
		visible_rect = clip_manual_rect;
	else 
		visible_rect = visible_rect.grow(100000000.0);
	for(int i = 0; i < _bullets.size(); i++) {
		if (_bullets[i].is_queued_for_deletion) 
			continue;
		_bullets.write[i].transform.set_origin(_bullets[i].transform.get_origin() + _bullets[i].velocity  * delta);
		_bullets.write[i].transform.set_rotation(_bullets[i].velocity.angle());
		if (!visible_rect.has_point(_bullets[i].transform.get_origin())) 
			_bullets.write[i].is_queued_for_deletion = true;
	}

	List<int>::Element *E = _active_bullets.front();
	while(E) {
		int idx = E->get();
		if( _bullets[idx].is_queued_for_deletion) {
			Physics2DServer::get_singleton()->area_set_shape_disabled(_area, idx, true);
			_unused_ids.push_front(_bullets[idx].id);
			E->erase();
		}
		else {
			//bullet->matrix[2] += bullet->direction * bullet->speed  * delta;
			Physics2DServer::get_singleton()->area_set_shape_transform(_area, _bullets[idx].id, _bullets[idx].transform);
		}
		E = E->next();
	}

	update();
}


void STGBulletFactory::_draw_bullets() {
	List<int>::Element *E = _active_bullets.front();
	while(E) {
		int i = E->get();
		draw_set_transform_matrix(_bullets[i].transform);
		draw_texture_rect_region(texture, _bullets[i].dest_rect, _bullets[i].src_rect);
		E = E->next();
	}
}

void STGBulletFactory::_draw_editor() {
	if (texture != NULL) {
		Rect2 dest_rect, src_rect;
		_get_rects(src_rect, dest_rect, region_filter_clip);
		draw_texture_rect_region(texture, dest_rect, src_rect);
	}
	if (collision_shape != NULL) {
		collision_shape->draw(get_canvas_item(), get_tree()->get_debug_collisions_color());
	}
}

int STGBulletFactory::create(Vector2 position, real_t angle, real_t speed) {
	STGBullet* bullet;
	Transform2D transform = get_transform();
	real_t rads = Math::deg2rad(angle);
	Vector2 velocity = Vector2(cos(rads), sin(rads)) * speed;
	transform.set_rotation(rads);
	transform.set_origin(position);
	if (_unused_ids.empty()) {
		STGBullet new_bullet;
		new_bullet.id = _bullets.size();
		_bullets.push_back(new_bullet);
		bullet = &_bullets.write[new_bullet.id];
		if (collision_shape != NULL) 
			Physics2DServer::get_singleton()->area_add_shape(_area, collision_shape->get_rid(), transform);
	} else {
		int id = _unused_ids.front()->get();
		_unused_ids.pop_front();
		bullet = &_bullets.write[id];
		if (collision_shape != NULL) {
			Physics2DServer::get_singleton()->area_set_shape_transform(_area, id, transform);
			Physics2DServer::get_singleton()->area_set_shape_disabled(_area, id, false);
		}
	}
	ERR_FAIL_COND_V(_active_bullets.size() >= _bullets.size(), NULL);
	bullet->transform = transform;
	bullet->velocity = velocity;
	bullet->is_queued_for_deletion = false;
	_get_rects(bullet->src_rect, bullet->dest_rect, region_filter_clip);
	_active_bullets.push_back(bullet->id);
	return bullet->id;
}

Array STGBulletFactory::get_active_ids() {
	Array r;
	r.resize(_active_bullets.size());
	int idx = 0;
	List<int>::Element *E = _active_bullets.front();
	while(E) {
		r[idx++] = E->get();
		E = E->next();
	}
	return r;
}

int STGBulletFactory::count() {
	return _active_bullets.size();
}

void STGBulletFactory::queue_delete_bullet(int bullet_id) {
	_bullets.write[bullet_id].is_queued_for_deletion = true;
}

void STGBulletFactory::clear() {
	List<int>::Element *E = _active_bullets.front();
	while(E) {
		_bullets.write[E->get()].is_queued_for_deletion = true;
		E =  E->next();
	}
}

void STGBulletFactory::set_bullet_position(int bullet_id, Vector2 position) {
	if (bullet_id < _bullets.size()) {
		_bullets.write[bullet_id].transform.set_origin(position);
	}
}

Vector2 STGBulletFactory::get_bullet_position(int bullet_id) const {
	if (bullet_id < _bullets.size()) {
		return _bullets[bullet_id].transform.get_origin();
	}
	return Vector2();
}

void STGBulletFactory::set_bullet_speed(int bullet_id, real_t speed) {
	if (bullet_id < _bullets.size()) {
		_bullets.write[bullet_id].velocity = _bullets[bullet_id].velocity.normalized() * speed;
	}
}

real_t STGBulletFactory::get_bullet_speed(int bullet_id) const {
	if (bullet_id < _bullets.size()) {
		return _bullets[bullet_id].velocity.length();
	}
	return 0.0;
}

void STGBulletFactory::set_bullet_angle(int bullet_id, real_t angle) {
	if (bullet_id < _bullets.size()) {
		real_t rads = Math::deg2rad(angle);
		_bullets.write[bullet_id].velocity = Vector2(cos(rads), sin(rads)) *_bullets[bullet_id].velocity.length();
	}
}

real_t STGBulletFactory::get_bullet_angle(int bullet_id) const {
	if (bullet_id < _bullets.size()) {
		return Math::rad2deg(_bullets[bullet_id].velocity.angle());
	}
	return 0.0;
}

void STGBulletFactory::set_bullet_custom_data(int bullet_id, Variant p_custom_data) {
	if (bullet_id < _bullets.size()) {
		_bullets.write[bullet_id].custom_data = p_custom_data;
	}
}

Variant STGBulletFactory::get_bullet_custom_data(int bullet_id) const {
	if (bullet_id < _bullets.size()) {
		return _bullets[bullet_id].custom_data;
	}
	return Variant();
}

void STGBulletFactory::_notification(int p_what) {

	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			if (!Engine::get_singleton()->is_editor_hint()) {
				set_transform(Transform2D()); //don't want bullets to be drawn at an offset
			}
			Physics2DServer* ps = Physics2DServer::get_singleton();
			ps->area_set_space(_area, get_world_2d()->get_space());
			ps->area_set_transform(_area, Transform2D());
			ps->area_set_collision_layer(_area, collision_layer);
			ps->area_set_collision_mask(_area, collision_mask);

			if (ProjectSettings::get_singleton()->has_setting("stgbullet/clip_type")) {
				clip_type = ProjectSettings::get_singleton()->get("stgbullet/clip_type");
			}
			if (ProjectSettings::get_singleton()->has_setting("stgbullet/clip_viewport_margin")) {
				clip_viewport_margin = ProjectSettings::get_singleton()->get("stgbullet/clip_viewport_margin");
			}
			/*if (ProjectSettings::get_singleton()->has_setting("stgbullet/clip_manual_rect")) {
				clip_manual_rect = ProjectSettings::get_singleton()->get("stgbullet/clip_manual_rect");
			}*/
		} break;
		case NOTIFICATION_READY: {
			set_physics_process(true);
			set_as_toplevel(true);
			//VS::get_singleton()->canvas_item_set_z_index(get_canvas_item(), z_index);
		} break;
		case NOTIFICATION_DRAW: {
			if (Engine::get_singleton()->is_editor_hint() && texture != NULL) {
				_draw_editor();
			}
			_draw_bullets();
		} break;

		case NOTIFICATION_PROCESS: {
		} break;
		case NOTIFICATION_PHYSICS_PROCESS: {
			_update_bullets();
		} break; 
	}
}


void STGBulletFactory::_area_inout(int p_status, const RID &p_area, int p_instance, int p_area_shape, int p_self_shape) {
	//ERR_FAIL_COND(p_self_shape >= _shapes.size());
	Object* collider = ObjectDB::get_instance(p_instance);
	emit_signal("area_entered_bullet", p_self_shape, collider);
	/*int bullet_id = _shapes[p_self_shape];
	if (bullet_id != -1 && _bullet_manager->is_bullet_active(bullet_id)) {
		emit_signal("area_entered_bullet", bullet_id, collider);
	}*/
}
void STGBulletFactory::_body_inout(int p_status, const RID &p_body, int p_instance, int p_body_shape, int p_area_shape) {
	//ERR_FAIL_COND(p_area_shape >= _shapes.size());
	Object* collider = ObjectDB::get_instance(p_instance);
	emit_signal("body_entered_bullet", p_area_shape, collider);
	/*int bullet_id = _shapes[p_area_shape];
	if (bullet_id != -1 && _bullet_manager->is_bullet_active(bullet_id)) {
		emit_signal("body_entered_bullet", bullet_id, collider);
	}*/
	
}

void STGBulletFactory::_shape_changed() {
	update();
}

void STGBulletFactory::_get_visible_rect(Rect2& rect)
{
	Transform2D ctrans = get_viewport()->get_canvas_transform();
	rect.position = -ctrans.get_origin() / ctrans.get_scale();
	rect.size = get_viewport_rect().size;
}

void STGBulletFactory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("create", "position", "angle","speed"), &STGBulletFactory::create);
	ClassDB::bind_method(D_METHOD("queue_delete_bullet", "bullet_id"), &STGBulletFactory::queue_delete_bullet);
	ClassDB::bind_method(D_METHOD("clear"), &STGBulletFactory::clear);
	ClassDB::bind_method(D_METHOD("get_active_ids"), &STGBulletFactory::get_active_ids);
	ClassDB::bind_method(D_METHOD("count"), &STGBulletFactory::count);
	ClassDB::bind_method(D_METHOD("set_bullet_position", "bullet_id", "position"), &STGBulletFactory::set_bullet_position);
	ClassDB::bind_method(D_METHOD("get_bullet_position", "bullet_id"), &STGBulletFactory::get_bullet_position);
	ClassDB::bind_method(D_METHOD("set_bullet_speed", "bullet_id", "speed"), &STGBulletFactory::set_bullet_speed);
	ClassDB::bind_method(D_METHOD("get_bullet_speed", "bullet_id"), &STGBulletFactory::get_bullet_speed);
	ClassDB::bind_method(D_METHOD("set_bullet_angle", "bullet_id", "angle"), &STGBulletFactory::set_bullet_angle);
	ClassDB::bind_method(D_METHOD("get_bullet_angle", "bullet_id"), &STGBulletFactory::get_bullet_angle);
	ClassDB::bind_method(D_METHOD("set_bullet_custom_data", "bullet_id", "custom_data"), &STGBulletFactory::set_bullet_custom_data);
	ClassDB::bind_method(D_METHOD("get_bullet_custom_data", "bullet_id"), &STGBulletFactory::get_bullet_custom_data);

	//PHYSICS
	ClassDB::bind_method(D_METHOD("_body_inout"), &STGBulletFactory::_body_inout);
	ClassDB::bind_method(D_METHOD("_area_inout"), &STGBulletFactory::_area_inout);
	ClassDB::bind_method(D_METHOD("_shape_changed"), &STGBulletFactory::_shape_changed);
	ClassDB::bind_method(D_METHOD("set_collision_shape", "collision_shape"), &STGBulletFactory::set_collision_shape);
	ClassDB::bind_method(D_METHOD("get_collision_shape"), &STGBulletFactory::get_collision_shape);
	ClassDB::bind_method(D_METHOD("set_collision_layer", "collision_layer"), &STGBulletFactory::set_collision_layer);
	ClassDB::bind_method(D_METHOD("get_collision_layer"), &STGBulletFactory::get_collision_layer);
	ClassDB::bind_method(D_METHOD("set_collision_mask", "collision_mask"), &STGBulletFactory::set_collision_mask);
	ClassDB::bind_method(D_METHOD("get_collision_mask"), &STGBulletFactory::get_collision_mask);
	ClassDB::bind_method(D_METHOD("set_clip_viewport_margin", "clip_viewport_margin"), &STGBulletFactory::set_clip_viewport_margin);
	ClassDB::bind_method(D_METHOD("get_clip_viewport_margin"), &STGBulletFactory::get_clip_viewport_margin);
	
	//VISUAL
	ClassDB::bind_method(D_METHOD("set_texture", "texture"), &STGBulletFactory::set_texture);
	ClassDB::bind_method(D_METHOD("get_texture"), &STGBulletFactory::get_texture);
	ClassDB::bind_method(D_METHOD("set_vframes", "vframes"), &STGBulletFactory::set_vframes);
	ClassDB::bind_method(D_METHOD("get_vframes"), &STGBulletFactory::get_vframes);
	ClassDB::bind_method(D_METHOD("set_hframes", "hframes"), &STGBulletFactory::set_hframes);
	ClassDB::bind_method(D_METHOD("get_hframes"), &STGBulletFactory::get_hframes);
	ClassDB::bind_method(D_METHOD("set_frame", "frame"), &STGBulletFactory::set_frame);
	ClassDB::bind_method(D_METHOD("get_frame"), &STGBulletFactory::get_frame);
	ClassDB::bind_method(D_METHOD("set_centered", "centered"), &STGBulletFactory::set_centered);
	ClassDB::bind_method(D_METHOD("is_centered"), &STGBulletFactory::is_centered);
	ClassDB::bind_method(D_METHOD("set_offset", "offset"), &STGBulletFactory::set_offset);
	ClassDB::bind_method(D_METHOD("get_offset"), &STGBulletFactory::get_offset);

	ClassDB::bind_method(D_METHOD("set_flip_h", "flip_h"), &STGBulletFactory::set_flip_h);
	ClassDB::bind_method(D_METHOD("is_flipped_h"), &STGBulletFactory::is_flipped_h);

	ClassDB::bind_method(D_METHOD("set_flip_v", "flip_v"), &STGBulletFactory::set_flip_v);
	ClassDB::bind_method(D_METHOD("is_flipped_v"), &STGBulletFactory::is_flipped_v);
	ClassDB::bind_method(D_METHOD("set_region", "enabled"), &STGBulletFactory::set_region);
	ClassDB::bind_method(D_METHOD("is_region"), &STGBulletFactory::is_region);
	ClassDB::bind_method(D_METHOD("set_region_rect", "rect"), &STGBulletFactory::set_region_rect);
	ClassDB::bind_method(D_METHOD("get_region_rect"), &STGBulletFactory::get_region_rect);
	ClassDB::bind_method(D_METHOD("set_region_filter_clip", "enabled"), &STGBulletFactory::set_region_filter_clip);
	ClassDB::bind_method(D_METHOD("is_region_filter_clip_enabled"), &STGBulletFactory::is_region_filter_clip_enabled);

	ClassDB::bind_method(D_METHOD("set_animated", "enabled"), &STGBulletFactory::set_animated);
	ClassDB::bind_method(D_METHOD("is_animated"), &STGBulletFactory::is_animated);
	ClassDB::bind_method(D_METHOD("set_anim_vframes", "vframes"), &STGBulletFactory::set_anim_vframes);
	ClassDB::bind_method(D_METHOD("get_anim_vframes"), &STGBulletFactory::get_anim_vframes);
	ClassDB::bind_method(D_METHOD("set_anim_hframes", "hframes"), &STGBulletFactory::set_anim_hframes);
	ClassDB::bind_method(D_METHOD("get_anim_hframes"), &STGBulletFactory::get_anim_hframes);
	ClassDB::bind_method(D_METHOD("set_anim_frame_length", "length"), &STGBulletFactory::set_anim_frame_length);
	ClassDB::bind_method(D_METHOD("get_anim_frame_length"), &STGBulletFactory::get_anim_frame_length);

	ADD_SIGNAL(MethodInfo("frame_changed"));
	ADD_SIGNAL(MethodInfo("texture_changed"));
	ADD_SIGNAL(MethodInfo("area_entered_bullet", PropertyInfo(Variant::INT, "bullet_id", PROPERTY_HINT_NONE, "bullet_id"), PropertyInfo(Variant::OBJECT, "area", PROPERTY_HINT_RESOURCE_TYPE, "Area2D")));
	ADD_SIGNAL(MethodInfo("body_entered_bullet", PropertyInfo(Variant::INT, "bullet_id", PROPERTY_HINT_NONE, "bullet_id"), PropertyInfo(Variant::OBJECT, "body", PROPERTY_HINT_RESOURCE_TYPE, "PhysicsBody2D")));

	//PHYSICS PROPERTIES
	ADD_GROUP("Collision", "collision_");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_layer", PROPERTY_HINT_LAYERS_2D_PHYSICS), "set_collision_layer", "get_collision_layer");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_2D_PHYSICS), "set_collision_mask", "get_collision_mask");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "collision_shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape2D"), "set_collision_shape", "get_collision_shape");
	//VISUAL PROPERTIES
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture");
	//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "normal_map", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_normal_map", "get_normal_map");
	ADD_GROUP("Offset", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "centered"), "set_centered", "is_centered");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "offset"), "set_offset", "get_offset");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_h"), "set_flip_h", "is_flipped_h");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flip_v"), "set_flip_v", "is_flipped_v");
	ADD_GROUP("Animation", "");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "vframes", PROPERTY_HINT_RANGE, "1,16384,1"), "set_vframes", "get_vframes");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "hframes", PROPERTY_HINT_RANGE, "1,16384,1"), "set_hframes", "get_hframes");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "frame", PROPERTY_HINT_SPRITE_FRAME), "set_frame", "get_frame");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "animation_enabled"), "set_animated", "is_animated");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "anim_vframes", PROPERTY_HINT_RANGE, "1,16384,1"), "set_anim_vframes", "get_anim_vframes");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "anim_hframes", PROPERTY_HINT_RANGE, "1,16384,1"), "set_anim_hframes", "get_anim_hframes");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "anim_frame_length", PROPERTY_HINT_NONE), "set_anim_frame_length", "get_anim_frame_length");
	ADD_GROUP("Region", "region_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "region_enabled"), "set_region", "is_region");
	ADD_PROPERTY(PropertyInfo(Variant::RECT2, "region_rect"), "set_region_rect", "get_region_rect");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "region_filter_clip"), "set_region_filter_clip", "is_region_filter_clip_enabled");
}

STGBulletFactory::STGBulletFactory() {
	Physics2DServer* ps = Physics2DServer::get_singleton();
	_area = ps->area_create();
	ps->area_set_transform(_area, Transform2D());
	ps->area_attach_object_instance_id(_area, get_instance_id());
	ps->area_set_monitorable(_area, true);
	ps->area_set_monitor_callback(_area, this, "_body_inout");
	ps->area_set_area_monitor_callback(_area, this, "_area_inout");
}


STGBulletFactory::~STGBulletFactory() {
	if(_area.is_valid())
		Physics2DServer::get_singleton()->free(_area);
}
