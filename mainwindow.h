#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QRadioButton>
#include <QProgressBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QGroupBox>
#include "huffmancompressor.h"
#include "rlecompressor.h"
#include "lzwcompressor.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QWidget* centralWidget;
    QLabel* titleLabel;
    QPushButton* selectFileBtn;
    QLabel* filePathLabel;
    QGroupBox* operationGroup;
    QRadioButton* compressRadio;
    QRadioButton* decompressRadio;
    QGroupBox* algorithmGroup;
    QComboBox* algorithmCombo;
    QPushButton* processBtn;
    QProgressBar* progressBar;
    QTextEdit* logOutput;

    QString selectedFilePath;
    HuffmanCompressor* huffmanComp;
    RLECompressor* rleComp;
    LZWCompressor* lzwComp;

    void setupUI();
    void connectSignals();

private slots:
    void selectFile();
    void processFile();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif
