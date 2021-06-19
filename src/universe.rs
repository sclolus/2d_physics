use crate::object::Object;

pub struct Universe {
    pub objects: Vec<Object>
}

impl Universe {
    pub fn new(size: piston_window::Size) -> Self {
	let mut objects = Vec::new();
	for _ in 0..100 {
	    objects.push(Object::random_object(10.0..50.0, 1.0..100.0, [0.0..size.width, 0.0..size.height]))
	}
	
	Universe {
	    objects: objects
	}
    }
}
