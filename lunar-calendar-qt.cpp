#include <QApplication>
#include <QLabel>


#include "LunarPhase.hpp"
#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("QGridLayout Example");

    QGridLayout *layout = new QGridLayout();

    LunarYear ly(2026, GregorianDate(2025, GregorianMonth::December, 19));
    const auto phases = ly.getPhases();
    int week = 0;
    QLabel *monthLabel = new QLabel("January");
    monthLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(monthLabel, 0, 0, 1, 7);
    for(int i = 1; i <= 31; i++) {
        LunarPhase p = phases[i - 1];
        QLabel *dayLabel = new QLabel(to_utf8(unicodeCodepoint(p)).c_str());
        auto dayOfWeek = GregorianDate(2026, GregorianMonth::January, i).getDayOfWeek();
        layout->addWidget(dayLabel, week + 1, static_cast<int>(dayOfWeek));
        if(dayOfWeek == GregorianDayOfWeek::Saturday) {
            week += 1;
        }
    }
    // Set layout on the window
    window.setLayout(layout);

    window.resize(300, 200);
    window.show();

    return app.exec();
}
