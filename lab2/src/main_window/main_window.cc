#include <QLabel>

#include <main_window/main_window.h>
#include <glwidget/glwidget.h>

namespace main_window_ns {
  MainWindow::MainWindow()
  {
    gl_widget = new glwidget::GLWidget(this);
    main_widget = new QWidget(this);
    main_layout = new QVBoxLayout(main_widget);
    setCentralWidget(main_widget);

    main_layout->addWidget(gl_widget);

    params_window = new params_window_ns::ParamsWindow(this);

    connect(params_window, &params_window_ns::ParamsWindow::sig_light_position_updated,
            [this](glm::fvec3 position) { gl_widget->set_light_position(position); });
    connect(params_window, &params_window_ns::ParamsWindow::sig_light_position_updated_x,
            [this](GLfloat new_x) { gl_widget->set_light_position_x(new_x); });
    connect(params_window, &params_window_ns::ParamsWindow::sig_light_position_updated_y,
            [this](GLfloat new_y) { gl_widget->set_light_position_y(new_y); });
    connect(params_window, &params_window_ns::ParamsWindow::sig_light_position_updated_z,
            [this](GLfloat new_z) { gl_widget->set_light_position_z(new_z); });

    connect(params_window, &params_window_ns::ParamsWindow::sig_color_changed,
            [this](QColor color) { gl_widget->set_light_color(color); });
    
    connect(params_window, &params_window_ns::ParamsWindow::sig_light_intensity_updated,
            [this](GLfloat new_intensity) { gl_widget->set_light_intensity(new_intensity); });

    params_window->show();
  }
} // namespace main_window_ns
