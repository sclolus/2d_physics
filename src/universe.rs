use crate::object::Object;


pub const GRAVITATIONAL_CONSTANT: f64 = 6.67430e-11f64;

pub struct Universe {
    pub objects: Vec<Object>,
}

impl Universe {
    pub fn new(size: piston_window::Size, number_of_particles: u64) -> Self {
	let mut objects = Vec::new();
	for _ in 0..number_of_particles {
	    objects.push(Object::random_object(10.0..50.0,
					       1e15..1e20,
					       [0.0..size.width, 0.0..size.height],
					       [0.0..30.0, 0.0..30.0],
					       [0.0..30.0, 0.0..30.0]))
	}
	
	Universe {
	    objects: objects,
	}
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
}
