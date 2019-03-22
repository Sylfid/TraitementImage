#!/usr/bin/env python3
"""
Python OpenGL practical application.
"""
# Python built-in modules
import os                           # os function, i.e. checking file status

# External, non built-in modules
import OpenGL.GL as GL              # standard Python OpenGL wrapper
import glfw                         # lean window system wrapper for OpenGL
import numpy as np                  # all matrix manipulations & OpenGL args
from transform import translate, rotate, scale, vec, perspective, Trackball, identity


# ------------ low level OpenGL object wrappers ----------------------------
class Shader:
    """ Helper class to create and automatically destroy shader program """
    @staticmethod
    def _compile_shader(src, shader_type):
        src = open(src, 'r').read() if os.path.exists(src) else src
        src = src.decode('ascii') if isinstance(src, bytes) else src
        shader = GL.glCreateShader(shader_type)
        GL.glShaderSource(shader, src)
        GL.glCompileShader(shader)
        status = GL.glGetShaderiv(shader, GL.GL_COMPILE_STATUS)
        src = ('%3d: %s' % (i+1, l) for i, l in enumerate(src.splitlines()))
        if not status:
            log = GL.glGetShaderInfoLog(shader).decode('ascii')
            GL.glDeleteShader(shader)
            src = '\n'.join(src)
            print('Compile failed for %s\n%s\n%s' % (shader_type, log, src))
            return None
        return shader

    def __init__(self, vertex_source, fragment_source):
        """ Shader can be initialized with raw strings or source file names """
        self.glid = None
        vert = self._compile_shader(vertex_source, GL.GL_VERTEX_SHADER)
        frag = self._compile_shader(fragment_source, GL.GL_FRAGMENT_SHADER)
        if vert and frag:
            self.glid = GL.glCreateProgram()  # pylint: disable=E1111
            GL.glAttachShader(self.glid, vert)
            GL.glAttachShader(self.glid, frag)
            GL.glLinkProgram(self.glid)
            GL.glDeleteShader(vert)
            GL.glDeleteShader(frag)
            status = GL.glGetProgramiv(self.glid, GL.GL_LINK_STATUS)
            if not status:
                print(GL.glGetProgramInfoLog(self.glid).decode('ascii'))
                GL.glDeleteProgram(self.glid)
                self.glid = None

    def __del__(self):
        GL.glUseProgram(0)
        if self.glid:                      # if this is a valid shader object
            GL.glDeleteProgram(self.glid)  # object dies => destroy GL object


# ------------  Simple color shaders ------------------------------------------
COLOR_VERT = """#version 330 core
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertex_colors;
out vec3 fragment_color;
void main() {
    gl_Position = proj * view * model * vec4(position, 1);
    fragment_color = vertex_colors;
}"""

COLOR_FRAG = """#version 330 core
out vec4 outColor;
in vec3 fragment_color;
void main() {
    outColor = vec4(fragment_color, 1);
}"""

#
# # ------------  Scene object classes ------------------------------------------
# class SimpleTriangle:
#     """Hello triangle object"""
#
#     def __init__(self):
#
#         # triangle position buffer
#         position = np.array(((0, .5, 0), (.5, -.5, 0), (-.5, -.5, 0)), 'f')
#
#         vertex_colors = np.array(((1, 0, 0), (0, 1, 0), (0, 0, 1), 'f'))
#
#         self.glid = GL.glGenVertexArrays(1)  # create OpenGL vertex array id
#         GL.glBindVertexArray(self.glid)      # activate to receive state below
#
#         # bind the vbo, upload position data to GPU, declare its size and type
#         self.buffers = [GL.glGenBuffers(1)]  # create buffer for position attrib
#         GL.glEnableVertexAttribArray(0)      # assign to layout = 1 attribute
#         GL.glBindBuffer(GL.GL_ARRAY_BUFFER, self.buffers[0])
#         GL.glBufferData(GL.GL_ARRAY_BUFFER, position, GL.GL_STATIC_DRAW)
#         GL.glVertexAttribPointer(0, 3, GL.GL_FLOAT, False, 0, None)
#
#         # Q4
#         self.buffers += [GL.glGenBuffers(1)]  # create buffer for position attrib
#         GL.glEnableVertexAttribArray(1)      # assign to layout = 1 attribute
#         GL.glBindBuffer(GL.GL_ARRAY_BUFFER, self.buffers[1])
#         GL.glBufferData(GL.GL_ARRAY_BUFFER, vertex_colors, GL.GL_STATIC_DRAW)
#         GL.glVertexAttribPointer(1, 3, GL.GL_FLOAT, False, 0, None)
#
#         # cleanup and unbind so no accidental subsequent state update
#         GL.glBindVertexArray(0)
#         GL.glBindBuffer(GL.GL_ARRAY_BUFFER, 0)
#
#     def draw(self, projection, view, model, color_shader, color=(0, 0, 0)):
#         GL.glUseProgram(color_shader.glid)
#         #Q2
#         #my_color_location = GL.glGetUniformLocation(color_shader.glid, 'color')
#         # draw triangle as GL_TRIANGLE vertex array, draw array call
#         GL.glBindVertexArray(self.glid)
#         GL.glDrawArrays(GL.GL_TRIANGLES, 0, 3)
#         GL.glBindVertexArray(0)
#
#         proj_location = GL.glGetUniformLocation(color_shader.glid, 'proj')
#         GL.glUniformMatrix4fv(proj_location, 1, True, projection)
#
#         view_location = GL.glGetUniformLocation(color_shader.glid, 'view')
#         GL.glUniformMatrix4fv(view_location, 1, True, view)
#
#         matrix_location = GL.glGetUniformLocation(color_shader.glid, 'model')
#         GL.glUniformMatrix4fv(matrix_location, 1, True, model)
#
#     def __del__(self):
#         GL.glDeleteVertexArrays(1, [self.glid])
#         GL.glDeleteBuffers(1, self.buffers)
#
#
class VertexArray:
    """ helper class to create and self destroy OpenGL vertex array objects. """
    def __init__(self, attributes, index=None, usage=GL.GL_STATIC_DRAW):
        """ Vertex array from attributes and optional index array. Vertex
            Attributes should be list of arrays with one row per vertex.
            same with att_colors"""

        # create vertex array object, bind it
        self.glid = GL.glGenVertexArrays(1)
        GL.glBindVertexArray(self.glid)
        self.buffers = []  # we will store buffers in a list
        nb_primitives, size = 0, 0

        # load buffer per vertex attribute (in list with index = shader layout)
        for loc, data in enumerate(attributes):
            if data is not None:
                # bind a new vbo, upload its data to GPU, declare size and type
                self.buffers += [GL.glGenBuffers(1)]
                data = np.array(data, np.float32, copy=False)  # ensure format
                nb_primitives, size = data.shape
                GL.glEnableVertexAttribArray(loc)
                GL.glBindBuffer(GL.GL_ARRAY_BUFFER, self.buffers[-1])
                GL.glBufferData(GL.GL_ARRAY_BUFFER, data, usage)
                GL.glVertexAttribPointer(loc, size, GL.GL_FLOAT, False, 0, None)

        # optionally create and upload an index buffer for this object
        self.draw_command = GL.glDrawArrays
        self.arguments = (0, nb_primitives)
        if index is not None:
            self.buffers += [GL.glGenBuffers(1)]
            index_buffer = np.array(index, np.int32, copy=False)  # good format
            GL.glBindBuffer(GL.GL_ELEMENT_ARRAY_BUFFER, self.buffers[-1])
            GL.glBufferData(GL.GL_ELEMENT_ARRAY_BUFFER, index_buffer, usage)
            self.draw_command = GL.glDrawElements
            self.arguments = (index_buffer.size, GL.GL_UNSIGNED_INT, None)

        # cleanup and unbind so no accidental subsequent state update
        GL.glBindVertexArray(0)
        GL.glBindBuffer(GL.GL_ARRAY_BUFFER, 0)
        GL.glBindBuffer(GL.GL_ELEMENT_ARRAY_BUFFER, 0)

    def execute(self, projection, view, model, color_shader, primitive):
        """ draw a vertex array, either as direct array or indexed array """
        GL.glUseProgram(color_shader.glid)

        proj_location = GL.glGetUniformLocation(color_shader.glid, 'proj')
        GL.glUniformMatrix4fv(proj_location, 1, True, projection)

        view_location = GL.glGetUniformLocation(color_shader.glid, 'view')
        GL.glUniformMatrix4fv(view_location, 1, True, view)

        matrix_location = GL.glGetUniformLocation(color_shader.glid, 'model')
        GL.glUniformMatrix4fv(matrix_location, 1, True, model)

        # # draw triangle as GL_TRIANGLE vertex array, draw array call
        # GL.glBindVertexArray(self.glid)
        # GL.glDrawElements(GL.GL_TRIANGLES, self.index.size, GL.GL_UNSIGNED_INT, None)  # 6 triangles, 6 points
        # GL.glBindVertexArray(0)
        #
        GL.glBindVertexArray(self.glid)
        self.draw_command(primitive, *self.arguments)
        GL.glBindVertexArray(0)

    def __del__(self):  # object dies => kill GL array and buffers from GPU
        GL.glDeleteVertexArrays(1, [self.glid])
        GL.glDeleteBuffers(len(self.buffers), self.buffers)

# ------------  Scene object classes ------------------------------------------
class Pyramid:
    """Hello pyramid object (a 5 triangle pyramid with square base)"""

    def __init__(self):


        # triangles position buffer
        position = np.array(((0, 0, 0),
                             (0, 1, 0),
                             (1, 1, 0),
                             (1, 0, 0),
                             (.5, .5, 1)), np.float32)
        self.index = np.array((0, 1, 2, #base one
                               0, 2, 3, #base two
                               0, 1, 4,
                               1, 2, 4,
                               2, 3, 4,
                               3, 0, 4), np.uint32)
        #super().__init__(position, index)

        vertex_colors = np.array(((1, 0, 0),
                                  (.5, .5, 0),
                                  (0, 0, 1),
                                  (0, .5, .5),
                                  (1, 1, 1)), np.float32)

        self.vert_arr = VertexArray([position, vertex_colors], self.index)
        # self.glid = GL.glGenVertexArrays(1)  # create OpenGL vertex array id
        # GL.glBindVertexArray(self.glid)      # activate to receive state below
        #
        # # bind the vbo, upload position data to GPU, declare its size and type
        # self.buffers = [GL.glGenBuffers(1)]  # create buffer for position attrib
        # GL.glEnableVertexAttribArray(0)      # assign to layout = 0 attribute
        # GL.glBindBuffer(GL.GL_ARRAY_BUFFER, self.buffers[0])
        # GL.glBufferData(GL.GL_ARRAY_BUFFER, position, GL.GL_STATIC_DRAW)
        # GL.glVertexAttribPointer(0, 3, GL.GL_FLOAT, False, 0, None)
        #
        # # Q4
        # self.buffers += [GL.glGenBuffers(1)]  # create buffer for position attrib
        # GL.glEnableVertexAttribArray(1)      # assign to layout = 1 attribute
        # GL.glBindBuffer(GL.GL_ARRAY_BUFFER, self.buffers[1])
        # GL.glBufferData(GL.GL_ARRAY_BUFFER, vertex_colors, GL.GL_STATIC_DRAW)
        # GL.glVertexAttribPointer(1, 3, GL.GL_FLOAT, False, 0, None)
        #
        # # create GPU index buffer
        # self.buffers += [GL.glGenBuffers(1)]
        # # make it active to receive
        # GL.glBindBuffer(GL.GL_ELEMENT_ARRAY_BUFFER, self.buffers[-1])
        # # our index array here
        # GL.glBufferData(GL.GL_ELEMENT_ARRAY_BUFFER, self.index, GL.GL_STATIC_DRAW)
        #
        # # cleanup and unbind so no accidental subsequent state update
        # GL.glBindVertexArray(0)
        # GL.glBindBuffer(GL.GL_ARRAY_BUFFER, 0)

    def draw(self, projection, view, model, color_shader, color=(0, 0, 0)):
        # GL.glUseProgram(color_shader.glid)
        # #Q2
        # #my_color_location = GL.glGetUniformLocation(color_shader.glid, 'color')
        #
        # proj_location = GL.glGetUniformLocation(color_shader.glid, 'proj')
        # GL.glUniformMatrix4fv(proj_location, 1, True, projection)
        #
        # view_location = GL.glGetUniformLocation(color_shader.glid, 'view')
        # GL.glUniformMatrix4fv(view_location, 1, True, view)
        #
        # matrix_location = GL.glGetUniformLocation(color_shader.glid, 'model')
        # GL.glUniformMatrix4fv(matrix_location, 1, True, model)
        #
        # # draw triangle as GL_TRIANGLE vertex array, draw array call
        # GL.glBindVertexArray(self.glid)
        # GL.glDrawElements(GL.GL_TRIANGLES, self.index.size, GL.GL_UNSIGNED_INT, None)  # 6 triangles, 6 points
        # GL.glBindVertexArray(0)
        self.vert_arr.execute(projection, view, model, color_shader, GL.GL_TRIANGLES)


    #
    # def __del__(self):
    #     GL.glDeleteVertexArrays(1, [self.glid])
    #     GL.glDeleteBuffers(1, self.buffers)




# ------------  Viewer class & window management ------------------------------
class Viewer:
    """ GLFW viewer window, with classic initialization & graphics loop """

    def __init__(self, width=640, height=480, color=(0, 0, 0)):

        # version hints: create GL window with >= OpenGL 3.3 and core profile
        glfw.window_hint(glfw.CONTEXT_VERSION_MAJOR, 3)
        glfw.window_hint(glfw.CONTEXT_VERSION_MINOR, 3)
        glfw.window_hint(glfw.OPENGL_FORWARD_COMPAT, GL.GL_TRUE)
        glfw.window_hint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)
        glfw.window_hint(glfw.RESIZABLE, False)
        self.win = glfw.create_window(width, height, 'Viewer', None, None)

        # make win's OpenGL context current; no OpenGL calls can happen before
        glfw.make_context_current(self.win)


        # register event handlers
        glfw.set_key_callback(self.win, self.on_key)

        # useful message to check OpenGL renderer characteristics
        print('OpenGL', GL.glGetString(GL.GL_VERSION).decode() + ', GLSL',
              GL.glGetString(GL.GL_SHADING_LANGUAGE_VERSION).decode() +
              ', Renderer', GL.glGetString(GL.GL_RENDERER).decode())

        # initialize GL by setting viewport and default render characteristics
        GL.glClearColor(0.1, 0.1, 0.1, 0.1)
        GL.glEnable(GL.GL_DEPTH_TEST)
        #GL.glEnable(GL.GL_CULL_FACE)

        # compile and initialize shader programs once globally
        self.color_shader = Shader(COLOR_VERT, COLOR_FRAG)

        # initially empty list of object to draw
        self.drawables = []

        #init color
        self.color = color

        #angle de rotation
        self.angle_hori = 0
        self.angle_verti = 0

        #trackball
        self.trackball = GLFWTrackball(self.win)

    def run(self):
        """ Main render loop for this OpenGL window """
        while not glfw.window_should_close(self.win):
            # clear draw buffer
            GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT)

            #trackball
            winsize = glfw.get_window_size(self.win)
            view = self.trackball.view_matrix()
            projection = self.trackball.projection_matrix(winsize)


            # draw our scene objects
            for drawable in self.drawables:
                drawable.draw(projection, view, identity(), self.color_shader)
                # drawable.draw(perspective(35, 1.333, 0.1, 10),
                #               translate(0, 0, -5),
                #               rotate(vec(0, 1, 0), self.angle_hori)
                #               @ rotate(vec(1, 0, 0), self.angle_verti),
                #               self.color_shader,
                #               self.color)

            # flush render commands, and swap draw buffers
            glfw.swap_buffers(self.win)

            # Poll for and process events
            glfw.poll_events()

    def add(self, *drawables):
        """ add objects to draw in this window """
        self.drawables.extend(drawables)

    def on_key(self, _win, key, _scancode, action, _mods):
        """ 'Q' or 'Escape' quits """
        if action == glfw.PRESS or action == glfw.REPEAT:
            if key == glfw.KEY_ESCAPE or key == glfw.KEY_Q:
                glfw.set_window_should_close(self.win, True)
            elif key == glfw.KEY_RIGHT:
                self.angle_hori += 5
            elif key == glfw.KEY_LEFT:
                self.angle_hori -= 5
            elif key == glfw.KEY_UP:
                self.angle_verti += 5
            elif key == glfw.KEY_DOWN:
                self.angle_verti -= 5


class GLFWTrackball(Trackball):
    """ Use in Viewer for interactive viewpoint control """

    def __init__(self, win):
        """ Init needs a GLFW window handler 'win' to register callbacks """
        super().__init__()
        self.mouse = (0, 0)
        glfw.set_cursor_pos_callback(win, self.on_mouse_move)
        glfw.set_scroll_callback(win, self.on_scroll)

    def on_mouse_move(self, win, xpos, ypos):
        """ Rotate on left-click & drag, pan on right-click & drag """
        old = self.mouse
        self.mouse = (xpos, glfw.get_window_size(win)[1] - ypos)
        if glfw.get_mouse_button(win, glfw.MOUSE_BUTTON_LEFT):
            self.drag(old, self.mouse, glfw.get_window_size(win))
        if glfw.get_mouse_button(win, glfw.MOUSE_BUTTON_RIGHT):
            self.pan(old, self.mouse)

    def on_scroll(self, win, _deltax, deltay):
        """ Scroll controls the camera distance to trackball center """
        self.zoom(deltay, glfw.get_window_size(win)[1])



# -------------- main program and scene setup --------------------------------
def main():
    """ create a window, add scene objects, then run rendering loop """
    viewer = Viewer(color=(1, 1, 0))

    # place instances of our basic objects
    viewer.add(Pyramid())

    # start rendering loop
    viewer.run()


if __name__ == '__main__':
    glfw.init()                # initialize window system glfw
    main()                     # main function keeps variables locally scoped
    glfw.terminate()           # destroy all glfw windows and GL contexts
