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
  }
} // namespace main_window_ns
