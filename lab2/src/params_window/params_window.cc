#include <QLabel>

#include <params_window/params_window.h>
#include <glwidget/glwidget.h>

#include <QColorDialog>

namespace params_window_ns {
  ParamsWindow::ParamsWindow(QWidget* parent)
    : QMainWindow(parent)
  {
    main_widget = new QWidget(this);
    main_layout = new QVBoxLayout(main_widget);
    setCentralWidget(main_widget);

    x_slider = new custom_widgets::FancySlider(Qt::Horizontal);
    y_slider = new custom_widgets::FancySlider(Qt::Horizontal);
    z_slider = new custom_widgets::FancySlider(Qt::Horizontal);

    for (auto& slider : {x_slider, y_slider, z_slider}) {
      slider->setMinimum(-1000);
      slider->setMaximum(1000);
      slider->setValue(100);
      slider->setTickPosition(QSlider::TickPosition::TicksAbove);
    }

    {
      QHBoxLayout* pair_layout = new QHBoxLayout;
      QLabel* label = new QLabel("X");
      pair_layout->addWidget(label);
      pair_layout->addWidget(x_slider);
      main_layout->addLayout(pair_layout);
      connect(x_slider, &QSlider::valueChanged,
              [this](int value) { sig_light_position_updated_x(value / 10.0f); });
    }

    {
      QHBoxLayout* pair_layout = new QHBoxLayout;
      QLabel* label = new QLabel("Y");
      pair_layout->addWidget(label);
      pair_layout->addWidget(y_slider);
      main_layout->addLayout(pair_layout);
      connect(y_slider, &QSlider::valueChanged,
              [this](int value) { sig_light_position_updated_y(value / 10.0f); });
    }

    {
      QHBoxLayout* pair_layout = new QHBoxLayout;
      QLabel* label = new QLabel("Z");
      pair_layout->addWidget(label);
      pair_layout->addWidget(z_slider);
      main_layout->addLayout(pair_layout);
      connect(z_slider, &QSlider::valueChanged,
              [this](int value) { sig_light_position_updated_z(value / 10.0f); });
    }

    choose_color_button = new QPushButton();
    choose_color_button->setText("Choose Color");
    connect(choose_color_button, &QPushButton::clicked, [this]() {
      auto new_color = QColorDialog::getColor(Qt::black, this, "Choose Color");

      if (new_color.isValid()) {
        sig_color_changed(new_color);
      }
    });
    main_layout->addWidget(choose_color_button);

    main_layout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));
  }
} // namespace params_window_ns
