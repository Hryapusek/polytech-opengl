import math
import random

import numpy as np
import pygame
from OpenGL.GL import *
from OpenGL.GLU import *
from pygame.locals import *

# Константы экрана
SCREEN_WIDTH, SCREEN_HEIGHT = 1800, 1600
BACKGROUND_COLOR = (0.02, 0.02, 0.1, 1)

# Константы эмиттера
EMITTER_POSITION = (0, -5, 0)
EMITTER_SIZE = (3, 1)

# Константы частиц
MIN_PARTICLES = 100
MAX_PARTICLES = 1000
LIFETIME = 10
PARTICLE_SIZE = 1
TRACE_LENGTH = 15
GRAVITY = np.array([0, -0.01, 0])
CONE_CENTER = (0, 3, -10)
CONE_RADIUS = 2
CONE_HEIGHT = 4
PLANE_Y = -4


class Particle:
    def __init__(self, start_position):
        self.position = np.array(start_position, dtype=float)
        self.velocity = self.calculate_initial_velocity()
        self.acceleration = GRAVITY
        self.size = PARTICLE_SIZE
        self.age = 0
        self.trail = []

    def calculate_initial_velocity(self):
        target_position = np.array([
            CONE_CENTER[0] + random.uniform(-CONE_RADIUS, CONE_RADIUS),
            CONE_CENTER[1],
            CONE_CENTER[2]
        ], dtype=float)
        direction = target_position - self.position
        direction /= np.linalg.norm(direction)
        return direction * random.uniform(2.1, 2.8)

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
        if len(self.trail) >= TRACE_LENGTH:
            self.trail.pop(0)
        self.trail.append(self.position.copy())

    def draw(self):
        self.draw_particle_point()
        self.draw_particle_trail()

    def draw_particle_point(self):
        # Расчёт яркости, основанный на расстоянии от плоскости
        distance_from_plane = abs(self.position[1] - PLANE_Y)
        brightness = min(1.0, distance_from_plane / 10)  # Регулируем яркость по расстоянию
        glColor4f(brightness, brightness, brightness, 1.0)  # Устанавливаем цвет с учётом яркости

        glPointSize(self.size * 10)
        glBegin(GL_POINTS)
        glVertex3fv(self.position)
        glEnd()

    def draw_particle_trail(self):
        # Рисуем след с помощью GL_QUAD_STRIP для толщины
        glBegin(GL_QUAD_STRIP)
        for i in range(len(self.trail) - 1):
            pos1 = self.trail[i]
            pos2 = self.trail[i + 1]

            # Вектор перпендикулярный движению для толщины следа
            perpendicular = np.cross(self.velocity, [0, 1, 0])
            perpendicular = perpendicular / np.linalg.norm(perpendicular) * self.size * 0.1

            # Левый и правый край квадрата следа
            left1 = pos1 - perpendicular
            right1 = pos1 + perpendicular
            left2 = pos2 - perpendicular
            right2 = pos2 + perpendicular

            # Используем тот же цвет с учетом яркости
            glColor4f(0.7, 0.7, 0.7, 1.0)

            glVertex3fv(left1)
            glVertex3fv(right1)
            glVertex3fv(left2)
            glVertex3fv(right2)
        glEnd()


def create_particle():
    x = random.uniform(-EMITTER_SIZE[0] / 2, EMITTER_SIZE[0] / 2)
    z = random.uniform(-EMITTER_SIZE[1] / 2, EMITTER_SIZE[1] / 2)  # Ширина по оси z на плоскости
    y = PLANE_Y  # Задаём y так, чтобы частицы начинались на плоскости
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
    distance_from_cone_center = np.linalg.norm(particle.position[:2] - np.array(CONE_CENTER[:2]))
    is_above_cone_base = particle.position[1] >= CONE_CENTER[1]
    if distance_from_cone_center < CONE_RADIUS and is_above_cone_base:
        particle.velocity[1] = -abs(particle.velocity[1])


def draw_plane():
    glPushMatrix()
    glTranslatef(0, PLANE_Y, -10)
    glRotatef(20, 1, 0, 0)
    glRotatef(15, 0, 1, 0)
    glBegin(GL_QUADS)
    glColor3f(1.0, 0.0, 0.0)  # Устанавливаем красный цвет для плоскости
    glVertex3f(-5, 0, -5)
    glVertex3f(5, 0, -5)
    glVertex3f(5, 0, 5)
    glVertex3f(-5, 0, 5)
    glEnd()
    glPopMatrix()


def draw_cone():
    # Рисуем конус с градиентом
    glBegin(GL_TRIANGLE_FAN)

    # Верх конуса (освещенная вершина)
    glColor3f(0.6, 0.6, 1.0)  # Светло-синий для вершины
    glVertex3f(CONE_CENTER[0], CONE_CENTER[1] + CONE_HEIGHT, CONE_CENTER[2])

    # Боковые грани конуса с постепенным затемнением
    for angle in range(0, 361, 10):
        x = CONE_CENTER[0] + CONE_RADIUS * math.cos(math.radians(angle))
        z = CONE_CENTER[2] + CONE_RADIUS * math.sin(math.radians(angle))

        # Переход к более тёмному синему ближе к основанию
        glColor3f(0.0, 0.0, 0.5)  # Темно-синий для основания
        glVertex3f(x, CONE_CENTER[1], z)
    glEnd()


def initialize_pygame():
    pygame.init()
    pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT), DOUBLEBUF | OPENGL)
    pygame.display.set_caption('course work')
    gluPerspective(45, (SCREEN_WIDTH / SCREEN_HEIGHT), 0.1, 50.0)
    glTranslatef(0.0, 0.0, -15)
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

        draw_plane()
        draw_cone()
        update_particles(particles, delta_time)

        for particle in particles:
            particle.draw()

        pygame.display.flip()
        clock.tick(60)

    pygame.quit()


if __name__ == "__main__":
    main_loop()
