#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cpphighlighter.h"
#include <QMainWindow>
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QStatusBar>
#include <QFile>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MacroDialog : public QDialog {
    Q_OBJECT
public:
    explicit MacroDialog(QWidget *parent = nullptr);
    QString getMacroName() const;
    QString getMacroValue() const;

private:
    QLineEdit *nameEdit;
    QLineEdit *valueEdit;
};

class FunctionDialog : public QDialog {
    Q_OBJECT
public:
    explicit FunctionDialog(QWidget *parent = nullptr);
    QString getReturnType() const;
    QString getFunctionName() const;
    QString getParameters() const;

private:
    QComboBox *returnTypeCombo;
    QLineEdit *nameEdit;
    QLineEdit *paramsEdit;
};

class VariableDialog : public QDialog {
    Q_OBJECT
public:
    explicit VariableDialog(QWidget *parent = nullptr);
    QString getVariableType() const;
    QString getVariableName() const;
    QString getInitialValue() const;
    bool getIsConst() const;

private:
    QComboBox *typeCombo;
    QLineEdit *nameEdit;
    QLineEdit *valueEdit;
    QCheckBox *constCheckBox;
};

class InputDialog : public QDialog {
    Q_OBJECT
public:
    explicit InputDialog(QWidget *parent = nullptr);
    QString getVariableName() const;
    QString getPrompt() const;

private:
    QLineEdit *varNameEdit;
    QLineEdit *promptEdit;
};

class OutputDialog : public QDialog {
    Q_OBJECT
public:
    explicit OutputDialog(QWidget *parent = nullptr);
    QString getOutputContent() const;

private:
    QLineEdit *contentEdit;
};

class IfDialog : public QDialog {
    Q_OBJECT
public:
    explicit IfDialog(QWidget *parent = nullptr);
    QString getCondition() const;

private:
    QLineEdit *conditionEdit;
};

class IfElseDialog : public QDialog {
    Q_OBJECT
public:
    explicit IfElseDialog(QWidget *parent = nullptr);
    QString getCondition() const;
    QString getTrueBranch() const;
    QString getFalseBranch() const;

private:
    QLineEdit *conditionEdit;
    QLineEdit *trueEdit;
    QLineEdit *falseEdit;
};

class MultiIfDialog : public QDialog {
    Q_OBJECT
public:
    explicit MultiIfDialog(QWidget *parent = nullptr);
    QString getCondition1() const;
    QString getCondition2() const;
    QString getTrueBranch1() const;
    QString getTrueBranch2() const;
    QString getFalseBranch() const;

private:
    QLineEdit *condition1Edit;
    QLineEdit *condition2Edit;
    QLineEdit *true1Edit;
    QLineEdit *true2Edit;
    QLineEdit *falseEdit;
};

class LoopDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoopDialog(QWidget *parent = nullptr);
    QString getLoopType() const;
    QString getCondition() const;
    QString getInitValue() const;
    QString getStep() const;
    QString getVarName() const;

private slots:
    void onLoopTypeChanged(int index);

private:
    QComboBox *typeCombo;
    QLineEdit *conditionEdit;
    QLineEdit *initValueEdit;
    QLineEdit *stepEdit;
    QLineEdit *varNameEdit;
    QLabel *initLabel;
    QLabel *stepLabel;
    QLabel *varLabel;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_setting_clicked();
    void on_pushButton_13_clicked();
    void on_pushButton_14_clicked();
    void on_pushButton_15_clicked();
    void on_action_triggered();
    void on_action_2_triggered();
    void on_action_3_triggered();
    void on_action_4_triggered();
    void on_action_5_triggered();

    void on_pushButton_16_clicked();

private:
    void insertTextAtCursor(const QString &text);
    void setupConnections();
    void loadFile(const QString &fileName);
    void saveFile(const QString &fileName);

private:
    Ui::MainWindow *ui;
    QString currentFilePath;
    CppHighlighter *highlighter;
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // MAINWINDOW_H
