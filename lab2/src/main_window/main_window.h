#pragma once

#include <QWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <glwidget/glwidget.h>

namespace main_window_ns {
  class MainWindow : public QMainWindow
  {
   public:
    MainWindow();

   private:
    QWidget *main_widget = nullptr;
    QVBoxLayout *main_layout = nullptr;
    QVBoxLayout *sliders_layout = nullptr;
    glwidget::GLWidget *gl_widget = nullptr;

    QSlider *x_slider = nullptr;
    QSlider *y_slider = nullptr;
    QSlider *z_slider = nullptr;
  };
} // namespace main_window_ns
