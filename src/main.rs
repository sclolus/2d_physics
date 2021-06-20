use graphics::*;

extern crate piston_window;
extern crate piston;
// extern crate cgmath;
use cgmath;

use piston_window::*;

mod universe;
mod object;

use universe::Universe;
use object::Object;

use cgmath::prelude::*;
use cgmath::Vector2;

use std::time::{Instant, Duration};


fn main() {
    println!("Hello, world!");
    let mut window: PistonWindow =
        WindowSettings::new("Hello Piston!", [640, 480])
        .exit_on_esc(true).build().unwrap();
    let size = window.size();
    
    let mut universe = Universe::new(window.size(), 300);

    let mut old_time = Instant::now();

    struct Camera {
	pub zoom: f64,
	pub pos: Vector2<f64>,
	pub movement_speed: f64,
    }

    impl Camera {
	pub fn new(size: piston_window::Size) -> Self {
	    Camera {
		zoom: 1.0,
		pos: Vector2::new(size.width / 2.0, size.height / 2.0),
		movement_speed: 100.0,
	    }
	}

	pub fn zoom(&mut self) -> &mut Self {
	    self.zoom *= 2.0;
	    self
	}

	pub fn dezoom(&mut self) -> &mut Self {
	    self.zoom /= 2.0;
	    self
	}

	pub fn move_up(&mut self) -> &mut Self {
	    self.pos -= Vector2::new(0.0, self.movement_speed);
	    self
	}
	
	pub fn move_down(&mut self) -> &mut Self {
	    self.pos += Vector2::new(0.0, self.movement_speed);
	    self
	}

	pub fn move_left(&mut self) -> &mut Self {
	    self.pos -= Vector2::new(self.movement_speed, 0.0);
	    self
	}

	pub fn move_right(&mut self) -> &mut Self {
	    self.pos += Vector2::new(self.movement_speed, 0.0);
	    self
	}
    }

    let mut cam = Camera::new(size);

    while let Some(event) = window.next() {
	let elapsed_time = old_time.elapsed().as_secs_f64();
	old_time = Instant::now();

	use piston::Button::Keyboard;

	match event {
	    Event::Input(Input::Button(ButtonArgs {
		state: ButtonState::Press,
		button: Keyboard(key),
		scancode: _}), _) => {
		match key {
		    Key::R => { universe = universe.reset(); },
		    Key::Left => { universe.slow_down(); },
		    Key::Right => { universe.speed_up(); },
		    Key::Down => { cam.dezoom(); },
		    Key::Up => { cam.zoom(); },
		    Key::W => { cam.move_up(); },
		    Key::A => { cam.move_left(); },
		    Key::S => { cam.move_down(); },
		    Key::D => { cam.move_right(); },
		    _ => (),
		}
	    }
	    _ => ()
	}

	let time_ratio = universe.time_ratio; // Okay technically time_ratio could have been updated in the keyboard match, and that would be wrong to use it as the time_ratio for the current frame. But the universe update and keyboard events should be separated anyway.

	universe = universe.apply_elapsed_time(elapsed_time * time_ratio);
	universe = universe.apply_gravity();



	
        window.draw_2d(&event, |context, graphics, _device| {
            clear([0.0; 4], graphics);

	    for object in universe.objects.iter() {
		let ellipse_geometry = graphics::ellipse::circle((object.pos[0] - cam.pos.x) * cam.zoom, (object.pos[1] - cam.pos.y) * cam.zoom, object.radius * cam.zoom);
		
		ellipse(object.rectangle().color,
			ellipse_geometry,
			context.transform,
			graphics);
	    }
        });
    }
}
