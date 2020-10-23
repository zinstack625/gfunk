#include "gtkmm/application.h"
#include "gtkmm/builder.h"
#include "gtkmm/editable.h"
#include "gtkmm/entry.h"
#include "gtkmm/glarea.h"
#include "gtkmm/widget.h"
#include "gtkmm/window.h"
#include "gtkmm/windowgroup.h"
#include "parser.h"
#include "rangeparser.h"
#include "sigc++/functors/ptr_fun.h"
#include <cstdlib>
#include <gtkmm.h>
#include <iostream>
#include <utility>

void on_FunctionInput_changed(const Gtk::Entry *FunctionInput,
                              Expression *function) {
  function->set_function(FunctionInput->get_text());
}
void on_RangeInput_changed(const Gtk::Entry *RangeInput,
                           std::pair<double, double> *range) {
  *range = rangeparser(RangeInput->get_text());
}

struct Ranges {
  std::pair<double, double> xrange;
  std::pair<double, double> yrange;
  std::pair<double, double> zrange;
};

int main(int argc, char *argv[]) {
  Expression function;
  Ranges func_ranges = {{0, 0}, {0, 0}, {0, 0}};
  auto app = Gtk::Application::create(argc, argv, "GFunk Calculator");
  auto builder = Gtk::Builder::create();
  try {
    builder->add_from_file("calc.glade");
  } catch (const Glib::FileError &ex) {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  } catch (const Glib::MarkupError &ex) {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
  } catch (const Gtk::BuilderError &ex) {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
  }
  Gtk::Window *MainWindow = nullptr;
  builder->get_widget("MainWindow", MainWindow);

  if (MainWindow) {
    Gtk::Widget *CentralDivider = nullptr;
    Gtk::Widget *InputBox = nullptr;
    Gtk::Entry *FunctionInput = nullptr;
    Gtk::Widget *VariableInput = nullptr;
    Gtk::Entry *XInput = nullptr;
    Gtk::Entry *YInput = nullptr;
    Gtk::Entry *ZInput = nullptr;
    Gtk::GLArea *FunctionOutput = nullptr;
    builder->get_widget("CentralDivider", CentralDivider);
    if (CentralDivider) {
      builder->get_widget("InputBox", InputBox);
      if (InputBox) {
        builder->get_widget("FunctionInput", FunctionInput);
        if (FunctionInput) {
          FunctionInput->signal_changed().connect(
              sigc::bind<Gtk::Entry*, Expression*>(sigc::ptr_fun(&on_FunctionInput_changed),
                                       FunctionInput, &function));
        }
        builder->get_widget("VariableInput", VariableInput);
        if (VariableInput) {
          builder->get_widget("XInput", XInput);
          if (XInput) {
            XInput->signal_changed().connect(
                sigc::bind<Gtk::Entry*, std::pair<double, double>*>(sigc::ptr_fun(&on_RangeInput_changed),
                                         XInput, &func_ranges.xrange));
          }
          builder->get_widget("YInput", YInput);
          if (YInput) {
            YInput->signal_changed().connect(
                sigc::bind<Gtk::Entry*, std::pair<double, double>*>(sigc::ptr_fun(&on_RangeInput_changed),
                                         YInput, &func_ranges.yrange));
          }
          builder->get_widget("ZInput", ZInput);
          if (ZInput) {
            ZInput->signal_changed().connect(
                sigc::bind<Gtk::Entry*, std::pair<double, double>*>(sigc::ptr_fun(&on_RangeInput_changed),
                                         ZInput, &func_ranges.zrange));
          }
        }
      }
      builder->get_widget("FunctionOutput", FunctionOutput);
      if (FunctionOutput) {
        // TODO: Connect signals and make GL routine
      }
    }
    app->run(*MainWindow);
    delete FunctionOutput;
    delete XInput;
    delete VariableInput;
    delete FunctionInput;
    delete InputBox;
    delete CentralDivider;
  }
  delete MainWindow;
  return 0;
}
