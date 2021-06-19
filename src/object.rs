use graphics::math::Vec2d;
use graphics::types::{Color};
use graphics::rectangle::Rectangle;
use graphics::color;
use graphics::types;
use std::ops::Range;
use rand::Rng;
use graphics::math;

pub struct Object {
    radius: f64,
    mass: f64,
    pos: Vec2d,
    velocity: Vec2d,
    acceleration: Vec2d,
    force: Vec2d,
}

impl Object {
    pub fn new() -> Self {
	Object {
	    radius: 10.0,
	    mass: 1.0,
	    pos: [100.0, 100.0],
	    velocity: Vec2d::default(),
	    acceleration: Vec2d::default(),
	    force: Vec2d::default(),
	}
    }

    pub fn random_object(radius: Range<f64>, mass: Range<f64>, position: [Range<f64>; 2]) -> Self {
	let mut rng = rand::thread_rng();
	
	Object {
	    radius: rng.gen_range(radius),
	    mass: rng.gen_range(mass),
	    pos: [
		rng.gen_range(position[0].clone()),
		rng.gen_range(position[1].clone())
	    ],
	    velocity: Vec2d::default(),
	    acceleration: Vec2d::default(),
	    force: Vec2d::default(),
	}
    }
    
    pub fn rectangle(&self) -> Rectangle {
	Rectangle::new(color::GREEN)
    }

    pub fn position_rectangle(&self) -> types::Rectangle {
	[
	    self.pos[0] - self.radius,
	    self.pos[1] - self.radius,
	    self.radius,
	    self.radius
	]
    }

    pub fn apply_elasped_time(self, elapsed_time: f64) -> Self {
	Self {
	    pos: math::add(self.pos, math::mul_scalar(self.velocity, elapsed_time)),
	    .. self
	}
    }
}
