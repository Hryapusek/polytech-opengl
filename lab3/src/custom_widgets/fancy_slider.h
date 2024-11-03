#pragma once

#include <QSlider>

namespace custom_widgets {
  class FancySlider : public QSlider
  {
    Q_OBJECT
   public:
    explicit FancySlider(QWidget* parent = 0);
    explicit FancySlider(Qt::Orientation orientation, QWidget* parent = 0);

   protected:
    virtual void sliderChange(SliderChange change);
  };
} // namespace custom_widgets
