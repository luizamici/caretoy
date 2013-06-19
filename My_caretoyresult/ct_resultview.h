#ifndef CT_RESULTVIEW_H
#define CT_RESULTVIEW_H

#include <QWidget>
#include <QStatusBar>
#include <QXmlStreamReader>

namespace Ui {
class CTResultView;
}

class CTResultView : public QWidget
{
    Q_OBJECT
    
public:
    explicit CTResultView(QWidget *parent = 0);
    ~CTResultView();

private:
    Ui::CTResultView *ui;
    QStatusBar *statusBar;

    void generateReport();
    void generateDataForPlotting();

public slots:
    void showMessage(bool b);

};

#endif // CT_RESULTVIEW_H
