#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QFontDialog>
#include <QInputDialog>
#include <QTextCursor>
#include <QTextEdit>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QGroupBox>
#include <QTabWidget>
#include <QListWidget>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

// 文件引入
QString getIncludeHeader() {
    QDialog dialog;
    dialog.setWindowTitle("引入文件");
    dialog.setMinimumSize(400, 300);
    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    QTabWidget *tabWidget = new QTabWidget(&dialog);
    QWidget *standardTab = new QWidget();
    QVBoxLayout *standardLayout = new QVBoxLayout(standardTab);
    QLabel *standardLabel = new QLabel("选择标准头文件:", standardTab);
    QListWidget *standardList = new QListWidget(standardTab);
    QStringList standardHeaders = {
        "iostream", "cstdio", "cstdlib", "cstring", "cmath", "ctime", "cctype",
        "vector", "string", "algorithm", "map", "set", "list", "deque",
        "queue", "stack", "array", "forward_list", "unordered_map",
        "unordered_set", "bitset", "valarray", "tuple", "utility",
        "functional", "memory", "iterator", "numeric", "random", "regex",
        "fstream", "sstream", "iomanip", "thread", "mutex", "future",
        "condition_variable", "atomic", "exception", "stdexcept", "new"
    };

    foreach (const QString &header, standardHeaders) {
        standardList->addItem(header);
    }

    standardLayout->addWidget(standardLabel);
    standardLayout->addWidget(standardList);
    QWidget *customTab = new QWidget();
    QVBoxLayout *customLayout = new QVBoxLayout(customTab);

    QLabel *customLabel = new QLabel("选择自定义头文件:", customTab);
    QLineEdit *filePathEdit = new QLineEdit(customTab);
    filePathEdit->setPlaceholderText("点击浏览按钮选择文件...");

    QPushButton *browseButton = new QPushButton("浏览...", customTab);

    QHBoxLayout *fileLayout = new QHBoxLayout();
    fileLayout->addWidget(filePathEdit);
    fileLayout->addWidget(browseButton);

    customLayout->addWidget(customLabel);
    customLayout->addLayout(fileLayout);
    tabWidget->addTab(standardTab, "标准头文件");
    tabWidget->addTab(customTab, "自定义文件");
    QPushButton *okButton = new QPushButton("确定", &dialog);
    QPushButton *cancelButton = new QPushButton("取消", &dialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addWidget(tabWidget);
    mainLayout->addLayout(buttonLayout);
    QString result;
    QObject::connect(browseButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&dialog,
                                                        "选择要引入的文件",
                                                        "",
                                                        "C++文件 (*.h *.hpp *.cpp);;所有文件 (*.*)");
        if (!fileName.isEmpty()) {
            filePathEdit->setText(fileName);
        }
    });

    QObject::connect(okButton, &QPushButton::clicked, [&]() {
        if (tabWidget->currentIndex() == 0) {
            //头文件
            QListWidgetItem *selected = standardList->currentItem();
            if (selected) {
                result = QString("#include <%1>\n").arg(selected->text());
            }
        } else {
            //自定义文件
            QString filePath = filePathEdit->text();
            if (!filePath.isEmpty()) {
                QString baseName = QFileInfo(filePath).fileName();
                result = QString("#include \"%1\"\n").arg(baseName);
            }
        }
        dialog.accept();
    });

    QObject::connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        return result;
    }

    return "";
}

//if...elif...else
MultiIfDialog::MultiIfDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("多重条件语句");
    setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *condition1Label = new QLabel("第一个条件:");
    condition1Edit = new QLineEdit();
    condition1Edit->setPlaceholderText("例如: score >= 90");

    QLabel *true1Label = new QLabel("条件成立时执行:");
    true1Edit = new QLineEdit();
    true1Edit->setPlaceholderText("例如: cout << \"优秀\" << endl;");
    QLabel *condition2Label = new QLabel("第二个条件:");
    condition2Edit = new QLineEdit();
    condition2Edit->setPlaceholderText("例如: score >= 60");
    QLabel *true2Label = new QLabel("条件成立时执行:");
    true2Edit = new QLineEdit();
    true2Edit->setPlaceholderText("例如: cout << \"及格\" << endl;");
    QLabel *falseLabel = new QLabel("其他情况执行:");
    falseEdit = new QLineEdit();
    falseEdit->setPlaceholderText("例如: cout << \"不及格\" << endl;");
    QPushButton *okButton = new QPushButton("确定");
    QPushButton *cancelButton = new QPushButton("取消");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addWidget(condition1Label);
    layout->addWidget(condition1Edit);
    layout->addWidget(true1Label);
    layout->addWidget(true1Edit);
    layout->addWidget(condition2Label);
    layout->addWidget(condition2Edit);
    layout->addWidget(true2Label);
    layout->addWidget(true2Edit);
    layout->addWidget(falseLabel);
    layout->addWidget(falseEdit);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString MultiIfDialog::getCondition1() const
{
    return condition1Edit->text().trimmed();
}

QString MultiIfDialog::getCondition2() const
{
    return condition2Edit->text().trimmed();
}

QString MultiIfDialog::getTrueBranch1() const
{
    return true1Edit->text().trimmed();
}

QString MultiIfDialog::getTrueBranch2() const
{
    return true2Edit->text().trimmed();
}

QString MultiIfDialog::getFalseBranch() const
{
    return falseEdit->text().trimmed();
}

//循环
LoopDialog::LoopDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("循环语句");
    setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *typeLabel = new QLabel("循环类型:");
    typeCombo = new QComboBox();
    typeCombo->addItems({"while循环", "for循环", "do-while循环"});

    //for
    varLabel = new QLabel("循环变量名:");
    varNameEdit = new QLineEdit();
    varNameEdit->setPlaceholderText("例如: i");
    varNameEdit->setText("i");

    //起始值
    initLabel = new QLabel("起始值:");
    initValueEdit = new QLineEdit();
    initValueEdit->setPlaceholderText("例如: 0");
    initValueEdit->setText("0");

    //循环条件
    QLabel *conditionLabel = new QLabel("循环条件:");
    conditionEdit = new QLineEdit();
    conditionEdit->setPlaceholderText("例如: i < 10");
    conditionEdit->setText("i < 10");

    //步长
    stepLabel = new QLabel("步长:");
    stepEdit = new QLineEdit();
    stepEdit->setPlaceholderText("例如: 1");
    stepEdit->setText("i++");

    // 按钮
    QPushButton *okButton = new QPushButton("确定");
    QPushButton *cancelButton = new QPushButton("取消");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(typeLabel);
    layout->addWidget(typeCombo);
    layout->addWidget(varLabel);
    layout->addWidget(varNameEdit);
    layout->addWidget(initLabel);
    layout->addWidget(initValueEdit);
    layout->addWidget(conditionLabel);
    layout->addWidget(conditionEdit);
    layout->addWidget(stepLabel);
    layout->addWidget(stepEdit);
    layout->addLayout(buttonLayout);
    varLabel->setVisible(false);
    varNameEdit->setVisible(false);
    initLabel->setVisible(false);
    initValueEdit->setVisible(false);
    stepLabel->setVisible(false);
    stepEdit->setVisible(false);
    connect(typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LoopDialog::onLoopTypeChanged);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void LoopDialog::onLoopTypeChanged(int index)
{
    bool isForLoop = (index == 1);

    varLabel->setVisible(isForLoop);
    varNameEdit->setVisible(isForLoop);
    initLabel->setVisible(isForLoop);
    initValueEdit->setVisible(isForLoop);
    stepLabel->setVisible(isForLoop);
    stepEdit->setVisible(isForLoop);
}

QString LoopDialog::getLoopType() const
{
    return typeCombo->currentText();
}

QString LoopDialog::getCondition() const
{
    return conditionEdit->text().trimmed();
}

QString LoopDialog::getInitValue() const
{
    return initValueEdit->text().trimmed();
}

QString LoopDialog::getStep() const
{
    return stepEdit->text().trimmed();
}

QString LoopDialog::getVarName() const
{
    return varNameEdit->text().trimmed();
}

//宏定义
MacroDialog::MacroDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("宏定义");
    setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *nameLabel = new QLabel("宏名称:");
    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("例如: MAX_SIZE");

    QLabel *valueLabel = new QLabel("宏值:");
    valueEdit = new QLineEdit();
    valueEdit->setPlaceholderText("例如: 100");

    QPushButton *okButton = new QPushButton("确定");
    QPushButton *cancelButton = new QPushButton("取消");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(valueLabel);
    layout->addWidget(valueEdit);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString MacroDialog::getMacroName() const {
    return nameEdit->text().trimmed();
}

QString MacroDialog::getMacroValue() const {
    return valueEdit->text().trimmed();
}

//新建函数
FunctionDialog::FunctionDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("新建函数");
    setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *returnLabel = new QLabel("返回类型:");
    returnTypeCombo = new QComboBox();
    returnTypeCombo->addItems({"void", "int", "float", "double", "char", "bool", "string"});
    returnTypeCombo->setEditable(true);

    QLabel *nameLabel = new QLabel("函数名称:");
    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("例如: calculateSum");

    QLabel *paramsLabel = new QLabel("参数列表:");
    paramsEdit = new QLineEdit();
    paramsEdit->setPlaceholderText("例如: int a, int b (留空表示无参数)");

    QPushButton *okButton = new QPushButton("确定");
    QPushButton *cancelButton = new QPushButton("取消");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(returnLabel);
    layout->addWidget(returnTypeCombo);
    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(paramsLabel);
    layout->addWidget(paramsEdit);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString FunctionDialog::getReturnType() const {
    return returnTypeCombo->currentText().trimmed();
}

QString FunctionDialog::getFunctionName() const {
    return nameEdit->text().trimmed();
}

QString FunctionDialog::getParameters() const {
    return paramsEdit->text().trimmed();
}

//新变量
VariableDialog::VariableDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("定义新变量");
    setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *typeLabel = new QLabel("变量类型:");
    typeCombo = new QComboBox();
    typeCombo->addItems({"int", "float", "double", "char", "bool", "string", "auto"});
    typeCombo->setEditable(true);

    QLabel *nameLabel = new QLabel("变量名称:");
    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("例如: counter");

    QLabel *valueLabel = new QLabel("初始值 (可选):");
    valueEdit = new QLineEdit();
    valueEdit->setPlaceholderText("例如: 0");

    constCheckBox = new QCheckBox("常量 (const)");

    QPushButton *okButton = new QPushButton("确定");
    QPushButton *cancelButton = new QPushButton("取消");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(typeLabel);
    layout->addWidget(typeCombo);
    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(valueLabel);
    layout->addWidget(valueEdit);
    layout->addWidget(constCheckBox);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString VariableDialog::getVariableType() const {
    return typeCombo->currentText().trimmed();
}

QString VariableDialog::getVariableName() const {
    return nameEdit->text().trimmed();
}

QString VariableDialog::getInitialValue() const {
    return valueEdit->text().trimmed();
}

bool VariableDialog::getIsConst() const {
    return constCheckBox->isChecked();
}

//I
InputDialog::InputDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("输入语句");
    setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *varLabel = new QLabel("变量名:");
    varNameEdit = new QLineEdit();
    varNameEdit->setPlaceholderText("例如: age");

    QLabel *promptLabel = new QLabel("提示信息 (可选):");
    promptEdit = new QLineEdit();
    promptEdit->setPlaceholderText("例如: 请输入您的年龄:");

    QPushButton *okButton = new QPushButton("确定");
    QPushButton *cancelButton = new QPushButton("取消");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(varLabel);
    layout->addWidget(varNameEdit);
    layout->addWidget(promptLabel);
    layout->addWidget(promptEdit);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString InputDialog::getVariableName() const {
    return varNameEdit->text().trimmed();
}

QString InputDialog::getPrompt() const {
    return promptEdit->text().trimmed();
}

//O
OutputDialog::OutputDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("输出语句");
    setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *contentLabel = new QLabel("输出内容:");
    contentEdit = new QLineEdit();
    contentEdit->setPlaceholderText("例如: \"Hello, World!\" 或 变量名");

    QPushButton *okButton = new QPushButton("确定");
    QPushButton *cancelButton = new QPushButton("取消");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(contentLabel);
    layout->addWidget(contentEdit);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString OutputDialog::getOutputContent() const {
    return contentEdit->text().trimmed();
}

//If
IfDialog::IfDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("如果语句");
    setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *conditionLabel = new QLabel("条件:");
    conditionEdit = new QLineEdit();
    conditionEdit->setPlaceholderText("例如: x > 0 或 flag == true");

    QPushButton *okButton = new QPushButton("确定");
    QPushButton *cancelButton = new QPushButton("取消");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(conditionLabel);
    layout->addWidget(conditionEdit);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString IfDialog::getCondition() const {
    return conditionEdit->text().trimmed();
}

//if..else..
IfElseDialog::IfElseDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("如果...那么...");
    setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *conditionLabel = new QLabel("条件:");
    conditionEdit = new QLineEdit();
    conditionEdit->setPlaceholderText("例如: score >= 60");

    QLabel *trueLabel = new QLabel("条件成立时执行:");
    trueEdit = new QLineEdit();
    trueEdit->setPlaceholderText("例如: cout << \"及格\" << endl;");

    QLabel *falseLabel = new QLabel("条件不成立时执行:");
    falseEdit = new QLineEdit();
    falseEdit->setPlaceholderText("例如: cout << \"不及格\" << endl;");

    QPushButton *okButton = new QPushButton("确定");
    QPushButton *cancelButton = new QPushButton("取消");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(conditionLabel);
    layout->addWidget(conditionEdit);
    layout->addWidget(trueLabel);
    layout->addWidget(trueEdit);
    layout->addWidget(falseLabel);
    layout->addWidget(falseEdit);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QString IfElseDialog::getCondition() const {
    return conditionEdit->text().trimmed();
}

QString IfElseDialog::getTrueBranch() const {
    return trueEdit->text().trimmed();
}

QString IfElseDialog::getFalseBranch() const {
    return falseEdit->text().trimmed();
}

//MAINWINDOW
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentFilePath("")
    , highlighter(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("代码生成器 - 未命名");
    highlighter = new CppHighlighter(ui->plainTextEdit->document());
    ui->plainTextEdit->installEventFilter(this);
    QFontMetrics metrics(ui->plainTextEdit->font());
    ui->plainTextEdit->setTabStopDistance(4 * metrics.horizontalAdvance(' '));
}

MainWindow::~MainWindow()
{
    delete highlighter;
    delete ui;
}


void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件: " + fileName);
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    ui->plainTextEdit->setPlainText(content);
    file.close();

    currentFilePath = fileName;
    setWindowTitle("代码生成器 - " + QFileInfo(fileName).fileName());
}

void MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法保存文件: " + fileName);
        return;
    }

    QTextStream out(&file);
    out << ui->plainTextEdit->toPlainText();
    file.close();

    currentFilePath = fileName;
    setWindowTitle("代码生成器 - " + QFileInfo(fileName).fileName());
}

void MainWindow::insertTextAtCursor(const QString &text)
{
    QTextCursor cursor = ui->plainTextEdit->textCursor();
    cursor.insertText(text);
    ui->plainTextEdit->setTextCursor(cursor);
    ui->plainTextEdit->setFocus();
}

//骨架

//引入文件
void MainWindow::on_pushButton_clicked() {
    QString includeText = getIncludeHeader();
    if (!includeText.isEmpty()) {
        insertTextAtCursor(includeText);
    }
}

//宏定义
void MainWindow::on_pushButton_2_clicked() {
    MacroDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString macroName = dialog.getMacroName();
        QString macroValue = dialog.getMacroValue();

        if (!macroName.isEmpty()) {
            QString macroText;
            if (!macroValue.isEmpty()) {
                macroText = QString("#define %1 %2\n").arg(macroName, macroValue);
            } else {
                macroText = QString("#define %1\n").arg(macroName);
            }
            insertTextAtCursor(macroText);
        }
    }
}

//新建函数
void MainWindow::on_pushButton_3_clicked() {
    FunctionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString returnType = dialog.getReturnType();
        QString funcName = dialog.getFunctionName();
        QString params = dialog.getParameters();

        if (!funcName.isEmpty()) {
            QString funcText;
            if (params.isEmpty()) {
                funcText = QString("%1 %2() {\n    // 函数体\n}\n\n")
                               .arg(returnType, funcName);
            } else {
                funcText = QString("%1 %2(%3) {\n    // 函数体\n}\n\n")
                               .arg(returnType, funcName, params);
            }
            insertTextAtCursor(funcText);
        }
    }
}

//main
void MainWindow::on_pushButton_4_clicked() {
    bool ok;
    QString argText = QInputDialog::getText(this, "main函数参数",
                                            "输入main函数的参数 (留空表示无参数):",
                                            QLineEdit::Normal, "int argc, char* argv[]", &ok);

    if (ok) {
        QString mainText;
        if (!argText.isEmpty()) {
            mainText = QString("int main(%1) {\n    // 主函数体\n    return 0;\n}\n")
                           .arg(argText);
        } else {
            mainText = QString("int main() {\n    // 主函数体\n    return 0;\n}\n");
        }
        insertTextAtCursor(mainText);
    }
}


//变量
void MainWindow::on_pushButton_7_clicked() {
    VariableDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString varType = dialog.getVariableType();
        QString varName = dialog.getVariableName();
        QString initValue = dialog.getInitialValue();
        bool isConst = dialog.getIsConst();

        if (!varName.isEmpty()) {
            QString varText;
            QString constPrefix = isConst ? "const " : "";

            if (!initValue.isEmpty()) {
                varText = QString("%1%2 %3 = %4;\n")
                .arg(constPrefix, varType, varName, initValue);
            } else {
                varText = QString("%1%2 %3;\n")
                .arg(constPrefix, varType, varName);
            }
            insertTextAtCursor(varText);
        }
    }
}

//I
void MainWindow::on_pushButton_6_clicked() {
    InputDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString varName = dialog.getVariableName();
        QString prompt = dialog.getPrompt();

        if (!varName.isEmpty()) {
            QString inputText;
            if (!prompt.isEmpty()) {
                inputText = QString("cout << \"%1\";\ncin >> %2;\n")
                .arg(prompt, varName);
            } else {
                inputText = QString("cin >> %1;\n").arg(varName);
            }
            insertTextAtCursor(inputText);
        }
    }
}

//O
void MainWindow::on_pushButton_5_clicked() {
    OutputDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString content = dialog.getOutputContent();

        if (!content.isEmpty()) {
            QString outputText;
            if (content.startsWith("\"") && content.endsWith("\"")) {
                outputText = QString("cout << %1 << endl;\n").arg(content);
            } else {
                outputText = QString("cout << \"%1\" << endl;\n").arg(content);
            }
            insertTextAtCursor(outputText);
        }
    }
}

//ret
void MainWindow::on_pushButton_8_clicked() {
    bool ok;
    QString returnValue = QInputDialog::getText(this, "返回语句",
                                                "输入返回值:", QLineEdit::Normal, "0", &ok);

    if (ok) {
        QString returnText;
        if (!returnValue.isEmpty()) {
            returnText = QString("return %1;\n").arg(returnValue);
        } else {
            returnText = "return;\n";
        }
        insertTextAtCursor(returnText);
    }
}


//if
void MainWindow::on_pushButton_9_clicked() {
    IfDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString condition = dialog.getCondition();

        if (!condition.isEmpty()) {
            QString ifText = QString("if (%1) {\n    // 条件成立时执行的代码\n}\n")
                                 .arg(condition);
            insertTextAtCursor(ifText);
        }
    }
}

// 如果...那么...
void MainWindow::on_pushButton_12_clicked() {
    IfElseDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString condition = dialog.getCondition();
        QString trueBranch = dialog.getTrueBranch();
        QString falseBranch = dialog.getFalseBranch();

        if (!condition.isEmpty()) {
            QString ifElseText;
            if (!trueBranch.isEmpty() && !falseBranch.isEmpty()) {
                ifElseText = QString("if (%1) {\n    %2\n} else {\n    %3\n}\n")
                .arg(condition, trueBranch, falseBranch);
            } else {
                ifElseText = QString("if (%1) {\n    // 条件成立时执行的代码\n} else {\n    // 条件不成立时执行的代码\n}\n")
                                 .arg(condition);
            }
            insertTextAtCursor(ifElseText);
        }
    }
}

//循环
void MainWindow::on_pushButton_10_clicked() {
    LoopDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString loopType = dialog.getLoopType();
        QString condition = dialog.getCondition();

        if (!condition.isEmpty()) {
            QString loopText;
            if (loopType == "while循环") {
                loopText = QString("while (%1) {\n    // 循环体\n}\n")
                               .arg(condition);
            } else if (loopType == "for循环") {
                QString varName = dialog.getVarName();
                QString initValue = dialog.getInitValue();
                QString step = dialog.getStep();

                loopText = QString("for (%1 %2 = %3; %4; %5) {\n    // 循环体\n}\n")
                               .arg("int", varName, initValue, condition, step);
            } else if (loopType == "do-while循环") {
                loopText = QString("do {\n    // 循环体\n} while (%1);\n")
                               .arg(condition);
            }
            insertTextAtCursor(loopText);
        }
    }
}

//if..elif..else
void MainWindow::on_pushButton_11_clicked() {
    MultiIfDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString condition1 = dialog.getCondition1();
        QString condition2 = dialog.getCondition2();
        QString trueBranch1 = dialog.getTrueBranch1();
        QString trueBranch2 = dialog.getTrueBranch2();
        QString falseBranch = dialog.getFalseBranch();

        if (!condition1.isEmpty() && !condition2.isEmpty()) {
            QString multiIfText;

            if (!trueBranch1.isEmpty() && !trueBranch2.isEmpty() && !falseBranch.isEmpty()) {
                multiIfText = QString(
                                  "if (%1) {\n"
                                  "    %2\n"
                                  "} else if (%3) {\n"
                                  "    %4\n"
                                  "} else {\n"
                                  "    %5\n"
                                  "}\n").arg(condition1, trueBranch1, condition2, trueBranch2, falseBranch);
            } else {
                multiIfText = QString(
                                  "if (%1) {\n"
                                  "    // 第一个条件成立时执行的代码\n"
                                  "} else if (%2) {\n"
                                  "    // 第二个条件成立时执行的代码\n"
                                  "} else {\n"
                                  "    // 所有条件都不成立时执行的代码\n"
                                  "}\n").arg(condition1, condition2);
            }
            insertTextAtCursor(multiIfText);
        }
    }
}


//设置
void MainWindow::on_pushButton_setting_clicked() {
    bool ok;
    int fontSize = QInputDialog::getInt(this, "字体大小设置",
                                        "请输入字体大小:", ui->plainTextEdit->font().pointSize(),
                                        8, 72, 1, &ok);

    if (ok) {
        QFont font = ui->plainTextEdit->font();
        font.setPointSize(fontSize);
        ui->plainTextEdit->setFont(font);
        ui->statusbar->showMessage(QString("字体大小已设置为: %1").arg(fontSize), 3000);
    }
}

//编译
void MainWindow::on_pushButton_13_clicked() {
    QString code = ui->plainTextEdit->toPlainText();
    if (code.isEmpty()) {
        QMessageBox::warning(this, "编译", "代码区为空");
        return;
    }

    if (currentFilePath.isEmpty()) {
        QMessageBox::warning(this, "编译", "请先保存文件");
        return;
    }

    QFileInfo fileInfo(currentFilePath);
    QString baseName = fileInfo.baseName();
    QString path = fileInfo.absolutePath();
    QString fileName = fileInfo.fileName();
    QString appDir = QCoreApplication::applicationDirPath();
    QString gccPath = appDir + "/mingw64/bin/g++.exe";

    if (!QFile::exists(gccPath)) {
        QMessageBox::critical(this, "错误",
                              QString("找不到编译器：\n%1\n\n请确保mingw64文件夹放在程序同目录下").arg(gccPath));
        return;
    }

    QString cmd = QString("cmd /c \"cd /d \"%1\" && \"%2\" -o \"%3.exe\" \"%4\"\"")
                      .arg(path, gccPath, baseName, fileName);

    int result = std::system(cmd.toStdString().c_str());

    if (result == 0) {
        ui->statusbar->showMessage("编译成功", 3000);
        QMessageBox::information(this, "编译成功",
                                 QString("程序编译成功！\n\n可执行文件：%1\\%2.exe").arg(path, baseName));
    } else {
        QMessageBox::warning(this, "编译失败",
                             QString("编译失败，返回码：%1").arg(result));
    }
}

//运行
void MainWindow::on_pushButton_14_clicked() {
    QString code = ui->plainTextEdit->toPlainText();
    if (currentFilePath.isEmpty()) {
        QMessageBox::warning(this, "运行", "请先保存文件并编译");
        return;
    }
    QFileInfo fileInfo(currentFilePath);
    QString path = fileInfo.absolutePath();
    QString baseName = fileInfo.baseName();
    QString exePath = QString("%1\\%2.exe").arg(path, baseName);
    if (!QFile::exists(exePath)) {
        QMessageBox::warning(this, "运行", "请先编译程序");
        return;
    }
    ui->statusbar->showMessage("运行中...", 3000);
    QString cmd = "start \"\" cmd /c \"\"" + exePath + "\" && pause\"";
    std::system(cmd.toStdString().c_str());
}

//编译并运行
void MainWindow::on_pushButton_15_clicked() {
    on_pushButton_13_clicked();
    on_pushButton_14_clicked();
}


//打开文件
void MainWindow::on_action_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "打开文件",
                                                    "",
                                                    "C++文件 (*.cpp *.h *.hpp);;文本文件 (*.txt);;所有文件 (*.*)");
    if (!fileName.isEmpty()) {
        loadFile(fileName);
    }
}

//保存文件
void MainWindow::on_action_2_triggered()
{
    if (currentFilePath.isEmpty()) {
        // 没有路径，弹出保存对话框
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        "保存文件",
                                                        "",
                                                        "C++文件 (*.cpp);;文本文件 (*.txt);;所有文件 (*.*)");
        if (!fileName.isEmpty()) {
            saveFile(fileName);
        }
    } else {
        // 已有路径，直接保存
        saveFile(currentFilePath);
    }
}

//新建文件夹
void MainWindow::on_action_3_triggered() {
    QString dirName = QFileDialog::getExistingDirectory(this,
                                                        "选择文件夹",
                                                        "",
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dirName.isEmpty()) {
        QMessageBox::information(this, "新建文件夹", QString("已选择文件夹: %1").arg(dirName));
    }
}

//查看代码
void MainWindow::on_action_4_triggered() {
    int lineCount = ui->plainTextEdit->toPlainText().count('\n') + 1;
    int charCount = ui->plainTextEdit->toPlainText().length();
    int wordCount = ui->plainTextEdit->toPlainText().split(QRegularExpression("\\s+"), Qt::SkipEmptyParts).size();

    QString info = QString("代码统计:\n"
                           "行数: %1\n"
                           "字符数: %2\n"
                           "单词数: %3").arg(lineCount).arg(charCount).arg(wordCount);
    QMessageBox::information(this, "查看代码", info);
}

//自动补全&&缩进
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->plainTextEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        QTextCursor cursor = ui->plainTextEdit->textCursor();
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            QString currentLine = cursor.block().text();
            int indentation = 0;
            for (int i = 0; i < currentLine.length(); i++) {
                if (currentLine[i] == ' ' || currentLine[i] == '\t') {
                    indentation++;
                } else {
                    break;
                }
            }
            QString trimmedLine = currentLine.trimmed();
            if (trimmedLine.endsWith("{") || trimmedLine.endsWith("(")) {
                indentation += 4;
            }
            cursor.insertText("\n" + QString(indentation, ' '));
            return true;
        }

        else if (keyEvent->key() == Qt::Key_BraceLeft) { // {
            cursor.insertText("{");
            cursor.insertText("}");
            cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
            return true;
        }
        else if (keyEvent->key() == Qt::Key_BracketLeft) { // [
            if (keyEvent->modifiers() & Qt::ShiftModifier) {
                return false;
            }
            cursor.insertText("[");
            cursor.insertText("]");
            cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
            return true;
        }
        else if (keyEvent->key() == Qt::Key_ParenLeft) { // (
            if (keyEvent->modifiers() & Qt::ShiftModifier) {
                cursor.insertText("(");
                cursor.insertText(")");
                cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
                return true;
            }
        }
        else if (keyEvent->key() == Qt::Key_QuoteDbl) { // "
            cursor.insertText("\"");
            cursor.insertText("\"");
            cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
            return true;
        }
        else if (keyEvent->key() == Qt::Key_Apostrophe) { // '
            cursor.insertText("'");
            cursor.insertText("'");
            cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
            return true;
        }
        else if (keyEvent->key() == Qt::Key_Backspace) {
            cursor.clearSelection();
            int position = cursor.position();
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
            QString nextChar = cursor.selectedText();
            cursor.setPosition(position);

            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 1);
            QString prevChar = cursor.selectedText();
            cursor.setPosition(position);
            bool isPaired = false;
            if (prevChar == "{" && nextChar == "}") isPaired = true;
            else if (prevChar == "[" && nextChar == "]") isPaired = true;
            else if (prevChar == "(" && nextChar == ")") isPaired = true;
            else if (prevChar == "\"" && nextChar == "\"") isPaired = true;
            else if (prevChar == "'" && nextChar == "'") isPaired = true;

            if (isPaired) {
                cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 1);
                cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
                cursor.removeSelectedText();
                return true;
            }
        }
        else if (keyEvent->key() == Qt::Key_Tab) {
            cursor.insertText("    ");
            return true;
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

//关于
void MainWindow::on_action_5_triggered() {
    QMessageBox::information(this, "关于",
                             "王导出品必属精品\n"
                             "命名空间啥的自己加🤓");
}


//注释
void MainWindow::on_pushButton_16_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("插入注释");
    dialog.setModal(true);
    dialog.setMinimumSize(400, 250);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);

    QTabWidget *tabWidget = new QTabWidget(&dialog);

    // //
    QWidget *singleLineTab = new QWidget();
    QVBoxLayout *singleLineLayout = new QVBoxLayout(singleLineTab);

    QLabel *singleLabel = new QLabel("单行注释:");
    QLineEdit *singleLineEdit = new QLineEdit();

    singleLineLayout->addWidget(singleLabel);
    singleLineLayout->addWidget(singleLineEdit);
    singleLineLayout->addStretch();

    // /**/
    QWidget *multiLineTab = new QWidget();
    QVBoxLayout *multiLineLayout = new QVBoxLayout(multiLineTab);

    QLabel *multiLabel = new QLabel("多行注释:");
    QTextEdit *multiLineEdit = new QTextEdit();
    multiLineEdit->setMinimumHeight(120);

    multiLineLayout->addWidget(multiLabel);
    multiLineLayout->addWidget(multiLineEdit);

    tabWidget->addTab(singleLineTab, "单行注释 //");
    tabWidget->addTab(multiLineTab, "多行注释 /* */");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("确定");
    QPushButton *cancelButton = new QPushButton("取消");

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addWidget(tabWidget);
    mainLayout->addLayout(buttonLayout);

    QString singleComment;
    QString multiComment;
    int selectedTab = 0;

    connect(okButton, &QPushButton::clicked, [&]() {
        selectedTab = tabWidget->currentIndex();
        singleComment = singleLineEdit->text().trimmed();
        multiComment = multiLineEdit->toPlainText().trimmed();
        dialog.accept();
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        if (selectedTab == 0 && !singleComment.isEmpty()) {
            QTextCursor textCursor = ui->plainTextEdit->textCursor();
            textCursor.insertText("// " + singleComment);
        }
        else if (selectedTab == 1 && !multiComment.isEmpty()) {
            QTextCursor textCursor = ui->plainTextEdit->textCursor();
            textCursor.insertText("/* " + multiComment + " */");
        }
    }
}

