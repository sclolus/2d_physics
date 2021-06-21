use graphics::*;

extern crate piston_window;
extern crate piston;
// extern crate cgmath;
use cgmath;

use cgmath::Vector2;
type Vec2 = Vector2<f64>;

use piston_window::*;

mod universe;
mod object;

use universe::Universe;
use object::Object;

use cgmath::prelude::*;
use rand::Rng;

use std::time::{Instant, Duration};


fn main() {
    let mut window: PistonWindow =
        WindowSettings::new("Hello Piston!", [640, 480])
        .exit_on_esc(true).build().unwrap();
    let size = window.size();

    let object_number = 300;
    let mut rng = rand::thread_rng();    
    let mut universe = Universe::new(window.size(), object_number);

    let mut old_time = Instant::now();

    struct Camera {
	pub zoom: f64,
	pub pos: Vector2<f64>,
	pub movement_speed: f64,
    }

    impl Camera {
	const ZOOM_SPEED: f64 = 1.05;
	fn initial_camera_pos(size: piston_window::Size) -> Vector2<f64> {
	    // let half_width = size.width as f64;
	    // let half_height = size.height as f64;

	    // Vector2::new(-half_width, -half_height)
	    Vector2::new(0.0, 0.0)
	}
	
	pub fn new(size: piston_window::Size) -> Self {
	    Camera {
		zoom: 1.0,
		pos: Self::initial_camera_pos(size),
		movement_speed: 10.0,
	    }
	}

	pub fn reset_pos(&mut self, size: piston_window::Size) {
	    self.pos = Self::initial_camera_pos(size);
	}

	pub fn zoom(&mut self) -> &mut Self {
	    self.zoom *= Self::ZOOM_SPEED;
	    self
	}

	pub fn dezoom(&mut self) -> &mut Self {
	    self.zoom /= Self::ZOOM_SPEED;
	    self
	}

	pub fn move_up(&mut self) -> &mut Self {
	    self.pos -= Vector2::new(0.0, self.movement_speed) * self.zoom.exp();
	    self
	}
	
	pub fn move_down(&mut self) -> &mut Self {
	    self.pos += Vector2::new(0.0, self.movement_speed) * self.zoom.exp();
	    self
	}

	pub fn move_left(&mut self) -> &mut Self {
	    self.pos -= Vector2::new(self.movement_speed, 0.0) * self.zoom.exp();
	    self
	}

	pub fn move_right(&mut self) -> &mut Self {
	    self.pos += Vector2::new(self.movement_speed, 0.0) * self.zoom.exp();
	    self
	}

	pub fn zoom_to_bounding_box(&mut self, size: piston_window::Size, min: Vec2, max: Vec2) {
	    let original_movement_speed = self.movement_speed;
	    self.movement_speed = ZOOM_SMOOTHNESS;
		
	    let mut width = size.width / self.zoom;
	    let mut height = size.height / self.zoom;

	    let mut half_width = width / 2.0;
	    let mut half_height = height / 2.0;
	    
	    let box_to_bound = (min, max);
	    let mut camera_box = (Vector2::new(self.pos.x - half_width, self.pos.y - half_height),
				  Vector2::new(self.pos.x + half_width, self.pos.y + half_height));
	    let box_contains = |a: (Vec2, Vec2), b: (Vec2, Vec2)| { a.0.x < b.0.x && a.0.y < b.0.y
								    && a.1.x > b.1.x && a.1.y > b.1.y};

	    const ZOOM_SMOOTHNESS: f64 = 1.0001;

	    while !box_contains(camera_box, box_to_bound) {
		self.dezoom();

		width = size.width / self.zoom;  
		height = size.height / self.zoom;
                
		half_width = width / 2.0;        
		half_height = height / 2.0;      		

		
		camera_box = (Vector2::new(self.pos.x - half_width, self.pos.y - half_height),
			      Vector2::new(self.pos.x + half_width, self.pos.y + half_height));
	    }
	    
	    while box_contains(camera_box, box_to_bound) {
		self.zoom();

		width = size.width / self.zoom;  
		height = size.height / self.zoom;
                
		half_width = width / 2.0;        
		half_height = height / 2.0;      		

		
		camera_box = (Vector2::new(self.pos.x - half_width, self.pos.y - half_height),
			      Vector2::new(self.pos.x + half_width, self.pos.y + half_height));
	    }

	    self.dezoom(); // This dance of zooming-dezooming seemed like the simpliest way to find the (almost-)minimum bouding camera box
	    self.movement_speed = original_movement_speed;
	}
    }

    use std::collections::BTreeSet;
    
    let mut cam = Camera::new(size);
    let mut keys_pressed: BTreeSet<Key> = BTreeSet::new();

    let half_width = size.width as f64 / 2.0;
    let half_height = size.height as f64 / 2.0;

    let mut keep_camera_zoomed_to_universe_bounding_box = false;
    let mut cam_following = false;
    let mut object_followed = 0;
	

    let assets = find_folder::Search::ParentsThenKids(3, 3)
        .for_folder("assets").unwrap();
    let ref font = assets.join("OpenSans-Semibold.ttf");
    
    let mut glyphs = window.load_font(font).unwrap();

    while let Some(event) = window.next() {
	let elapsed_time = old_time.elapsed().as_secs_f64();
	old_time = Instant::now();

	use piston::Button::Keyboard;

	
	match event {
	    Event::Input(Input::Button(ButtonArgs {
		state: ButtonState::Press,
		button: Keyboard(key),
		scancode: _}), _) => {
		keys_pressed.insert(key);
	    },
	    Event::Input(Input::Button(ButtonArgs {
		state: ButtonState::Release,
		button: Keyboard(key),
		scancode: _}), _) => { keys_pressed.remove(&key) ; },
	    _ => ()
	}

	for key in keys_pressed.clone().iter() {
	    match key {
		Key::R => { universe = universe.reset(); cam.reset_pos(size); keys_pressed.remove(&Key::R); },
		Key::Left => { universe.slow_down(); },
		Key::Right => { universe.speed_up(); },
		Key::Down => { cam.dezoom(); },
		Key::Up => { cam.zoom(); },
		Key::W => { cam.move_up(); },
		Key::A => { cam.move_left(); },
		Key::S => { cam.move_down(); },
		Key::D => { cam.move_right(); },
		Key::B => { keep_camera_zoomed_to_universe_bounding_box ^= true; keys_pressed.remove(&Key::B); }
		Key::M => { cam_following ^= true; object_followed = rng.gen_range(0..universe.objects.len()); keys_pressed.remove(&Key::M); },
		_ => (),
	    }
	}
	

	let time_ratio = universe.time_ratio; // Okay technically time_ratio could have been updated in the keyboard match, and that would be wrong to use it as the time_ratio for the current frame. But the universe update and keyboard events should be separated anyway.

	universe = universe.apply_elapsed_time(elapsed_time * time_ratio);
	universe = universe.apply_gravity();


	if keep_camera_zoomed_to_universe_bounding_box {
	    let (min, max) = universe.bounding_box();
	    
	    cam.zoom_to_bounding_box(size, min, max);
	}

	if cam_following {
	    cam.pos = universe.objects[object_followed].pos;
	}
	
	use graphics::text;

	let text = Text::new_color([1.0, 1.0, 1.0, 1.0], 16);
	
	
	window.draw_2d(&event, |context, graphics, _device| {
            clear([0.0; 4], graphics);

	    let time_ratio_text = format!("time_ratio: {:.10}", time_ratio);
	    text.draw(&time_ratio_text, &mut glyphs, &DrawState::default(), context.transform.trans(10.0, 20.0), graphics);

	    let zoom = format!("zoom: {:.10}", cam.zoom);
	    text.draw(&zoom, &mut glyphs, &DrawState::default(), context.transform.trans(10.0, 40.0), graphics);

	    let cam_pos = format!("cam pos: ({}, {})", cam.pos.x, cam.pos.y);
	    text.draw(&cam_pos, &mut glyphs, &DrawState::default(), context.transform.trans(10.0, 60.0), graphics);

	    let number_of_objects_text = format!("number of objects: {})", universe.objects.len());
	    text.draw(&number_of_objects_text, &mut glyphs, &DrawState::default(), context.transform.trans(10.0, 80.0), graphics);

	    let bounding_text = format!("bounding: {}", keep_camera_zoomed_to_universe_bounding_box);
	    text.draw(&bounding_text, &mut glyphs, &DrawState::default(), context.transform.trans(10.0, 100.0), graphics);

	    let following_text = format!("following: {}", cam_following);
	    text.draw(&following_text, &mut glyphs, &DrawState::default(), context.transform.trans(10.0, 120.0), graphics);

	    let v = universe.objects[object_followed].velocity;
	    
	    let followed_object_velocity_text = format!("followed object velocity: ({:.10}, {:.10})", v.x, v.y);
	    text.draw(&followed_object_velocity_text, &mut glyphs, &DrawState::default(), context.transform.trans(10.0, 140.0), graphics);
	    
	    let a = universe.objects[object_followed].acceleration;

	    let followed_object_acceleration_text = format!("followed object acceleration: ({:.10}, {:.10})", a.x, a.y);
	    text.draw(&followed_object_acceleration_text, &mut glyphs, &DrawState::default(), context.transform.trans(10.0, 160.0), graphics);

	    let (min, max) = universe.bounding_box();
	    let universe_bounding_box_text = format!("univers bounding box: (({:.10}, {:.10}), {:.10}, {:.10})", min.x, min.y, max.x, max.y);
	    text.draw(&universe_bounding_box_text, &mut glyphs, &DrawState::default(), context.transform.trans(10.0, 180.0), graphics);



	    glyphs.factory.encoder.flush(_device);

	    for object in universe.objects.iter() {
		let pos = Vector2::new(object.pos[0], object.pos[1]);
		let half_vector = Vector2::new(half_width, half_height);
		let pos_relative_to_camera = pos - cam.pos;

		let zoomed_position = pos_relative_to_camera * cam.zoom;
		let centered_position = zoomed_position + half_vector;
		let final_pos = centered_position;
		
		let ellipse_geometry = graphics::ellipse::circle(final_pos.x,
								 final_pos.y,
								 (object.radius * cam.zoom).max(1.0));

		use std::cmp::Ordering;
		let min_mass = universe.objects.iter().min_by(|a, b| if a.mass < b.mass { Ordering::Less } else { Ordering::Greater} ).unwrap().mass;
		let max_mass = universe.objects.iter().max_by(|a, b| if a.mass < b.mass { Ordering::Less } else { Ordering::Greater}).unwrap().mass;
		
		let mut color = [0.0, 0.5, 0.0, 1.0];
		color[0] = ((object.mass - min_mass) / (max_mass - min_mass)).tanh() as f32;
		ellipse(color,
			ellipse_geometry,
			context.transform,
			graphics);
	    }
	});
    }

}
