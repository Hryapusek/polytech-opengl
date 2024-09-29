#pragma once

#include <QWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QPushButton>
#include <glwidget/glwidget.h>

#include <custom_widgets/fancy_slider.h>

namespace params_window_ns {
  class ParamsWindow : public QMainWindow
  {
    Q_OBJECT
   public:
    ParamsWindow(QWidget *parent = nullptr);

   signals:
    void sig_light_position_updated(glm::fvec3 position);
    void sig_light_position_updated_x(GLfloat new_x);
    void sig_light_position_updated_y(GLfloat new_y);
    void sig_light_position_updated_z(GLfloat new_z);

    void sig_color_changed(QColor color);

   private:
    QWidget *main_widget = nullptr;
    QVBoxLayout *main_layout = nullptr;

    custom_widgets::FancySlider *x_slider = nullptr;
    custom_widgets::FancySlider *y_slider = nullptr;
    custom_widgets::FancySlider *z_slider = nullptr;

    QPushButton *choose_color_button = nullptr;
  };
} // namespace main_window_ns
