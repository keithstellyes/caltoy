#include "lunar.hpp"
#include "util.hpp"

#include <QApplication>
#include <QLabel>

#include <QApplication>
#include <QComboBox>
#include <QWidget>
#include <QGridLayout>
#include <QBoxLayout>
#include <QPushButton>
#include <QStackedWidget>

class LunarMonthView : public QWidget {
    public:
        QWidget *createMonthWidget(const lunar::Year &ly, gregorian::Month month)
        {
            QGridLayout *layout = new QGridLayout();

            const auto phases = ly.getPhases();
            int week = 0;
            layout->setSpacing(0);
            // otherwise rows might get spaces added
            layout->setAlignment(Qt::AlignTop);
            const char *daysOfWeek[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
            for(int i = 0; i < 7; i++) {
                QLabel *dowLabel = new QLabel(daysOfWeek[i]);
                dowLabel->setMaximumHeight(20);
                dowLabel->setStyleSheet("border: 1px solid black;");
                layout->addWidget(dowLabel, 2, i);
            }
            for(int i = 0; i < static_cast<int>(gregorian::Date(2026, month, 1).getDayOfWeek()); i++) {
                QLabel *blankLabel = new QLabel(" ");
                blankLabel->setStyleSheet("border: 1px solid black;");
                blankLabel->setFixedHeight(100);
                layout->addWidget(blankLabel, week + 3, i);
            }
            int phasesStartingIndex = 0;
            gregorian::Month currMonth = gregorian::Month::January;
            while(currMonth != month) {
                phasesStartingIndex += gregorian::daysInMonth(currMonth, gregorian::yearIsLeapYear(ly.gregorianYear));
                currMonth = static_cast<gregorian::Month>(static_cast<int>(currMonth) + 1);
            }
            for(int i = 1; i <= gregorian::daysInMonth(currMonth, gregorian::yearIsLeapYear(ly.gregorianYear)); i++) {
                lunar::Phase p = phases[(i - 1) + phasesStartingIndex];
                QLabel *dayLabel = new QLabel((std::to_string(i) + "\t" + to_utf8(unicodeCodepoint(p)) + "\n" + std::string(to_string(p))).c_str());
                dayLabel->setStyleSheet("border: 1px solid black;");
                dayLabel->setFixedHeight(100);
                auto dayOfWeek = gregorian::Date(2026, month, i).getDayOfWeek();
                layout->addWidget(dayLabel, week + 3, static_cast<int>(dayOfWeek));
                if(dayOfWeek == gregorian::DayOfWeek::Saturday) {
                    week += 1;
                }
            }

            QWidget *w = new QWidget();
            w->setLayout(layout);
            return w;
        }

        LunarMonthView(const lunar::Year &ly)
        {
            QComboBox *monthSelect = new QComboBox();
            QStackedWidget *stackedWidget = new QStackedWidget();
            QObject::connect(monthSelect, &QComboBox::currentIndexChanged, [monthSelect, stackedWidget](int index) {
                    stackedWidget->setCurrentIndex(index);
            });

            for(int i = 0; i < 12; i++) {
                monthSelect->addItem(std::string(to_string(static_cast<gregorian::Month>(i))).c_str());
                stackedWidget->addWidget(createMonthWidget(ly, static_cast<gregorian::Month>(i)));
            }
            QBoxLayout *layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
            layout->addWidget(monthSelect);
            layout->addWidget(stackedWidget);
            setLayout(layout);
        }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    LunarMonthView monthView(lunar::Year(2026));
    monthView.show();
    monthView.setWindowTitle("Lunar Calendar");

    return app.exec();
}
