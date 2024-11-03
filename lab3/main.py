from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
import sys
import math
import numpy as np
from PIL import Image

# Параметры освещения
light_angle = 0
light_position = [0.0, 7.6, 10.0, 1.0]  # Источник света над объектами

light_color = [1.0, 1.0, 1.0, 1.0]
light_intensity = 1

texture_id = None

def load_image(image_path):
    image = Image.open(image_path)
    return image.transpose(Image.FLIP_TOP_BOTTOM)

def initialize_texture(image):
    texture_data = np.array(image.convert("RGBA"), np.uint8)
    glBindTexture(GL_TEXTURE_2D, texture_id)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data)
    glBindTexture(GL_TEXTURE_2D, 0)

def load_texture(image_path):
    global texture_id
    texture_id = glGenTextures(1)
    image = load_image(image_path)
    initialize_texture(image)

def draw_cylinder_surface(radius, height, num_segments):
    glBegin(GL_QUAD_STRIP)
    for i in range(num_segments + 1):
        theta = 2.0 * math.pi * i / num_segments
        x = radius * math.cos(theta)
        z = radius * math.sin(theta)
        glNormal3f(x, 0.0, z)
        glVertex3f(x, height / 2, z)
        glVertex3f(x, -height / 2, z)
    glEnd()

def draw_cylinder_cap(radius, height, num_segments, is_top):
    glBegin(GL_TRIANGLE_FAN)
    glNormal3f(0.0, 1.0 if is_top else -1.0, 0.0)
    glVertex3f(0.0, height / 2 if is_top else -height / 2, 0.0)
    for i in range(num_segments + 1):
        theta = 2.0 * math.pi * i / num_segments
        x = radius * math.cos(theta)
        z = radius * math.sin(theta)
        glVertex3f(x, height / 2 if is_top else -height / 2, z)
    glEnd()

def draw_smooth_cylinder(radius, height, num_segments):
    draw_cylinder_surface(radius, height, num_segments)
    draw_cylinder_cap(radius, height, num_segments, True)
    draw_cylinder_cap(radius, height, num_segments, False)

def draw_textured_sphere(texture_id, radius, num_segments_lat, num_segments_long):
    glEnable(GL_TEXTURE_2D)
    glBindTexture(GL_TEXTURE_2D, texture_id)
    glBegin(GL_QUADS)

    for i in range(-90, 90, num_segments_lat):
        for j in range(0, 360, num_segments_long):
            theta1 = math.radians(i)
            theta2 = math.radians(i + num_segments_lat)
            phi1 = math.radians(j)
            phi2 = math.radians(j + num_segments_long)

            v1 = [math.sin(theta1) * math.cos(phi1), math.cos(theta1), math.sin(theta1) * math.sin(phi1)]
            v2 = [math.sin(theta2) * math.cos(phi1), math.cos(theta2), math.sin(theta2) * math.sin(phi1)]
            v3 = [math.sin(theta2) * math.cos(phi2), math.cos(theta2), math.sin(theta2) * math.sin(phi2)]
            v4 = [math.sin(theta1) * math.cos(phi2), math.cos(theta1), math.sin(theta1) * math.sin(phi2)]

            glNormal3fv(v1)
            glTexCoord2f(0.0, 0.0)
            glVertex3fv(v1)

            glNormal3fv(v2)
            glTexCoord2f(0.6, 0.0)
            glVertex3fv(v2)

            glNormal3fv(v3)
            glTexCoord2f(0.6, 0.6)
            glVertex3fv(v3)

            glNormal3fv(v4)
            glTexCoord2f(0.0, 0.6)
            glVertex3fv(v4)

    glEnd()
    glBindTexture(GL_TEXTURE_2D, 0)
    glDisable(GL_TEXTURE_2D)

def draw_plane():
    glBegin(GL_QUADS)
    glNormal3f(0.0, 0.0, 1.0)
    glColor3f(0, 1, 0)
    glVertex3f(-3.7, -1.0, -1.6)
    glVertex3f(4.3, -1.0, -1.6)
    glVertex3f(4.3, 1.8, -1.6)
    glVertex3f(-3.7, 1.8, -1.6)
    glEnd()

def init_lighting():
    glEnable(GL_LIGHTING)
    glEnable(GL_LIGHT0)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color)
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color)
    glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE)

def init_opengl():
    glClearColor(0.0, 0.0, 0.0, 1.0)
    glMatrixMode(GL_PROJECTION)
    glOrtho(-6.0, 6.0, -2.0, 2.0, 0.1, 50.0)
    glMatrixMode(GL_MODELVIEW)
    init_lighting()
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    load_texture("/home/hryapusek/work/7th_sem/opengl/polytech-opengl/lab3/textures/cone_texture.png")

moving_right = True

def update_light_position():
    global light_position, moving_right
    # Плавное движение источника света
    if moving_right:
        light_position[0] += 0.1  # Двигаемся вправо
        if light_position[0] >= 14.0:  # Если достигли правой границы
            moving_right = False  # Меняем направление на левое
    else:
        light_position[0] -= 0.05  # Двигаемся влево
        if light_position[0] <= -9.0:  # Если достигли левой границы
            moving_right = True  # Меняем направление на правое

    # Обновляем позицию света
    glLightfv(GL_LIGHT0, GL_POSITION, light_position)

def set_lighting_intensity():
    intensity = [c * light_intensity for c in light_color]
    glLightfv(GL_LIGHT0, GL_DIFFUSE, intensity + [1.0])

def configure_material_shiny():
    glMaterialfv(GL_FRONT, GL_SPECULAR, [1.0, 1.0, 1.0, 1.0])
    glMaterialf(GL_FRONT, GL_SHININESS, 128.0)

def configure_material_matte():
    glMaterialfv(GL_FRONT, GL_SPECULAR, [0.0, 0.0, 0.0, 1.0])
    glMaterialfv(GL_FRONT, GL_DIFFUSE, [1.0, 1.0, 0.0, 0.0])
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0)

def render_matte_cylinder():
    glPushMatrix()
    glTranslatef(-3.3, -1.5, -1.0)
    configure_material_matte()
    glColor3f(0.4, 0.4, 0.4)
    glRotatef(27, 0, 0, 0)
    draw_smooth_cylinder(0.5, 1.2, 100)
    glPopMatrix()

def render_shiny_cone():
    glPushMatrix()
    glTranslatef(-1.3, -1.5, -1.0)
    glRotatef(30, 1, 1, 0)
    configure_material_shiny()
    glColor3f(0.2, 0.7, 0.3)
    glutSolidCone(1, 1, 20, 15);
    glPopMatrix()

def render_textured_sphere():
    glPushMatrix()
    glColor4f(1, 1, 1, 0.3)
    glTranslatef(1.2, -1.5, -1.0)
    glRotatef(78, 1, 0, 0)
    glRotatef(14, 0, 1, 0)
    draw_textured_sphere(texture_id, 0.6, 10, 10)
    glPopMatrix()

def create_shadow_matrix(light_pos, plane_normal, point_on_plane):
    d = np.dot(plane_normal, light_pos[:3])  # Берем только первые три компонента
    c = np.dot(plane_normal, point_on_plane)
    shadow_matrix = np.array([[d - light_pos[0] * plane_normal[0], -light_pos[0] * plane_normal[1], -light_pos[0] * plane_normal[2], -light_pos[0] * c],
                              [-light_pos[1] * plane_normal[0], d - light_pos[1] * plane_normal[1], -light_pos[1] * plane_normal[2], -light_pos[1] * c],
                              [-light_pos[2] * plane_normal[0], -light_pos[2] * plane_normal[1], d - light_pos[2] * plane_normal[2], -light_pos[2] * c],
                              [-plane_normal[0], -plane_normal[1], -plane_normal[2], d]])
    return shadow_matrix

def render_shadow():
    shadow_matrix = create_shadow_matrix(light_position, [0.0, 0.0, 1.0], [0.0, 0.0, -1.6])
    
    glPushMatrix()
    glMultMatrixf(shadow_matrix.T)

    # Отключаем освещение и текстуры для отрисовки теней
    glDisable(GL_LIGHTING)
    glDisable(GL_TEXTURE_2D)
    
    # Устанавливаем черный цвет для всех теней
    glColor4f(0.0, 0.0, 0.0, 0.5)

    # Прямо отрисовываем геометрию, но без материалов, чтобы они были черными
    glPushMatrix()
    glTranslatef(-3.3, -0.5, -1.0)
    draw_smooth_cylinder(0.5, 1.2, 100)  # Тень от цилиндра
    glPopMatrix()

    glPushMatrix()
    glTranslatef(-1.0, -0.5, -1.0)
    glRotatef(30, 1, 1, 0)
    glutSolidCone(1, 1, 20, 15);
    glPopMatrix()

    glPushMatrix()
    glTranslatef(1.5, -0.5, -1.0)
    glRotatef(78, 1, 0, 0)
    glRotatef(14, 0, 1, 0)
    glutSolidSphere(1.0, 15, 15)  # Тень от сферы
    glPopMatrix()
    
    # Включаем обратно освещение и текстуры после отрисовки теней
    glEnable(GL_LIGHTING)
    glEnable(GL_TEXTURE_2D)
    
    glPopMatrix()


def display():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()
    gluLookAt(12, 8, 30, 0, 0, 0, 0, 1, 0)
    update_light_position()
    set_lighting_intensity()
    render_matte_cylinder()
    render_shiny_cone()
    render_textured_sphere()

    render_shadow()

    draw_plane()
    glutSwapBuffers()

def update(value):
    glutPostRedisplay()
    glutTimerFunc(16, update, 0)

def main():
    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)
    glutInitWindowSize(1500, 500)
    glutCreateWindow(b"3D Primitives with Moving Light Source and Shiny Torus")
    glEnable(GL_DEPTH_TEST)
    init_opengl()
    glutDisplayFunc(display)
    glutTimerFunc(16, update, 0)
    glutMainLoop()

if __name__ == "__main__":
    main()
