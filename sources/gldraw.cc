#include "gldraw.h"
#include <string>

struct vert {
  float x;
  float y;
  float z;
};
float vcomp = 1;
size_t vert_cnt = 0;
bool readytodraw;
GLuint VAO;
GLuint VBO;
GLuint prog;

void realize(Gtk::GLArea* area, std::vector<std::vector<float>>& calculations) {
  std::cout << "realization was called" << std::endl;
  readytodraw = false;
  if (calculations.empty()) {
    return;
  }
  area->make_current();
  vert_cnt = calculations.size() * (calculations[0].size() + 1);
  vert verts[vert_cnt];
  for (size_t i = 0; i < calculations.size(); i++) {
    for (size_t j = 0; j < calculations[i].size(); j++) {
      verts[i * calculations[i].size() + j].x = i/calculations.size();
      verts[i * calculations[i].size() + j].y = j/calculations[i].size();
      verts[i * calculations[i].size() + j].z = calculations[i][j];
      if (std::abs(verts[i * calculations[i].size() + j].z) > vcomp) {
        vcomp = std::abs(verts[i * calculations[i].size() + j].z);
      }
    }
  }
  // store buffers in vao
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  // vbo
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
  // vaa
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(vert), (void*)0);
  // program
  prog = glCreateProgram();
  std::ifstream ifs;
  ifs.open("sources/graph.vert");
  if (!ifs.is_open()) {
    std::cout << "failed to open vert shader" << std::endl;
    return;
  }
  std::string vertShaderF;
  std::getline(ifs, vertShaderF, '\0');
  ifs.close();
  const char* vertshader = vertShaderF.c_str();
  GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vshader, 1, &vertshader, nullptr);
  glCompileShader(vshader);
  int successv = 0;
  char complogv[512];
  glGetShaderiv(vshader, GL_COMPILE_STATUS, &successv);
  if (!successv) {
    glGetShaderInfoLog(vshader, 512, nullptr, complogv);
    std::cout << "failed to compile vshader: " << complogv << std::endl <<
      "shader source is ass follows: " << std::endl << vertshader;
  }
  glAttachShader(prog, vshader);
  ifs.open("sources/graph.frag");
  if (!ifs.is_open()) {
    std::cout << "failed to open frag shared" << std::endl;
    return;
  }
  std::string fragShaderF;
  std::getline(ifs, fragShaderF, '\0');
  ifs.close();
  const char* fragshader = fragShaderF.c_str();
  GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fshader, 1, &fragshader, nullptr);
  glCompileShader(fshader);
  int successf = 0;
  char complogf[512];
  glGetShaderiv(fshader, GL_COMPILE_STATUS, &successf);
  if (!successf) {
    glGetShaderInfoLog(fshader, 512, nullptr, complogf);
    std::cout << "failed to compile fshader: " << complogf << std::endl <<
      "shader source is ass follows: " << std::endl << fragshader;

  }
  glAttachShader(prog, fshader);
  glLinkProgram(prog);
  // compress the graph
  vcomp = 5;
  glUniform1f(glGetUniformLocation(prog, "vcomp"), vcomp);
  // cleanup
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glDeleteBuffers(1, &VAO);
  glDeleteShader(vshader);
  glDeleteShader(fshader);
  std::cout << "readytodraw is set" << std::endl;
  readytodraw = true;
}

void unrealize(Gtk::GLArea* area) {
}

bool draw(const Glib::RefPtr<Gdk::GLContext>& context) {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  if (!readytodraw) {
    std::cout << "not ready to draw" << std::endl;
    return false;
  }
  context->make_current();
  glUseProgram(prog);
  glUniform1f(glGetUniformLocation(prog, "vcomp"), vcomp);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, vert_cnt);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glUseProgram(0);
  return true;
}
