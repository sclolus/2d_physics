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

use std::time::{Instant, Duration};


fn main() {
    println!("Hello, world!");
    let mut window: PistonWindow =
        WindowSettings::new("Hello Piston!", [640, 480])
        .exit_on_esc(true).build().unwrap();
    
    let mut universe = Universe::new(window.size(), 300);

    let mut old_time = Instant::now();
    let mut time_ratio = 1.0;
    let mut zoom = 1.0;

    while let Some(event) = window.next() {
	let elapsed_time = old_time.elapsed().as_secs_f64();
	old_time = Instant::now();

	use piston::Button::Keyboard;

	
	match event {
	    Event::Input(Input::Button(ButtonArgs {
		state: ButtonState::Press,
		button: Keyboard(Key::R),
		scancode: _}), _) => universe = Universe::new(window.size(), 300),
	    Event::Input(Input::Button(ButtonArgs {
		state: ButtonState::Press,
		button: Keyboard(Key::Left),
			scancode: _}), _) => time_ratio /= 2.0,
		Event::Input(Input::Button(ButtonArgs {
		state: ButtonState::Press,
		button: Keyboard(Key::Right),
		    scancode: _}), _) => time_ratio *= 2.0,
	    Event::Input(Input::Button(ButtonArgs {
		state: ButtonState::Press,
		button: Keyboard(Key::Down),
		scancode: _}), _) => zoom *= 2.0,

	    Event::Input(Input::Button(ButtonArgs {
		state: ButtonState::Press,
		button: Keyboard(Key::Up),
		scancode: _}), _) => zoom /= 2.0,

	    _ => ()
	}

	universe = universe.apply_elapsed_time(elapsed_time * time_ratio);
	universe = universe.apply_gravity();
	
        window.draw_2d(&event, |context, graphics, _device| {
            clear([1.0; 4], graphics);

	    for object in universe.objects.iter() {
		let mut position_rectangle = object.position_rectangle();
		position_rectangle = [
		    position_rectangle[0] / zoom,
		    position_rectangle[1] / zoom,
		    position_rectangle[2] / zoom,
		    position_rectangle[3] / zoom,
		];
		
		rectangle(object.rectangle().color,
			  position_rectangle,
			  context.transform,
			  graphics);
	    }
        });
    }
}
