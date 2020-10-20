#include "gtkmm/application.h"
#include "gtkmm/builder.h"
#include "gtkmm/editable.h"
#include "gtkmm/entry.h"
#include "gtkmm/widget.h"
#include "gtkmm/window.h"
#include "gtkmm/windowgroup.h"
#include "parser.h"
#include "sigc++/functors/ptr_fun.h"
#include <gtkmm.h>
#include <iostream>

Expression function("");

void on_FunctionInput_changed(Gtk::Entry *FunctionInput) {
  function.set_function(FunctionInput->get_text());
}

int main(int argc, char *argv[]) {
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
    Gtk::Widget *XInput = nullptr;
    Gtk::Widget *FunctionOutput = nullptr;
    builder->get_widget("CentralDivider", CentralDivider);
    if (CentralDivider) {
      builder->get_widget("InputBox", InputBox);
      if (InputBox) {
        builder->get_widget("FunctionInput", FunctionInput);
        if (FunctionInput) {
          FunctionInput->set_text("x^3");
          FunctionInput->signal_changed().connect(sigc::bind<Gtk::Entry *>(
              sigc::ptr_fun(&on_FunctionInput_changed), FunctionInput));
          std::cout << FunctionInput->get_text() << std::endl;
        }
        builder->get_widget("VariableInput", VariableInput);
        if (VariableInput) {
          // Too lazy
        }
      }
      builder->get_widget("FunctionOutput", FunctionOutput);
      if (FunctionOutput) {
        // Connect signals
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
