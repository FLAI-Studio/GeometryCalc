#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QEvent>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void onGraphChanged(int index);
    void onDigitClicked();
    void onDotClicked();
    void onBackspaceClicked();
    void onClearInputClicked();
    void onCalculate();
    void onClearAll();

private:
    void updateLabelHighlight();

    QComboBox*  graphCombo;   // 替换原来的 graphGroup
    QLineEdit*  edit1;
    QLineEdit*  edit2;
    QLineEdit*  edit3;
    QLabel*     label1;
    QLabel*     label2;
    QLabel*     label3;
    QLabel*     resultLabel;

    int activeParam = -1;
    constexpr static double PI = 3.14159265358979323846;
};