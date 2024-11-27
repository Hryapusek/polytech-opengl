import math
import random
import numpy as np
import pygame
from OpenGL.GL import *
from OpenGL.GLU import *
from pygame.locals import *

# Constants
SCREEN_WIDTH, SCREEN_HEIGHT = 1800, 1600
BACKGROUND_COLOR = (0.02, 0.02, 0.1, 1)

# Emitting particles from the sphere's center
EMITTER_POSITION = (0, 0, 0)  # Sphere at the origin
EMITTER_RADIUS = 1  # Radius of the sphere for particle emission

# Particle constants
MIN_PARTICLES = 200  # Increased number of particles
MAX_PARTICLES = 500  # Increased number of particles
LIFETIME = 10
PARTICLE_SIZE = 1
TRACE_LENGTH_MIN = 1
TRACE_LENGTH_MAX = 2
GRAVITY = np.array([0, -0.01, 0])

# Cube for bump figure
CUBE_SIZE = 2
CUBE_POSITION = (0, 10 + CUBE_SIZE / 2, 0)


class Particle:
    def __init__(self, start_position):
        self.position = np.array(start_position, dtype=float)
        self.velocity = self.calculate_initial_velocity()
        self.acceleration = GRAVITY
        self.size = PARTICLE_SIZE
        self.age = 0
        self.trace_length = random.randint(TRACE_LENGTH_MIN, TRACE_LENGTH_MAX)
        self.trail = []

    def calculate_initial_velocity(self):
        # Calculate the direction from the emitter (sphere) to the cube
        direction_to_cube = (
            np.array([CUBE_POSITION[0], CUBE_POSITION[1], CUBE_POSITION[2]])
            - self.position
        )
        direction_to_cube /= np.linalg.norm(direction_to_cube)  # Normalize the vector

        # Add some randomness to the direction
        random_angle = random.uniform(
            -math.pi / 4, math.pi / 4
        )  # Randomize angle slightly
        rotation_axis = np.array(
            [random.uniform(-1, 1), random.uniform(-1, 1), random.uniform(-1, 1)]
        )
        rotation_axis /= np.linalg.norm(rotation_axis)  # Normalize to get a unit vector

        # Apply random rotation to the direction vector
        rotation_matrix = self.rotation_matrix(
            direction_to_cube, random_angle, rotation_axis
        )
        direction_to_cube = np.dot(rotation_matrix, direction_to_cube)

        # Return the velocity scaled by a random speed factor
        return direction_to_cube * random.uniform(3.0, 5.0)

    def rotation_matrix(self, vector, angle, axis):
        # Compute a rotation matrix using the axis-angle formula
        cos_theta = math.cos(angle)
        sin_theta = math.sin(angle)
        ux, uy, uz = axis

        # Rotation matrix components
        rotation_matrix = np.array(
            [
                [
                    cos_theta + ux * ux * (1 - cos_theta),
                    ux * uy * (1 - cos_theta) - uz * sin_theta,
                    ux * uz * (1 - cos_theta) + uy * sin_theta,
                ],
                [
                    uy * ux * (1 - cos_theta) + uz * sin_theta,
                    cos_theta + uy * uy * (1 - cos_theta),
                    uy * uz * (1 - cos_theta) - ux * sin_theta,
                ],
                [
                    uz * ux * (1 - cos_theta) - uy * sin_theta,
                    uz * uy * (1 - cos_theta) + ux * sin_theta,
                    cos_theta + uz * uz * (1 - cos_theta),
                ],
            ]
        )

        return rotation_matrix

    def update(self, delta_time):
        self.apply_gravity(delta_time)
        self.update_position(delta_time)
        self.age += delta_time
        self.update_trail()

    def apply_gravity(self, delta_time):
        self.velocity += self.acceleration * delta_time

    def update_position(self, delta_time):
        self.position += self.velocity * delta_time

    def update_trail(self):
        if len(self.trail) >= self.trace_length:
            self.trail.pop(0)
        self.trail.append(self.position.copy())

    def draw(self):
        self.draw_particle_point()
        self.draw_particle_trail()

    def draw_particle_point(self):
      # Calculate transparency based on age
      alpha = max(0, 1 - self.age / LIFETIME)  # Transparency decreases with time
      glColor4f(1.0, 1.0, 1.0, alpha)  # White color with varying transparency

      # Draw particle as a small round point
      glPointSize(self.size * 3)  # Make particles smaller and more subtle
      glBegin(GL_POINTS)
      glVertex3fv(self.position)
      glEnd()

    def draw_particle_trail(self):
        # Draw the trail with GL_QUAD_STRIP for thickness but add color fading
        glBegin(GL_QUAD_STRIP)
        for i in range(len(self.trail) - 1):
            pos1 = self.trail[i]
            pos2 = self.trail[i + 1]

            # Perpendicular vector for trail thickness (keep it slightly thinner)
            perpendicular = np.cross(self.velocity, [0, 1, 0])
            perpendicular = (
                perpendicular / np.linalg.norm(perpendicular) * self.size * 0.05
            )

            left1 = pos1 - perpendicular
            right1 = pos1 + perpendicular
            left2 = pos2 - perpendicular
            right2 = pos2 + perpendicular

            # Color of the trail will fade from white to transparent as the particle ages
            trail_alpha = max(0, 1 - self.age / LIFETIME)  # Fade the trail as the particle ages
            glColor4f(1.0, 1.0, 1.0, trail_alpha)  # White trail with fading alpha

            glVertex3fv(left1)
            glVertex3fv(right1)
            glVertex3fv(left2)
            glVertex3fv(right2)
        glEnd()



def create_particle():
    # Random position inside the sphere
    theta = random.uniform(0, 2 * math.pi)
    phi = random.uniform(0, math.pi)
    r = random.uniform(0, EMITTER_RADIUS)

    x = EMITTER_POSITION[0] + r * math.sin(phi) * math.cos(theta)
    y = EMITTER_POSITION[1] + r * math.cos(phi)
    z = EMITTER_POSITION[2] + r * math.sin(phi) * math.sin(theta)

    return Particle((x, y, z))


def initialize_particles():
    particles = [create_particle() for _ in range(MIN_PARTICLES)]
    return particles


def update_particles(particles, delta_time):
    for particle in particles[:]:
        particle.update(delta_time)
        handle_particle_collision(particle)
        if particle.age > LIFETIME:
            particles.remove(particle)
            particles.append(create_particle())


def handle_particle_collision(particle):
    # Cube collision detection and handling (improved bounce)
    min_x, max_x = CUBE_POSITION[0] - CUBE_SIZE / 2, CUBE_POSITION[0] + CUBE_SIZE / 2
    min_y, max_y = CUBE_POSITION[1] - CUBE_SIZE / 2, CUBE_POSITION[1] + CUBE_SIZE / 2
    min_z, max_z = CUBE_POSITION[2] - CUBE_SIZE / 2, CUBE_POSITION[2] + CUBE_SIZE / 2

    # Debugging: Print particle position and cube bounds
    print(f"Particle Position: {particle.position}")
    print(f"Cube Bounds: X({min_x}, {max_x}), Y({min_y}, {max_y}), Z({min_z}, {max_z})")

    # Check for collision with the cube's faces
    if (
        min_x <= particle.position[0] <= max_x
        and min_y <= particle.position[1] <= max_y
        and min_z <= particle.position[2] <= max_z
    ):

        margin = 0.05
        # Reflect velocity off each of the 6 cube faces
        if min_x - margin <= particle.position[0] <= max_x + margin:
            particle.velocity[0] = -particle.velocity[0]  # Reflect along x-axis
        if min_y - margin <= particle.position[1] <= max_y + margin:
            particle.velocity[1] = -particle.velocity[1]  # Reflect along y-axis
        if min_z - margin <= particle.position[2] <= max_z + margin:
            particle.velocity[2] = -particle.velocity[2]  # Reflect along z-axis


def draw_cube():
    # Duller color for the cube (grayish)
    glColor3f(0.5, 0.5, 0.5)  # Light gray color
    glPushMatrix()
    glTranslatef(CUBE_POSITION[0], CUBE_POSITION[1], CUBE_POSITION[2])

    # Draw the faces of the cube
    glBegin(GL_QUADS)
    for face in [
        # Front face
        [(1, 1, -1), (-1, 1, -1), (-1, -1, -1), (1, -1, -1)],
        # Back face
        [(1, 1, 1), (1, -1, 1), (-1, -1, 1), (-1, 1, 1)],
        # Left face
        [(-1, 1, 1), (-1, -1, 1), (-1, -1, -1), (-1, 1, -1)],
        # Right face
        [(1, 1, -1), (1, -1, -1), (1, -1, 1), (1, 1, 1)],
        # Top face
        [(1, 1, 1), (-1, 1, 1), (-1, 1, -1), (1, 1, -1)],
        # Bottom face
        [(1, -1, -1), (-1, -1, -1), (-1, -1, 1), (1, -1, 1)],
    ]:
        for vertex in face:
            glVertex3f(*[CUBE_SIZE * x for x in vertex])
    glEnd()

    # Draw frames around the cube faces
    glColor3f(0, 0, 0)  # Black color for cube edges (frames)
    glBegin(GL_LINES)
    for face in [
        # Front face
        [(1, 1, -1), (-1, 1, -1), (-1, -1, -1), (1, -1, -1)],
        # Back face
        [(1, 1, 1), (1, -1, 1), (-1, -1, 1), (-1, 1, 1)],
        # Left face
        [(-1, 1, 1), (-1, -1, 1), (-1, -1, -1), (-1, 1, -1)],
        # Right face
        [(1, 1, -1), (1, -1, -1), (1, -1, 1), (1, 1, 1)],
        # Top face
        [(1, 1, 1), (-1, 1, 1), (-1, 1, -1), (1, 1, -1)],
        # Bottom face
        [(1, -1, -1), (-1, -1, -1), (-1, -1, 1), (1, -1, 1)],
    ]:
        # Draw edges for each face by connecting vertices
        for i in range(len(face)):
            start = np.array(face[i]) * CUBE_SIZE
            end = np.array(face[(i + 1) % len(face)]) * CUBE_SIZE
            glVertex3fv(start)
            glVertex3fv(end)
    glEnd()

    glPopMatrix()



def draw_sphere():
    # Draw the sphere at the emitter position
    glPushMatrix()
    glTranslatef(EMITTER_POSITION[0], EMITTER_POSITION[1], EMITTER_POSITION[2])

    # Set color of the sphere (e.g., blue)
    glColor3f(1, 1, 1)

    # Use gluSphere to draw the sphere
    sphere = gluNewQuadric()
    gluQuadricDrawStyle(sphere, GLU_FILL)
    gluSphere(sphere, EMITTER_RADIUS, 32, 32)  # 32 slices and 32 stacks for smoothness
    gluDeleteQuadric(sphere)

    glPopMatrix()


def initialize_pygame():
    pygame.init()
    pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT), DOUBLEBUF | OPENGL)
    pygame.display.set_caption("course work")
    gluPerspective(45, (SCREEN_WIDTH / SCREEN_HEIGHT), 0.1, 50.0)
    glTranslatef(0.0, 0, -30)
    glRotatef(40, 0, 1, 0)
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    return pygame.time.Clock()


def main_loop():
    clock = initialize_pygame()
    particles = initialize_particles()
    running = True

    while running:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glClearColor(*BACKGROUND_COLOR)

        delta_time = clock.get_time() / 1000

        for event in pygame.event.get():
            if event.type == QUIT:
                running = False

        # Draw the sphere at the emitter position
        draw_sphere()
        
        # Draw the cube above the sphere
        draw_cube()

        # Update and draw particles
        update_particles(particles, delta_time)
        for particle in particles:
            particle.draw()

        pygame.display.flip()
        clock.tick(60)

    pygame.quit()

if __name__ == "__main__":
    main_loop()
