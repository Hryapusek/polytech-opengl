#pragma once

#include <QWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <glwidget/glwidget.h>
#include <params_window/params_window.h>

namespace main_window_ns {
  class MainWindow : public QMainWindow
  {
   public:
    MainWindow();

   private:
    QWidget *main_widget = nullptr;
    QVBoxLayout *main_layout = nullptr;
    glwidget::GLWidget *gl_widget = nullptr;

    params_window_ns::ParamsWindow *params_window = nullptr;
  };
} // namespace main_window_ns
