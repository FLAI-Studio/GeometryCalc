#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QString>
#include <QEvent>
#include <QComboBox>
#include <QPushButton>
#include <QOverload>

// ============================================================
//  构造函数
// ============================================================
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("图形面体积计算器");
    resize(420, 680);

    // ---------- 标题 ----------
    QLabel* title = new QLabel("📐 图形面体积计算器");
    title->setAlignment(Qt::AlignCenter);
    QFont f = title->font();
    f.setPointSize(20);
    f.setBold(true);
    title->setFont(f);

    // ---------- 图形选择（下拉框）----------
    graphCombo = new QComboBox();
    graphCombo->setMinimumHeight(36);
    graphCombo->setStyleSheet("font-size: 15px; padding: 4px;");
    graphCombo->addItems({
        "正方形（面积）",
        "长方形（面积）",
        "平行四边形（面积）",
        "梯形（面积）",
        "圆形（面积）",
        "正方体（体积）",
        "长方体（体积）",
        "圆柱体（体积）",
        "圆锥体（体积）",
        "球体（体积）"
    });

    QGroupBox* graphBox = new QGroupBox("选择图形");
    QVBoxLayout* comboLayout = new QVBoxLayout();
    comboLayout->addWidget(graphCombo);
    graphBox->setLayout(comboLayout);

    // ---------- 参数输入区 ----------
    label1 = new QLabel("参数1：");
    label2 = new QLabel("参数2：");
    label3 = new QLabel("参数3：");

    edit1 = new QLineEdit(); edit1->setAlignment(Qt::AlignRight);
    edit2 = new QLineEdit(); edit2->setAlignment(Qt::AlignRight);
    edit3 = new QLineEdit(); edit3->setAlignment(Qt::AlignRight);
    // 不再只读，支持手动键盘输入
    edit1->setPlaceholderText("点击此处或选标签后输入");
    edit2->setPlaceholderText("点击此处或选标签后输入");
    edit3->setPlaceholderText("点击此处或选标签后输入");
    edit1->setMinimumHeight(32); edit2->setMinimumHeight(32); edit3->setMinimumHeight(32);

    label1->setStyleSheet("padding: 4px; border-radius: 4px;");
    label2->setStyleSheet("padding: 4px; border-radius: 4px;");
    label3->setStyleSheet("padding: 4px; border-radius: 4px;");

    label1->setProperty("paramIndex", 0);
    label2->setProperty("paramIndex", 1);
    label3->setProperty("paramIndex", 2);
    label1->installEventFilter(this);
    label2->installEventFilter(this);
    label3->installEventFilter(this);
    // 编辑框也装 eventFilter，获得焦点时自动切换激活参数
    edit1->installEventFilter(this);
    edit2->installEventFilter(this);
    edit3->installEventFilter(this);

    QFormLayout* paramLayout = new QFormLayout();
    paramLayout->addRow(label1, edit1);
    paramLayout->addRow(label2, edit2);
    paramLayout->addRow(label3, edit3);

    QGroupBox* paramBox = new QGroupBox("参数输入（可手动打字或点标签后用数字键）");
    paramBox->setLayout(paramLayout);

    // ---------- 数字键盘 ----------
    QGridLayout* numGrid = new QGridLayout();
    numGrid->setSpacing(6);

    for (int i = 1; i <= 9; ++i) {
        QPushButton* btn = new QPushButton(QString::number(i));
        btn->setMinimumHeight(44);
        btn->setStyleSheet("QPushButton { background: #fff; border: 2px solid #ddd; border-radius: 8px; font-size: 18px; font-weight: bold; }"
                           "QPushButton:pressed { background: #cce; }");
        connect(btn, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
        numGrid->addWidget(btn, (i-1)/3, (i-1)%3);
    }
    QPushButton* btn0 = new QPushButton("0");
    btn0->setMinimumHeight(44);
    btn0->setStyleSheet("QPushButton { background: #fff; border: 2px solid #ddd; border-radius: 8px; font-size: 18px; font-weight: bold; }"
                        "QPushButton:pressed { background: #cce; }");
    connect(btn0, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
    numGrid->addWidget(btn0, 3, 0);

    QPushButton* btnDot = new QPushButton(".");
    btnDot->setMinimumHeight(44);
    btnDot->setStyleSheet("QPushButton { background: #fff; border: 2px solid #ddd; border-radius: 8px; font-size: 18px; font-weight: bold; }"
                          "QPushButton:pressed { background: #cce; }");
    connect(btnDot, &QPushButton::clicked, this, &MainWindow::onDotClicked);
    numGrid->addWidget(btnDot, 3, 1);

    QPushButton* btnBack = new QPushButton("←");
    btnBack->setMinimumHeight(44);
    btnBack->setStyleSheet("QPushButton { background: #ffcccb; border: 2px solid #f99; border-radius: 8px; font-size: 18px; font-weight: bold; }"
                           "QPushButton:pressed { background: #f99; }");
    connect(btnBack, &QPushButton::clicked, this, &MainWindow::onBackspaceClicked);
    numGrid->addWidget(btnBack, 3, 2);

    QPushButton* btnClearInput = new QPushButton("C");
    btnClearInput->setMinimumHeight(44);
    btnClearInput->setStyleSheet("QPushButton { background: #ffeb99; border: 2px solid #f0c040; border-radius: 8px; font-size: 18px; font-weight: bold; }"
                                 "QPushButton:pressed { background: #f0c040; }");
    connect(btnClearInput, &QPushButton::clicked, this, &MainWindow::onClearInputClicked);
    numGrid->addWidget(btnClearInput, 4, 0, 1, 3);

    QGroupBox* numBox = new QGroupBox("数字键盘");
    numBox->setLayout(numGrid);

    // ---------- 计算 / 清空按钮 ----------
    QPushButton* calcBtn  = new QPushButton("计 算");
    QPushButton* clearBtn = new QPushButton("全部清空");
    calcBtn->setMinimumHeight(48);
    clearBtn->setMinimumHeight(48);
    calcBtn->setStyleSheet("QPushButton { background: #5b8def; color: white; border-radius: 9px; font-size: 18px; font-weight: bold; }");
    clearBtn->setStyleSheet("QPushButton { background: #ccc; color: #333; border-radius: 9px; font-size: 16px; }");
    connect(calcBtn,  &QPushButton::clicked, this, &MainWindow::onCalculate);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::onClearAll);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(calcBtn);
    btnLayout->addWidget(clearBtn);

    // ---------- 结果显示 ----------
    resultLabel = new QLabel("请先选择一个图形");
    resultLabel->setWordWrap(true);
    resultLabel->setStyleSheet("font-size: 14px; padding: 10px; background: #f5f5f5; border-radius: 6px;");
    QGroupBox* resultBox = new QGroupBox("计算结果");
    QVBoxLayout* resultBoxLayout = new QVBoxLayout();
    resultBoxLayout->addWidget(resultLabel);
    resultBox->setLayout(resultBoxLayout);

    // ---------- 底部信息 ----------
    QLabel* footer = new QLabel(
        "版本：v0.0.1-rc1\n"
        "开发者：Byjsmc\n"
        "最后更新于：2026/09/03"
        );
    footer->setStyleSheet("color: gray; font-size: 11px;");
    footer->setWordWrap(true);
    footer->setAlignment(Qt::AlignCenter);

    // ---------- 主布局 ----------
    QWidget* central = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(8);
    mainLayout->addWidget(title);
    mainLayout->addWidget(graphBox);
    mainLayout->addWidget(paramBox);
    mainLayout->addWidget(numBox);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(resultBox);
    mainLayout->addStretch();
    mainLayout->addWidget(footer);
    setCentralWidget(central);

    // 默认隐藏参数
    label1->hide(); edit1->hide();
    label2->hide(); edit2->hide();
    label3->hide(); edit3->hide();

    // 连接下拉框信号
    connect(graphCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onGraphChanged);

    // 初始刷新
    onGraphChanged(-1);
}

// ============================================================
//  eventFilter：标签点击 + 编辑框焦点
// ============================================================
bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    // 标签点击
    if (event->type() == QEvent::MouseButtonPress) {
        QLabel* label = qobject_cast<QLabel*>(obj);
        if (label && (label == label1 || label == label2 || label == label3)) {
            activeParam = label->property("paramIndex").toInt();
            updateLabelHighlight();
            resultLabel->setText(QString("已选中：%1，可手动输入或用数字键")
                                     .arg(label->text()));
            return true;
        }
    }
    // 编辑框获得焦点
    if (event->type() == QEvent::FocusIn) {
        if (obj == edit1)      { activeParam = 0; updateLabelHighlight(); }
        else if (obj == edit2) { activeParam = 1; updateLabelHighlight(); }
        else if (obj == edit3) { activeParam = 2; updateLabelHighlight(); }
    }
    return QMainWindow::eventFilter(obj, event);
}

// ============================================================
//  图形切换（下拉框）
// ============================================================
void MainWindow::onGraphChanged(int index) {
    int id = index;
    if (id < 0) return;

    activeParam = -1;
    label1->setStyleSheet(""); label2->setStyleSheet(""); label3->setStyleSheet("");
    label1->hide(); edit1->hide(); edit1->clear();
    label2->hide(); edit2->hide(); edit2->clear();
    label3->hide(); edit3->hide(); edit3->clear();

    switch (id) {
    case 0: label1->setText("边长："); label1->show(); edit1->show(); break;
    case 1: label1->setText("长："); label2->setText("宽："); label1->show(); edit1->show(); label2->show(); edit2->show(); break;
    case 2: label1->setText("底："); label2->setText("高："); label1->show(); edit1->show(); label2->show(); edit2->show(); break;
    case 3: label1->setText("上底："); label2->setText("下底："); label3->setText("高："); label1->show(); edit1->show(); label2->show(); edit2->show(); label3->show(); edit3->show(); break;
    case 4: label1->setText("半径："); label1->show(); edit1->show(); break;
    case 5: label1->setText("边长："); label1->show(); edit1->show(); break;
    case 6: label1->setText("长："); label2->setText("宽："); label3->setText("高："); label1->show(); edit1->show(); label2->show(); edit2->show(); label3->show(); edit3->show(); break;
    case 7: label1->setText("底面半径："); label2->setText("高："); label1->show(); edit1->show(); label2->show(); edit2->show(); break;
    case 8: label1->setText("底面半径："); label2->setText("高："); label1->show(); edit1->show(); label2->show(); edit2->show(); break;
    case 9: label1->setText("半径："); label1->show(); edit1->show(); break;
    }

    // 自动激活第一个参数
    activeParam = 0;
    updateLabelHighlight();
    resultLabel->setText("请直接输入参数，或点击标签后用数字键盘输入");
}

// ============================================================
//  高亮更新
// ============================================================
void MainWindow::updateLabelHighlight() {
    label1->setStyleSheet(""); label2->setStyleSheet(""); label3->setStyleSheet("");
    if (activeParam == 0)      label1->setStyleSheet("background: #5b8def; color: white; padding: 4px; border-radius: 4px;");
    else if (activeParam == 1) label2->setStyleSheet("background: #5b8def; color: white; padding: 4px; border-radius: 4px;");
    else if (activeParam == 2) label3->setStyleSheet("background: #5b8def; color: white; padding: 4px; border-radius: 4px;");
}

// ============================================================
//  数字键
// ============================================================
void MainWindow::onDigitClicked() {
    if (activeParam < 0) { resultLabel->setText("⚠️ 请先选择一个图形！"); return; }
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    QLineEdit* edits[] = {edit1, edit2, edit3};
    edits[activeParam]->insert(btn->text());
}

void MainWindow::onDotClicked() {
    if (activeParam < 0) return;
    QLineEdit* edits[] = {edit1, edit2, edit3};
    QString text = edits[activeParam]->text();
    if (!text.contains('.')) edits[activeParam]->insert(".");
}

void MainWindow::onBackspaceClicked() {
    if (activeParam < 0) return;
    QLineEdit* edits[] = {edit1, edit2, edit3};
    QString text = edits[activeParam]->text();
    if (!text.isEmpty()) edits[activeParam]->setText(text.chopped(1));
}

void MainWindow::onClearInputClicked() {
    if (activeParam < 0) return;
    QLineEdit* edits[] = {edit1, edit2, edit3};
    edits[activeParam]->clear();
}

// ============================================================
//  计算
// ============================================================
void MainWindow::onCalculate() {
    int id = graphCombo->currentIndex();
    if (id < 0) { resultLabel->setText("⚠️ 请先选择一个图形！"); return; }

    bool ok1 = true, ok2 = true, ok3 = true;
    double a = edit1->text().toDouble(&ok1);
    double b = edit2->text().toDouble(&ok2);
    double c = edit3->text().toDouble(&ok3);

    QString err;
    switch (id) {
    case 0: case 4: case 5: case 9: if (!ok1 || a <= 0) err = "参数1无效"; break;
    case 1: case 2: case 7: case 8: if (!ok1 || !ok2 || a <= 0 || b <= 0) err = "参数无效"; break;
    case 3: case 6: if (!ok1 || !ok2 || !ok3 || a <= 0 || b <= 0 || c <= 0) err = "参数无效"; break;
    }
    if (!err.isEmpty()) { resultLabel->setText(QString("⚠️ %1，请检查输入").arg(err)); return; }

    double res = 0; QString formula;
    switch (id) {
    case 0: res = a*a; formula = QString("边长×边长 = %1×%2").arg(a).arg(a); break;
    case 1: res = a*b; formula = QString("长×宽 = %1×%2").arg(a).arg(b); break;
    case 2: res = a*b; formula = QString("底×高 = %1×%2").arg(a).arg(b); break;
    case 3: res = (a+b)*c/2.0; formula = QString("(上底+下底)×高÷2 = (%1+%2)×%3÷2").arg(a).arg(b).arg(c); break;
    case 4: res = a*a*PI; formula = QString("π×半径² = π×%1²").arg(a); break;
    case 5: res = a*a*a; formula = QString("边长³ = %1³").arg(a); break;
    case 6: res = a*b*c; formula = QString("长×宽×高 = %1×%2×%3").arg(a).arg(b).arg(c); break;
    case 7: res = a*a*PI*b; formula = QString("π×半径²×高 = π×%1²×%2").arg(a).arg(b); break;
    case 8: res = a*a*PI*b/3.0; formula = QString("π×半径²×高÷3 = π×%1²×%2÷3").arg(a).arg(b); break;
    case 9: res = a*a*a*PI*4.0/3.0; formula = QString("4/3×π×半径³ = 4/3×π×%1³").arg(a); break;
    }

    resultLabel->setText(QString("结果：<b>%1</b><br><span style='color:#666;font-size:12px;'>公式：%2</span>")
                             .arg(res).arg(formula));
}

// ============================================================
//  全部清空
// ============================================================
void MainWindow::onClearAll() {
    graphCombo->setCurrentIndex(-1);

    activeParam = -1;
    label1->setStyleSheet(""); label2->setStyleSheet(""); label3->setStyleSheet("");
    edit1->clear(); edit2->clear(); edit3->clear();
    label1->hide(); edit1->hide();
    label2->hide(); edit2->hide();
    label3->hide(); edit3->hide();
    resultLabel->setText("已清空，请重新选择图形");
}