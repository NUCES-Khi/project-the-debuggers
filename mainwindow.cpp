#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDateTime>
#include <QGraphicsDropShadowEffect>
#include "huffmancompressor.h"
#include "rlecompressor.h"
#include "lzwcompressor.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    huffmanComp = new HuffmanCompressor();
    rleComp = new RLECompressor();
    lzwComp = new LZWCompressor();
    setupUI();
}

MainWindow::~MainWindow() {
    delete huffmanComp;
    delete rleComp;
    delete lzwComp;
}

void MainWindow::setupUI() {
    // Set window properties
    setWindowTitle("Compression Studio Pro");
    setMinimumSize(850, 900);
    resize(850, 900);

    // Main widget with gradient background
    centralWidget = new QWidget(this);
    centralWidget->setStyleSheet(
        "QWidget#centralWidget {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "       stop:0 #1a1a2e, stop:0.5 #16213e, stop:1 #0f3460);"
        "}"
        );
    centralWidget->setObjectName("centralWidget");

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    // ========== HEADER SECTION ==========
    QWidget* headerWidget = new QWidget();
    headerWidget->setMaximumHeight(100);
    headerWidget->setStyleSheet(
        "background: rgba(255, 255, 255, 0.05);"
        "border-radius: 12px;"
        "border: 2px solid rgba(255, 255, 255, 0.1);"
        );
    QVBoxLayout* headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setContentsMargins(20, 15, 20, 15);
    headerLayout->setSpacing(5);

    // Title with icon
    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setSpacing(10);

    QLabel* iconLabel = new QLabel("🗜️");
    iconLabel->setStyleSheet("font-size: 32px; background: transparent; border: none;");
    titleLayout->addWidget(iconLabel);

    titleLabel = new QLabel("Compression Studio Pro");
    titleLabel->setStyleSheet(
        "font-size: 26px;"
        "font-weight: bold;"
        "color: #00d4ff;"
        "background: transparent;"
        "border: none;"
        "letter-spacing: 1px;"
        );
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();

    headerLayout->addLayout(titleLayout);

    QLabel* subtitleLabel = new QLabel("Advanced File Compression System • DSA Project 2024");
    subtitleLabel->setStyleSheet(
        "font-size: 12px;"
        "color: rgba(255, 255, 255, 0.6);"
        "background: transparent;"
        "border: none;"
        );
    headerLayout->addWidget(subtitleLabel);

    mainLayout->addWidget(headerWidget);

    // ========== FILE SELECTION CARD ==========
    QWidget* fileCard = new QWidget();
    fileCard->setMaximumHeight(180);
    fileCard->setStyleSheet(
        "background: rgba(255, 255, 255, 0.08);"
        "border-radius: 12px;"
        "border: 2px solid rgba(255, 255, 255, 0.15);"
        );
    QVBoxLayout* fileLayout = new QVBoxLayout(fileCard);
    fileLayout->setContentsMargins(20, 18, 20, 18);
    fileLayout->setSpacing(12);

    QLabel* fileLabel = new QLabel("📂 File Selection");
    fileLabel->setStyleSheet(
        "font-size: 15px;"
        "font-weight: bold;"
        "color: #ffffff;"
        "background: transparent;"
        "border: none;"
        );
    fileLayout->addWidget(fileLabel);

    selectFileBtn = new QPushButton("📁  Select File to Process");
    selectFileBtn->setCursor(Qt::PointingHandCursor);
    selectFileBtn->setMinimumHeight(45);
    selectFileBtn->setMaximumHeight(45);
    selectFileBtn->setStyleSheet(
        "QPushButton {"
        "   padding: 12px 25px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00d4ff, stop:1 #0099cc);"
        "   color: #ffffff;"
        "   border: none;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00e5ff, stop:1 #00b3e6);"
        "}"
        "QPushButton:pressed {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00a8cc, stop:1 #007799);"
        "}"
        );
    fileLayout->addWidget(selectFileBtn);

    filePathLabel = new QLabel("No file selected");
    filePathLabel->setWordWrap(true);
    filePathLabel->setMinimumHeight(50);
    filePathLabel->setMaximumHeight(50);
    filePathLabel->setStyleSheet(
        "padding: 12px 15px;"
        "background: rgba(0, 0, 0, 0.4);"
        "color: #ffffff;"
        "border-radius: 8px;"
        "border: 2px dashed rgba(0, 212, 255, 0.3);"
        "font-size: 13px;"
        );
    fileLayout->addWidget(filePathLabel);

    mainLayout->addWidget(fileCard);

    // ========== SETTINGS CARD ==========
    QWidget* settingsCard = new QWidget();
    settingsCard->setMaximumHeight(220);
    settingsCard->setStyleSheet(
        "background: rgba(255, 255, 255, 0.08);"
        "border-radius: 12px;"
        "border: 2px solid rgba(255, 255, 255, 0.15);"
        );
    QVBoxLayout* settingsLayout = new QVBoxLayout(settingsCard);
    settingsLayout->setContentsMargins(20, 18, 20, 18);
    settingsLayout->setSpacing(12);

    QLabel* settingsLabel = new QLabel("⚙️ Configuration");
    settingsLabel->setStyleSheet(
        "font-size: 15px;"
        "font-weight: bold;"
        "color: #ffffff;"
        "background: transparent;"
        "border: none;"
        );
    settingsLayout->addWidget(settingsLabel);

    // Operation Selection - Modern Radio Buttons
    QLabel* opLabel = new QLabel("Operation Mode");
    opLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #ffffff;"
        "background: transparent;"
        "border: none;"
        "font-weight: bold;"
        "margin-bottom: 3px;"
        );
    settingsLayout->addWidget(opLabel);

    QHBoxLayout* radioLayout = new QHBoxLayout();
    radioLayout->setSpacing(15);

    compressRadio = new QRadioButton("  🗜️  Compress");
    compressRadio->setChecked(true);
    compressRadio->setStyleSheet(
        "QRadioButton {"
        "   color: #ffffff;"
        "   font-size: 14px;"
        "   font-weight: 500;"
        "   spacing: 8px;"
        "   background: transparent;"
        "   padding: 5px;"
        "}"
        "QRadioButton::indicator {"
        "   width: 18px;"
        "   height: 18px;"
        "}"
        "QRadioButton::indicator:unchecked {"
        "   background: rgba(255, 255, 255, 0.15);"
        "   border: 2px solid rgba(255, 255, 255, 0.4);"
        "   border-radius: 9px;"
        "}"
        "QRadioButton::indicator:checked {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "       stop:0 #00d4ff, stop:1 #0099cc);"
        "   border: 2px solid #00d4ff;"
        "   border-radius: 9px;"
        "}"
        "QRadioButton::indicator:hover {"
        "   border: 2px solid #00d4ff;"
        "}"
        );

    decompressRadio = new QRadioButton("  📦  Decompress");
    decompressRadio->setStyleSheet(compressRadio->styleSheet());

    radioLayout->addWidget(compressRadio);
    radioLayout->addWidget(decompressRadio);
    radioLayout->addStretch();

    settingsLayout->addLayout(radioLayout);

    // Algorithm Selection
    QLabel* algoLabel = new QLabel("Compression Algorithm");
    algoLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #ffffff;"
        "background: transparent;"
        "border: none;"
        "font-weight: bold;"
        "margin-top: 5px;"
        "margin-bottom: 3px;"
        );
    settingsLayout->addWidget(algoLabel);

    algorithmCombo = new QComboBox();
    algorithmCombo->addItem("  🎯  Huffman Encoding - Optimal for text & varied data");
    algorithmCombo->addItem("  🔄  Run-Length Encoding (RLE) - Best for repetitive data");
    algorithmCombo->addItem("  📚  LZW Compression - Dictionary-based patterns");
    algorithmCombo->setCursor(Qt::PointingHandCursor);
    algorithmCombo->setMinimumHeight(40);
    algorithmCombo->setMaximumHeight(40);
    algorithmCombo->setStyleSheet(
        "QComboBox {"
        "   padding: 10px 12px;"
        "   font-size: 13px;"
        "   background: rgba(255, 255, 255, 0.12);"
        "   color: #ffffff;"
        "   border: 2px solid rgba(0, 212, 255, 0.3);"
        "   border-radius: 8px;"
        "}"
        "QComboBox:hover {"
        "   border: 2px solid #00d4ff;"
        "   background: rgba(255, 255, 255, 0.18);"
        "}"
        "QComboBox::drop-down {"
        "   border: none;"
        "   padding-right: 15px;"
        "   width: 25px;"
        "}"
        "QComboBox::down-arrow {"
        "   image: none;"
        "   border-left: 5px solid transparent;"
        "   border-right: 5px solid transparent;"
        "   border-top: 7px solid #00d4ff;"
        "   margin-right: 10px;"
        "}"
        "QComboBox QAbstractItemView {"
        "   background: #16213e;"
        "   color: #ffffff;"
        "   selection-background-color: #00d4ff;"
        "   selection-color: #ffffff;"
        "   border: 2px solid #00d4ff;"
        "   padding: 5px;"
        "   outline: none;"
        "   font-size: 13px;"
        "}"
        "QComboBox QAbstractItemView::item {"
        "   padding: 8px;"
        "   border-radius: 4px;"
        "   margin: 2px;"
        "}"
        "QComboBox QAbstractItemView::item:hover {"
        "   background: rgba(0, 212, 255, 0.3);"
        "}"
        );
    settingsLayout->addWidget(algorithmCombo);

    mainLayout->addWidget(settingsCard);

    // ========== PROCESS BUTTON ==========
    processBtn = new QPushButton("▶️  START PROCESSING");
    processBtn->setEnabled(false);
    processBtn->setCursor(Qt::PointingHandCursor);
    processBtn->setMinimumHeight(50);
    processBtn->setMaximumHeight(50);
    processBtn->setStyleSheet(
        "QPushButton {"
        "   padding: 15px;"
        "   font-size: 15px;"
        "   font-weight: bold;"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00ff88, stop:1 #00cc66);"
        "   color: #ffffff;"
        "   border: none;"
        "   border-radius: 10px;"
        "   letter-spacing: 1px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00ff99, stop:1 #00dd77);"
        "}"
        "QPushButton:pressed {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00cc77, stop:1 #00aa55);"
        "}"
        "QPushButton:disabled {"
        "   background: rgba(255, 255, 255, 0.1);"
        "   color: rgba(255, 255, 255, 0.3);"
        "}"
        );
    mainLayout->addWidget(processBtn);

    // ========== PROGRESS BAR ==========
    progressBar = new QProgressBar();
    progressBar->setVisible(false);
    progressBar->setMinimumHeight(6);
    progressBar->setMaximumHeight(6);
    progressBar->setTextVisible(false);
    progressBar->setStyleSheet(
        "QProgressBar {"
        "   border: none;"
        "   border-radius: 3px;"
        "   background: rgba(0, 0, 0, 0.3);"
        "}"
        "QProgressBar::chunk {"
        "   border-radius: 3px;"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #00d4ff, stop:1 #00ff88);"
        "}"
        );
    mainLayout->addWidget(progressBar);

    // ========== LOG OUTPUT ==========
    QWidget* logCard = new QWidget();
    logCard->setStyleSheet(
        "background: rgba(0, 0, 0, 0.4);"
        "border-radius: 12px;"
        "border: 2px solid rgba(255, 255, 255, 0.1);"
        );
    QVBoxLayout* logLayout = new QVBoxLayout(logCard);
    logLayout->setContentsMargins(18, 15, 18, 15);
    logLayout->setSpacing(8);

    QLabel* logLabel = new QLabel("📊 Processing Log");
    logLabel->setStyleSheet(
        "font-size: 14px;"
        "font-weight: bold;"
        "color: #00d4ff;"
        "background: transparent;"
        "border: none;"
        );
    logLayout->addWidget(logLabel);

    logOutput = new QTextEdit();
    logOutput->setReadOnly(true);
    logOutput->setMinimumHeight(200);
    logOutput->setMaximumHeight(250);
    logOutput->setStyleSheet(
        "QTextEdit {"
        "   background: rgba(0, 0, 0, 0.6);"
        "   color: #00ff88;"
        "   padding: 12px;"
        "   font-family: 'Consolas', 'Courier New', monospace;"
        "   font-size: 11px;"
        "   border: 1px solid rgba(255, 255, 255, 0.1);"
        "   border-radius: 8px;"
        "   selection-background-color: #00d4ff;"
        "}"
        "QScrollBar:vertical {"
        "   background: rgba(255, 255, 255, 0.05);"
        "   width: 10px;"
        "   border-radius: 5px;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: rgba(0, 212, 255, 0.5);"
        "   border-radius: 5px;"
        "   min-height: 20px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "   background: rgba(0, 212, 255, 0.8);"
        "}"
        );
    logLayout->addWidget(logOutput);

    mainLayout->addWidget(logCard);

    // ========== FOOTER ==========
    QLabel* footer = new QLabel(
        "💻 DSA Project 2024  •  Muhammad Amaan Jilani (K24-2540)  •  Ahmed Raza (K24-2515)"
        );
    footer->setAlignment(Qt::AlignCenter);
    footer->setStyleSheet(
        "color: rgba(255, 255, 255, 0.4);"
        "font-size: 10px;"
        "padding: 10px;"
        "background: transparent;"
        "border: none;"
        );
    mainLayout->addWidget(footer);

    setCentralWidget(centralWidget);
    connectSignals();

    // Initial log messages
    logOutput->append("<span style='color:#00d4ff;font-weight:bold;'>═══════════════════════════════════════════════</span>");
    logOutput->append("<span style='color:#00ff88;'>✓</span> <span style='color:#ffffff;'>Compression Studio Pro Initialized</span>");
    logOutput->append("<span style='color:#00d4ff;'>═══════════════════════════════════════════════</span>");
    logOutput->append("<span style='color:#00ff88;'>✓</span> Huffman Encoding <span style='color:#666;'>(Binary optimal)</span>");
    logOutput->append("<span style='color:#00ff88;'>✓</span> Run-Length Encoding <span style='color:#666;'>(Repetition specialist)</span>");
    logOutput->append("<span style='color:#00ff88;'>✓</span> LZW Compression <span style='color:#666;'>(Pattern recognition)</span>");
    logOutput->append("<span style='color:#00d4ff;'>═══════════════════════════════════════════════</span>");
    logOutput->append("<span style='color:#ffaa00;'>⚡</span> <span style='color:#ffffff;'>Ready to process files...</span>\n");
}

void MainWindow::connectSignals() {
    connect(selectFileBtn, &QPushButton::clicked, this, &MainWindow::selectFile);
    connect(processBtn, &QPushButton::clicked, this, &MainWindow::processFile);
}

void MainWindow::selectFile() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Select File to Process",
        "",
        "All Files (*.*)"
        );

    if(!filePath.isEmpty()) {
        selectedFilePath = filePath;

        // Extract filename
        QString fileName = filePath.split("/").last();
        if(fileName.isEmpty()) fileName = filePath.split("\\").last();

        filePathLabel->setText("📄 " + fileName + "\n🗂️ " + filePath);
        processBtn->setEnabled(true);

        logOutput->append("<span style='color:#00d4ff;'>───────────────────────────────────────────────</span>");
        logOutput->append("<span style='color:#ffaa00;'>📁</span> <span style='color:#ffffff;'>File Selected:</span> <span style='color:#00d4ff;'>" + fileName + "</span>");

        QFile file(filePath);
        if(file.open(QIODevice::ReadOnly)) {
            qint64 size = file.size();
            file.close();

            QString sizeStr;
            if(size < 1024) {
                sizeStr = QString::number(size) + " bytes";
            } else if(size < 1024 * 1024) {
                sizeStr = QString::number(size / 1024.0, 'f', 2) + " KB";
            } else {
                sizeStr = QString::number(size / (1024.0 * 1024.0), 'f', 2) + " MB";
            }

            logOutput->append("<span style='color:#00ff88;'>📊</span> <span style='color:#ffffff;'>File Size:</span> <span style='color:#00ff88;'>" + sizeStr + "</span>");
        }
    }
}

void MainWindow::processFile() {
    if(selectedFilePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a file first!");
        return;
    }

    processBtn->setEnabled(false);
    progressBar->setVisible(true);
    progressBar->setValue(0);

    QFile inputFile(selectedFilePath);
    if(!inputFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Cannot open input file!");
        progressBar->setVisible(false);
        processBtn->setEnabled(true);
        return;
    }

    QByteArray fileData = inputFile.readAll();
    inputFile.close();

    progressBar->setValue(20);

    logOutput->append("\n<span style='color:#00d4ff;font-weight:bold;'>═══════════════════════════════════════════════</span>");
    logOutput->append("<span style='color:#ffaa00;'>⚡</span> <span style='color:#ffffff;font-weight:bold;'>Processing Started...</span>");

    QString sizeStr;
    if(fileData.size() < 1024) {
        sizeStr = QString::number(fileData.size()) + " bytes";
    } else if(fileData.size() < 1024 * 1024) {
        sizeStr = QString::number(fileData.size() / 1024.0, 'f', 2) + " KB";
    } else {
        sizeStr = QString::number(fileData.size() / (1024.0 * 1024.0), 'f', 2) + " MB";
    }

    logOutput->append("<span style='color:#ffffff;'>📏 Original Size:</span> <span style='color:#00ff88;'>" + sizeStr + "</span>");

    QByteArray result;
    QString outputPath;
    int selectedAlgo = algorithmCombo->currentIndex();
    bool isCompress = compressRadio->isChecked();

    QDateTime startTime = QDateTime::currentDateTime();

    progressBar->setValue(40);

    try {
        if(isCompress) {
            logOutput->append("<span style='color:#00d4ff;'>🗜️  Operation:</span> <span style='color:#ffffff;'>Compression</span>");
            switch(selectedAlgo) {
            case 0:
                logOutput->append("<span style='color:#00ff88;'>🎯 Algorithm:</span> <span style='color:#ffffff;'>Huffman Encoding</span>");
                result = huffmanComp->compress(fileData);
                outputPath = selectedFilePath + ".huff";
                break;
            case 1:
                logOutput->append("<span style='color:#00ff88;'>🔄 Algorithm:</span> <span style='color:#ffffff;'>Run-Length Encoding</span>");
                result = rleComp->compress(fileData);
                outputPath = selectedFilePath + ".rle";

                if(result.size() >= 8 &&
                    (unsigned char)result[0] == 0xFF &&
                    (unsigned char)result[1] == 0xFF &&
                    (unsigned char)result[2] == 0xFF &&
                    (unsigned char)result[3] == 0xFF) {
                    logOutput->append("<span style='color:#ffaa00;'>⚠️</span> <span style='color:#ffffff;'>No repetition detected - stored uncompressed</span>");
                }
                break;
            case 2:
                logOutput->append("<span style='color:#00ff88;'>📚 Algorithm:</span> <span style='color:#ffffff;'>LZW Compression</span>");
                result = lzwComp->compress(fileData);
                outputPath = selectedFilePath + ".lzw";
                break;
            }

            if(result.isEmpty()) {
                throw std::runtime_error("Compression failed");
            }

            if(result.size() >= fileData.size()) {
                logOutput->append("<span style='color:#ff6b6b;'>⚠️ WARNING:</span> <span style='color:#ffffff;'>Compressed ≥ Original size</span>");
            } else {
                logOutput->append("<span style='color:#00ff88;'>✓ Compression successful!</span>");
            }

        } else {
            logOutput->append("<span style='color:#00d4ff;'>📦 Operation:</span> <span style='color:#ffffff;'>Decompression</span>");

            QString basePath = selectedFilePath;

            if(basePath.endsWith(".huff")) {
                basePath = basePath.left(basePath.length() - 5);
                outputPath = basePath;
            } else if(basePath.endsWith(".rle")) {
                basePath = basePath.left(basePath.length() - 4);
                outputPath = basePath;
            } else if(basePath.endsWith(".lzw")) {
                basePath = basePath.left(basePath.length() - 4);
                outputPath = basePath;
            } else {
                outputPath = selectedFilePath + ".decompressed";
            }

            QFile testFile(outputPath);
            if(testFile.exists()) {
                outputPath = basePath + "_restored_" +
                             QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");

                int lastDot = basePath.lastIndexOf('.');
                if(lastDot > 0) {
                    QString ext = basePath.mid(lastDot);
                    outputPath += ext;
                }
            }

            switch(selectedAlgo) {
            case 0:
                logOutput->append("<span style='color:#00ff88;'>🎯 Algorithm:</span> <span style='color:#ffffff;'>Huffman Decoding</span>");
                result = huffmanComp->decompress(fileData);
                break;
            case 1:
                logOutput->append("<span style='color:#00ff88;'>🔄 Algorithm:</span> <span style='color:#ffffff;'>RLE Decompression</span>");
                result = rleComp->decompress(fileData);
                break;
            case 2:
                logOutput->append("<span style='color:#00ff88;'>📚 Algorithm:</span> <span style='color:#ffffff;'>LZW Decompression</span>");
                result = lzwComp->decompress(fileData);
                break;
            }

            if(result.isEmpty()) {
                throw std::runtime_error("Decompression failed");
            }

            logOutput->append("<span style='color:#00d4ff;'>💾 Restoring to:</span> <span style='color:#ffffff;'>" + outputPath.split("/").last() + "</span>");
        }
    } catch(...) {
        QMessageBox::critical(this, "Error", "Processing failed!");
        progressBar->setVisible(false);
        processBtn->setEnabled(true);
        logOutput->append("<span style='color:#ff6b6b;'>❌ ERROR: Processing failed!</span>");
        return;
    }

    progressBar->setValue(70);

    QDateTime endTime = QDateTime::currentDateTime();
    qint64 elapsedMs = startTime.msecsTo(endTime);

    QFile outputFile(outputPath);
    if(outputFile.open(QIODevice::WriteOnly)) {
        outputFile.write(result);
        outputFile.close();

        progressBar->setValue(100);

        QString resultSizeStr;
        if(result.size() < 1024) {
            resultSizeStr = QString::number(result.size()) + " bytes";
        } else if(result.size() < 1024 * 1024) {
            resultSizeStr = QString::number(result.size() / 1024.0, 'f', 2) + " KB";
        } else {
            resultSizeStr = QString::number(result.size() / (1024.0 * 1024.0), 'f', 2) + " MB";
        }

        logOutput->append("<span style='color:#ffffff;'>📦 Output Size:</span> <span style='color:#00ff88;'>" + resultSizeStr + "</span>");

        if(isCompress && fileData.size() > 0) {
            double ratio = 100.0 * result.size() / fileData.size();
            logOutput->append("<span style='color:#ffffff;'>📊 Compression Ratio:</span> <span style='color:#00d4ff;'>" +
                              QString::number(ratio, 'f', 2) + "%</span>");

            qint64 saved = fileData.size() - result.size();
            if(saved > 0) {
                QString savedStr;
                if(saved < 1024) {
                    savedStr = QString::number(saved) + " bytes";
                } else if(saved < 1024 * 1024) {
                    savedStr = QString::number(saved / 1024.0, 'f', 2) + " KB";
                } else {
                    savedStr = QString::number(saved / (1024.0 * 1024.0), 'f', 2) + " MB";
                }

                logOutput->append("<span style='color:#00ff88;'>💾 Space Saved:</span> <span style='color:#00ff88;'>" +
                                  savedStr + " (" + QString::number(100.0 * saved / fileData.size(), 'f', 1) + "%)</span>");
            } else {
                logOutput->append("<span style='color:#ff6b6b;'>📈 Space Lost:</span> <span style='color:#ff6b6b;'>" +
                                  QString::number(-saved) + " bytes</span>");
            }
        }

        logOutput->append("<span style='color:#ffffff;'>⏱️  Processing Time:</span> <span style='color:#00d4ff;'>" +
                          QString::number(elapsedMs) + " ms</span>");
        logOutput->append("<span style='color:#00ff88;'>✓ Output saved:</span> <span style='color:#ffffff;'>" +
                          outputPath + "</span>");
        logOutput->append("<span style='color:#00d4ff;font-weight:bold;'>═══════════════════════════════════════════════</span>");
        logOutput->append("<span style='color:#00ff88;font-weight:bold;'>✓ Processing Complete!</span>\n");

        QString statusMsg;

        if(isCompress) {
            statusMsg = QString("✅ File Compressed Successfully!\n\n"
                                "📏 Original: %1\n"
                                "📦 Compressed: %2\n"
                                "⏱️  Time: %3 ms\n\n"
                                "⚠️  Note: Compressed file is binary format.\n"
                                "Use 'Decompress' to restore original.\n\n"
                                "💾 Saved to:\n%4")
                            .arg(sizeStr)
                            .arg(resultSizeStr)
                            .arg(elapsedMs)
                            .arg(outputPath);
        } else {
            statusMsg = QString("✅ File Decompressed Successfully!\n\n"
                                "📦 Compressed: %1\n"
                                "📄 Restored: %2\n"
                                "⏱️  Time: %3 ms\n\n"
                                "✓ File can now be opened normally!\n\n"
                                "💾 Saved to:\n%4")
                            .arg(sizeStr)
                            .arg(resultSizeStr)
                            .arg(elapsedMs)
                            .arg(outputPath);
        }

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Success!");
        msgBox.setText(statusMsg);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(
            "QMessageBox {"
            "   background: #16213e;"
            "}"
            "QMessageBox QLabel {"
            "   color: #ffffff;"
            "   font-size: 13px;"
            "}"
            "QPushButton {"
            "   background: #00d4ff;"
            "   color: white;"
            "   border: none;"
            "   padding: 8px 20px;"
            "   border-radius: 5px;"
            "   font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "   background: #00e5ff;"
            "}"
            );
        msgBox.exec();
    } else {
        QMessageBox::critical(this, "Error", "Cannot write output file!");
        logOutput->append("<span style='color:#ff6b6b;'>❌ ERROR: Cannot write output file!</span>");
    }

    progressBar->setVisible(false);
    processBtn->setEnabled(true);
}
