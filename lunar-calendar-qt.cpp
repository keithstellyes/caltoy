#include <QApplication>
#include <QLabel>


#include "LunarPhase.hpp"
#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>

class LunarMonthView : public QWidget {
    public:
        LunarMonthView();
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("QGridLayout Example");

    QGridLayout *layout = new QGridLayout();

    LunarYear ly(2026, GregorianDate(2025, GregorianMonth::December, 19));
    const auto phases = ly.getPhases();
    int week = 0;
    QLabel *monthLabel = new QLabel(("January " + std::to_string(2026)).c_str());
    monthLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(monthLabel, 0, 0, 1, 7);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);
    const char *daysOfWeek[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    for(int i = 0; i < 7; i++) {
        QLabel *dowLabel = new QLabel(daysOfWeek[i]);
        dowLabel->setStyleSheet("border: 1px solid black;");
        layout->addWidget(dowLabel, 2, i);
    }
    for(int i = 0; i < static_cast<int>(GregorianDate(2026, GregorianMonth::January, i).getDayOfWeek()); i++) {
        QLabel *blankLabel = new QLabel();
        blankLabel->setStyleSheet("border: 1px solid black;");
        layout->addWidget(blankLabel, week + 3, i);
    }
    for(int i = 1; i <= 31; i++) {
        LunarPhase p = phases[i - 1];
        QLabel *dayLabel = new QLabel((std::to_string(i) + "\t" + to_utf8(unicodeCodepoint(p)) + "\n" + std::string(to_string(p))).c_str());
        dayLabel->setStyleSheet("border: 1px solid black;");
        auto dayOfWeek = GregorianDate(2026, GregorianMonth::January, i).getDayOfWeek();
        layout->addWidget(dayLabel, week + 3, static_cast<int>(dayOfWeek));
        if(dayOfWeek == GregorianDayOfWeek::Saturday) {
            week += 1;
        }
    }
    // Set layout on the window
    window.setLayout(layout);
    window.resize(300, 600);
    window.show();

    return app.exec();
}
