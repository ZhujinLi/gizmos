#define GL_SILENCE_DEPRECATION // OpenGL has been deprecated on macOS

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <fstream>
#include <iostream>

// We need to keep the aspect 1 because the calculations in the geometry shader
// assume an isotropic coordinate system.
const int W = 640;
const int H = 640;

GLuint createShader(const char *path, GLenum type) {
  GLuint shader = glCreateShader(type);

  std::ifstream f(path);
  std::string code((std::istreambuf_iterator<char>(f)),
                   (std::istreambuf_iterator<char>()));

  const GLchar *const codeArray[1] = {code.c_str()};
  glShaderSource(shader, 1, codeArray, NULL);

  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status) {
    std::cerr << "Failed to compile shader: " << path << std::endl;

    GLchar infoLog[1024];
    GLsizei _;
    glGetShaderInfoLog(shader, 1024, &_, infoLog);
    std::cerr << infoLog << std::endl;

    glDeleteShader(shader);
    exit(1);
  }

  return shader;
}

GLuint createShaderProgram() {
  GLuint prog = glCreateProgram();

  GLuint vs = createShader("polyline.vert", GL_VERTEX_SHADER);
  glAttachShader(prog, vs);

  GLuint gs = createShader("polyline.geom", GL_GEOMETRY_SHADER);
  glAttachShader(prog, gs);

  GLuint fs = createShader("polyline.frag", GL_FRAGMENT_SHADER);
  glAttachShader(prog, fs);

  glLinkProgram(prog);

  GLint linkStatus;
  glGetProgramiv(prog, GL_LINK_STATUS, &linkStatus);
  if (!linkStatus) {
    GLchar infoLog[1024];
    GLsizei _;
    glGetProgramInfoLog(prog, 1024, &_, infoLog);
    std::cerr << infoLog << std::endl;

    exit(-2);
  }

  glValidateProgram(prog);

  return prog;
}

void render() {
  // We need to append 1 point at the beginning and the end since it's adjacency
  // primitive.
  const int NPOINT = 4 + 2;
  float pointsXy[NPOINT * 2] = {0.0f, 0.0f,  -0.8f, 0.0f, 0.0f, 0.0f,
                                0.3f, -0.5f, 0.8f,  0.0f, 0.0f, 0.0f};
  pointsXy[0] = pointsXy[2] * 2 - pointsXy[4];
  pointsXy[1] = pointsXy[3] * 2 - pointsXy[5];
  pointsXy[10] = pointsXy[8] * 2 - pointsXy[6];
  pointsXy[11] = pointsXy[9] * 2 - pointsXy[7];

  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  GLuint prog = createShaderProgram();
  glUseProgram(prog);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(pointsXy), pointsXy, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glDrawArrays(GL_LINE_STRIP_ADJACENCY, 0, NPOINT);

  glDisableVertexAttribArray(0);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
  glDeleteProgram(prog);
}

int main(void) {
  GLFWwindow *window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(W, H, "Polyline triangulation with geometry shader",
                            NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    render();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
