#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void onGraphChanged(int index);
    void onDigitClicked();
    void onDotClicked();
    void onClearInputClicked();
    void onCalculate();
    void onClearAll();
    void onCopyResult();
    void onHistoryItemClicked(QListWidgetItem* item);
    void onDeleteHistory();
    void onClearAllHistory();

private:
    void updateLabelHighlight();
    void saveHistory();
    void loadHistory();
    void saveConfig();
    void loadConfig();

    // UI
    QComboBox* graphCombo = nullptr;
    QLabel* label1 = nullptr;
    QLabel* label2 = nullptr;
    QLabel* label3 = nullptr;
    QLineEdit* edit1 = nullptr;
    QLineEdit* edit2 = nullptr;
    QLineEdit* edit3 = nullptr;
    QLabel* resultLabel = nullptr;
    QPushButton* btnCopy = nullptr;
    QListWidget* historyList = nullptr;
    QPushButton* btnDeleteHistory = nullptr;
    QPushButton* btnClearAllHistory = nullptr;

    int activeParam = -1;
    constexpr static double PI = 3.14159265358979323846;
};

#endif // MAINWINDOW_H
