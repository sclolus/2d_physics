use crate::object::Object;

use cgmath::Vector2;
type Vec2 = Vector2<f64>;
pub const GRAVITATIONAL_CONSTANT: f64 = 6.67430e-11f64;

pub struct Universe {
    pub objects: Vec<Object>,
    pub time_ratio: f64,
    pub window_size: piston_window::Size, // Yeah that's ugly, but fuck it, I'll fix that later
}

impl Universe {    
    fn populate(&mut self, number_of_particles: u64) {
	let mut objects = Vec::new();
	let half_width = self.window_size.width as f64 * 100.0;
	let half_height = self.window_size.height as f64 * 100.0;
	
	for _ in 0..number_of_particles {
	    objects.push(Object::random_object(100.0..500.0,
					       1e15..1e20,
					       [-half_width..half_width, -half_height..half_height],
					       [0.0..30.0, 0.0..30.0],
					       [0.0..30.0, 0.0..30.0]))
	}

	self.objects = objects;
    }
    
    pub fn new(size: piston_window::Size, number_of_particles: u64) -> Self {	
	let mut universe = Universe {
	    objects: Vec::new(),
	    time_ratio: 1.0,
	    window_size: size,
	};

	universe.populate(number_of_particles);
	universe
    }

    pub fn reset(mut self) -> Self {
	use std::convert::TryInto;
	
	let number_of_particles = self.objects.len();
	
	self.populate(number_of_particles.try_into().unwrap());
	self
    }

    pub fn apply_elapsed_time(mut self, elapsed_time: f64) -> Self {
	for object in self.objects.iter_mut() {
	    object.apply_elapsed_time(elapsed_time);
	}
	self
    }

    pub fn apply_gravity(mut self) -> Self {
	for object in self.objects.iter_mut() {
	    object.reset_forces();
	}

	let clone = self.objects.clone();
	
	for (i, object) in self.objects.iter_mut().enumerate() {
	    for (u, attractor) in clone.iter().enumerate() {
		if i != u {
		    object.apply_gravity(attractor);
		}
	    }
	}
	self
    }

    pub fn speed_up(&mut self) -> &mut Self {
	self.time_ratio *= 1.05;
	self
    }

    pub fn slow_down(&mut self) -> &mut Self {
	self.time_ratio /= 1.05;
	self
    }

    pub fn bounding_box(&self) -> (Vec2, Vec2) {
	let mut min = Vec2::new(f64::MAX, f64::MAX);
	let mut max = Vec2::new(f64::MIN, f64::MIN);

	for object in self.objects.iter() {
	    min = Vec2::new(object.pos.x.min(min.x), object.pos.y.min(min.y));
	    max = Vec2::new(object.pos.x.max(max.x), object.pos.y.max(max.y));
	}

	(min, max)
    }

}
