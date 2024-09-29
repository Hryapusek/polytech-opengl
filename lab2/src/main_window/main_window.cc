#include <QLabel>

#include <main_window/main_window.h>
#include <glwidget/glwidget.h>

namespace main_window_ns {
  MainWindow::MainWindow() {
    gl_widget = new glwidget::GLWidget(this);
    main_widget = new QWidget(this);
    main_layout = new QVBoxLayout(main_widget);
    setCentralWidget(main_widget);

    sliders_layout = new QVBoxLayout();
    main_layout->addLayout(sliders_layout);
    main_layout->addWidget(gl_widget);

    x_slider = new QSlider(Qt::Horizontal);
    y_slider = new QSlider(Qt::Horizontal);
    z_slider = new QSlider(Qt::Horizontal);

    for (auto &slider : {x_slider, y_slider, z_slider}) {
      slider->setMinimum(-1000);
      slider->setMaximum(1000);
      slider->setValue(100);
      slider->setTickPosition(QSlider::TickPosition::TicksAbove);
      slider->setTickInterval(10);
    }

    gl_widget->set_light_position({1.0f, 1.0f, 1.0f});

    {
      QHBoxLayout *pair_layout = new QHBoxLayout;
      QLabel *label = new QLabel("X");
      pair_layout->addWidget(label);
      pair_layout->addWidget(x_slider);
      sliders_layout->addLayout(pair_layout);
      connect(x_slider, &QSlider::valueChanged, [this](int value) {
        gl_widget->set_light_position_x(value/10.0f);
      });
    }

    {
      QHBoxLayout *pair_layout = new QHBoxLayout;
      QLabel *label = new QLabel("Y");
      pair_layout->addWidget(label);
      pair_layout->addWidget(y_slider);
      sliders_layout->addLayout(pair_layout);
      connect(y_slider, &QSlider::valueChanged, [this](int value) {
        gl_widget->set_light_position_y(value/10.0f);
      });
    }

    {
      QHBoxLayout *pair_layout = new QHBoxLayout;
      QLabel *label = new QLabel("Z");
      pair_layout->addWidget(label);
      pair_layout->addWidget(z_slider);
      sliders_layout->addLayout(pair_layout);
      connect(z_slider, &QSlider::valueChanged, [this](int value) {
        gl_widget->set_light_position_z(value/10.0f);
      });
    }
  }
} // namespace main_window_ns
