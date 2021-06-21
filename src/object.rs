use std::ops::Range;
use rand::Rng;
use cgmath::prelude::*;
use cgmath::Vector2;
use crate::universe;

type Vec2 = Vector2<f64>;


#[derive(Clone, Copy)]
pub struct Object {
    pub radius: f64,
    pub mass: f64,
    pub    pos: Vec2,
    pub velocity: Vec2,
    pub acceleration: Vec2,
    force: Vec2,
}

impl Object {
    #[allow(dead_code)]
    pub fn new() -> Self {
	Object {
	    radius: 10.0,
	    mass: 1.0,
	    pos: Vec2::new(100.0, 100.0),
	    velocity: Vec2::new(0.0, 0.0),
	    acceleration: Vec2::new(0.0, 0.0),
	    force: Vec2::new(0.0, 0.0),
	}
    }

    pub fn random_object(radius: Range<f64>,
			 mass: Range<f64>,
			 position: [Range<f64>; 2],
			 _velocity: [Range<f64>; 2],
			 _acceleration: [Range<f64>; 2]) -> Self {
	let mut rng = rand::thread_rng();
	
	Object {
	    radius: rng.gen_range(radius),
	    mass: rng.gen_range(mass),
	    pos: Vec2::new(
		rng.gen_range(position[0].clone()),
		rng.gen_range(position[1].clone())
	    ),
	    // velocity: Vec2::new(
	    // 	rng.gen_range(velocity[0].clone()),
	    // 	rng.gen_range(velocity[1].clone())
	    // ),
	    // acceleration: Vec2::new(
	    // 	rng.gen_range(acceleration[0].clone()),
	    // 	rng.gen_range(acceleration[1].clone())
	    // ),
	    velocity: Vec2::new(0.0, 0.0),
	    acceleration: Vec2::new(0.0, 0.0),
	    
	    force: Vec2::new(0.0, 0.0),
	}
    }
    

    pub fn reset_forces(&mut self) -> &mut Self {
	self.force = Vec2::new(0.0, 0.0);
	
	self
    }

    pub fn apply_elapsed_time(&mut self, elapsed_time: f64) -> &mut Self {
	self.pos = self.pos + self.velocity * elapsed_time + 1.0 / 2.0 * self.acceleration * elapsed_time * elapsed_time;
	self.velocity = self.velocity + self.acceleration * elapsed_time;

	self
    }


    pub fn apply_gravity(&mut self, attractor: &Self) -> &mut Self {
	let object_to_attractor = attractor.pos - self.pos;
	let distance = object_to_attractor.magnitude().max(self.radius + attractor.radius);
	let force = universe::GRAVITATIONAL_CONSTANT * self.mass * attractor.mass / (distance * distance);

	self.force = self.force + object_to_attractor.normalize_to(force);
	self.acceleration = self.force / self.mass;

	self
    }
}
