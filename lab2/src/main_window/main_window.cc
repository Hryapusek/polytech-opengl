
#include <main_window/main_window.h>
#include <glwidget/glwidget.h>

namespace main_window_ns {
  MainWindow::MainWindow() {
    gl_widget = new glwidget::GLWidget(this);
    main_widget = new QWidget(this);
    main_layout = new QVBoxLayout(main_widget);
    setCentralWidget(main_widget);

    sliders_layout = new QHBoxLayout();
    main_layout->addLayout(sliders_layout);
    main_layout->addWidget(gl_widget);
  }
} // namespace main_window_ns
