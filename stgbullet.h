#ifndef STGBULLET_H
#define STGBULLET_H


#include "scene/2d/node_2d.h"
#include "scene/2d/collision_shape_2d.h"


class STGBulletFactory : public Node2D {
	GDCLASS(STGBulletFactory, Node2D)

	struct STGBullet {
		Transform2D transform;
		Vector2 velocity;
		Rect2 src_rect;
		Rect2 dest_rect;
		Variant custom_data;
		bool is_queued_for_deletion = false;
		int id;
	};

	Ref<Shape2D> collision_shape;
	uint32_t collision_mask = 0;
	uint32_t collision_layer = 0;
	real_t clip_viewport_margin = 32.0;
	Rect2 clip_manual_rect = Rect2();
	uint32_t clip_type = 0;
	Ref<Texture2D> texture;
	int vframes = 1;
	int hframes = 1;
	int frame = 0;
	bool centered = true;
	Point2 offset;
	bool hflip = false;
	bool vflip = false;
	bool region = false ;
	Rect2 region_rect;
	bool region_filter_clip = false;
	bool animated = false;
	int anim_hframes = 0;
	int anim_vframes = 0;
	real_t anim_frame_length = 0.0;

	RID _area;
	Vector<STGBullet> _bullets;
	//Active bullets only. A linked list allows constant time deletion in the middle of a list, without changing the order of the elements.
	List<int> _active_bullets; 
	List<int> _unused_ids; //bullet ids are indices into the bullets vector.
	Vector2 _scale;

	void _get_rects(Rect2 &r_src_rect, Rect2 &r_dst_rect, bool &r_filter_clip) const;
	void _update_bullets();
	void _update_bullets_editor();
	void _draw_bullets();
	void _draw_bullets_editor();
	void _draw_editor();
	void _draw_collision();
	void _get_visible_rect(Rect2 &rect);
	

protected:
	
	void _notification(int p_what);
	static void _bind_methods();
	

public:
	STGBulletFactory();
	~STGBulletFactory();
	void _area_inout(int p_status, const RID &p_area, ObjectID p_instance, int p_area_shape, int p_self_shape);
	void _body_inout(int p_status, const RID &p_body, ObjectID p_instance, int p_body_shape, int p_area_shape);
	void _shape_changed();

	void set_collision_mask(uint32_t p_mask);
	uint32_t get_collision_mask() const;
	void set_collision_layer(uint32_t p_layer);
	uint32_t get_collision_layer() const;
	void set_collision_shape(const Ref<Shape2D> &p_shape);
	Ref<Shape2D> get_collision_shape() const;
	void set_clip_viewport_margin(real_t p_bounds_margin);
	real_t get_clip_viewport_margin() const;
	void set_texture(const Ref<Texture2D> &p_texture);
	Ref<Texture2D> get_texture() const;
	void set_vframes(int p_amount);
	int get_vframes() const;
	void set_hframes(int p_amount);
	int get_hframes() const;
	void set_frame(int frame);
	int get_frame() const;
	void set_centered(bool p_center);
	bool is_centered() const;
	void set_offset(const Point2 &p_offset);
	Point2 get_offset() const;
	void set_flip_h(bool p_flip);
	bool is_flipped_h() const;
	void set_flip_v(bool p_flip);
	bool is_flipped_v() const;

	void set_region(bool p_region);
	bool is_region() const;
	void set_region_rect(const Rect2 &p_region_rec);
	Rect2 get_region_rect() const;
	void set_region_filter_clip(bool p_enable);
	bool is_region_filter_clip_enabled() const ;

    void set_animated(bool p_animated);
	bool is_animated() const;
	void set_anim_vframes(int p_amount);
	int get_anim_vframes() const;
	void set_anim_hframes(int p_amount);
	int get_anim_hframes() const;
	void set_anim_frame_length(real_t p_length);
	real_t get_anim_frame_length() const;


	int create(Vector2 position, real_t angle, real_t speed);
	void queue_delete_bullet(int bullet_id);
	void clear();
	Array get_active_ids();
	int count();

	void set_bullet_position(int bullet_id, Vector2 position);
	Vector2 get_bullet_position(int bullet_id) const;
	void set_bullet_speed(int bullet_id, real_t speed);
	real_t get_bullet_speed(int bullet_id) const;
	void set_bullet_angle(int bullet_id, real_t speed);
	real_t get_bullet_angle(int bullet_id) const;

	Variant get_bullet_custom_data(int bullet_id) const;
	void set_bullet_custom_data(int bullet_id, Variant p_custom_data);

	enum {
		CLIP_TYPE_VIEWPORT = 0,
		CLIP_TYPE_MANUAL = 1,
		CLIP_TYPE_NONE = 2
	};

};

#endif