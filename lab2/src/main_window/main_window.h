#pragma once

#include <QWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <glwidget/glwidget.h>

namespace main_window_ns {
  class MainWindow : public QMainWindow
  {
   public:
    MainWindow();

   private:
    QWidget *main_widget = nullptr;
    QVBoxLayout *main_layout = nullptr;
    QHBoxLayout *sliders_layout = nullptr;
    glwidget::GLWidget *gl_widget = nullptr;
  };
} // namespace main_window_ns
