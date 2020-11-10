#include "gdkmm/glcontext.h"
#include "gldraw.h"
#include "gtkmm/glarea.h"
#include "parser.h"
#include "rangeparser.h"
#include "sigc++/functors/ptr_fun.h"
#include <cstdlib>
#include <gtkmm.h>
#include <iostream>
#include <utility>

struct Ranges {
  std::pair<float, float> xrange;
  std::pair<float, float> yrange;
  std::pair<float, float> zrange;
};

void on_FunctionInput_changed(const Gtk::Entry *FunctionInput,
                              Expression *function) {
  function->set_function(FunctionInput->get_text());
}
void on_RangeInput_changed(const Gtk::Entry *RangeInput,
                           std::pair<float,float>* range) {
  *range = rangeparser(RangeInput->get_text());
}

void on_CalculateButton_clicked(Expression *function, Ranges* ranges,
                                std::vector<std::vector<float>> &calculations,
                                float precision,
                                Gtk::GLArea *output) {
  size_t xi = 0;
  for (float x = ranges->xrange.first; x < ranges->xrange.second;
       x += precision, xi++) {
    size_t yi = 0;
    calculations.push_back({});
    for (float y = ranges->yrange.first; y < ranges->yrange.second;
         y += precision, yi++) {
      calculations[xi].push_back(function->calculate(x, y));
    }
  }
  std::cout << "output is " << output << std::endl;
  if (output) {
    realize(output, calculations);
    output->queue_draw();
  }
}

int main(int argc, char *argv[]) {
  Expression function; 
  Ranges func_ranges = {{0, 0}, {0, 0}, {0, 0}};
  std::vector<std::vector<float>> calculations;
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
    Gtk::Button *CalculateButton = nullptr;
    Gtk::GLArea *FunctionOutput = nullptr;
    builder->get_widget("CentralDivider", CentralDivider);
    if (CentralDivider) {
      builder->get_widget("InputBox", InputBox);
      if (InputBox) {
        builder->get_widget("FunctionInput", FunctionInput);
        if (FunctionInput) {
          FunctionInput->signal_changed().connect(
              sigc::bind<Gtk::Entry *, Expression *>(
                  sigc::ptr_fun(&on_FunctionInput_changed), FunctionInput,
                  &function));
        }
        builder->get_widget("VariableInput", VariableInput);
        if (VariableInput) {
          builder->get_widget("XInput", XInput);
          if (XInput) {
            XInput->signal_changed().connect(
                sigc::bind<Gtk::Entry *, std::pair<float,float>*>(
                    sigc::ptr_fun(&on_RangeInput_changed), XInput,
                    &func_ranges.xrange));
          }
          builder->get_widget("YInput", YInput);
          if (YInput) {
            YInput->signal_changed().connect(
                sigc::bind<Gtk::Entry *, std::pair<float,float>*>(
                    sigc::ptr_fun(&on_RangeInput_changed), YInput,
                    &func_ranges.yrange));
          }
          builder->get_widget("ZInput", ZInput);
          if (ZInput) {
            ZInput->signal_changed().connect(
                sigc::bind<Gtk::Entry *, std::pair<float,float>*>(
                    sigc::ptr_fun(&on_RangeInput_changed), ZInput,
                    &func_ranges.zrange));
          }
        }
      }
      builder->get_widget("FunctionOutput", FunctionOutput);
      if (FunctionOutput) {
        FunctionOutput->signal_realize().connect(
            sigc::bind<Gtk::GLArea *, std::vector<std::vector<float>> &>(
                sigc::ptr_fun(&realize), FunctionOutput, calculations));
        FunctionOutput->signal_unrealize().connect(sigc::bind<Gtk::GLArea *>(
            sigc::ptr_fun(&unrealize), FunctionOutput));
        FunctionOutput->signal_render().connect(sigc::ptr_fun(draw), false);
        glewInit();
      }
      // calculate button needs a pointer to function output
      builder->get_widget("CalculateButton", CalculateButton);
      if (CalculateButton) {
        CalculateButton->signal_clicked().connect(
            sigc::bind<Expression *, Ranges*, std::vector<std::vector<float>>&,
                       float, Gtk::GLArea *>(
                sigc::ptr_fun(&on_CalculateButton_clicked), &function, &func_ranges,
                calculations, 0.1, FunctionOutput));
        // TODO: make precision user-definable ^^^
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
