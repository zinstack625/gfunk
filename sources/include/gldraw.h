#include "gdkmm/glcontext.h"
#include "glibmm/refptr.h"
#include "gtkmm/glarea.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <fstream>
#include <iostream>

void realize(Gtk::GLArea* area, std::vector<std::vector<float>>& calculations);
void unrealize(Gtk::GLArea* area);
bool draw(const Glib::RefPtr<Gdk::GLContext>& context);
