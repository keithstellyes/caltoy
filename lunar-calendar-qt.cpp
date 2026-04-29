#include <QApplication>
#include <QLabel>

#include "LunarPhase.hpp"
#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>

class LunarMonthView : public QWidget {
    public:
        LunarMonthView(const LunarYear &ly, GregorianMonth month)
        {
            setWindowTitle("QGridLayout Example");

            QGridLayout *layout = new QGridLayout();

            const auto phases = ly.getPhases();
            int week = 0;
            QLabel *monthLabel = new QLabel((std::string(to_string(month)) + " " + std::to_string(2026)).c_str());
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
            for(int i = 0; i < static_cast<int>(GregorianDate(2026, month, i).getDayOfWeek()); i++) {
                QLabel *blankLabel = new QLabel();
                blankLabel->setStyleSheet("border: 1px solid black;");
                layout->addWidget(blankLabel, week + 3, i);
            }
            int phasesStartingIndex = 0;
            GregorianMonth currMonth = GregorianMonth::January;
            while(currMonth != month) {
                phasesStartingIndex += daysInGregorianMonth(currMonth, yearIsLeapYear(ly.gregorianYear));
                currMonth = static_cast<GregorianMonth>(static_cast<int>(currMonth) + 1);
            }
            for(int i = 1; i <= daysInGregorianMonth(currMonth, yearIsLeapYear(ly.gregorianYear)); i++) {
                LunarPhase p = phases[(i - 1) + phasesStartingIndex];
                QLabel *dayLabel = new QLabel((std::to_string(i) + "\t" + to_utf8(unicodeCodepoint(p)) + "\n" + std::string(to_string(p))).c_str());
                dayLabel->setStyleSheet("border: 1px solid black;");
                auto dayOfWeek = GregorianDate(2026, month, i).getDayOfWeek();
                layout->addWidget(dayLabel, week + 3, static_cast<int>(dayOfWeek));
                if(dayOfWeek == GregorianDayOfWeek::Saturday) {
                    week += 1;
                }
            }
            setLayout(layout);
        }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GregorianMonth month = GregorianMonth::January;
    if(argc == 2) {
        month = static_cast<GregorianMonth>(std::stoi(argv[1]) - 1);
    }
    LunarMonthView monthView(LunarYear(2026, GregorianDate(2025, GregorianMonth::December, 19)), month);
    monthView.resize(300, 600);
    monthView.show();
    monthView.setWindowTitle("Lunar Calendar");

    return app.exec();
}
