use graphics::*;

extern crate piston_window;

use piston_window::*;

mod universe;
mod object;

use universe::Universe;
use object::Object;


fn main() {
    println!("Hello, world!");
    let mut window: PistonWindow =
        WindowSettings::new("Hello Piston!", [640, 480])
        .exit_on_esc(true).build().unwrap();
    
    let mut universe = Universe::new(window.size());


    while let Some(event) = window.next() {
	use piston::Button::Keyboard;

	
	match event {
	    Event::Input(Input::Button(ButtonArgs {
		state: ButtonState::Press,
		button: Keyboard(Key::R),
		scancode: _}), _) => universe = Universe::new(window.size()),
	    _ => ()
	}
	
        window.draw_2d(&event, |context, graphics, _device| {
            clear([1.0; 4], graphics);

	    for object in universe.objects.iter() {
		rectangle(object.rectangle().color,
			  object.position_rectangle(),
			  context.transform,
			  graphics);
	    }
        });
    }
}
