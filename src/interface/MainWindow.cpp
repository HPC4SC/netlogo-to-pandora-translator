#include "MainWindow.hpp"
#include <QGridLayout>
#include <QFileDialog>
#include <QGroupBox>
#include <QLineEdit>
#include <QProcess>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent) {

    QFont appFont("Montserrat");
    appFont.setStyleHint(QFont::Monospace);

    QFont titleFont("Scada");
    titleFont.setStyleHint(QFont::Monospace);

    QApplication::setFont(appFont);

    this->setContentsMargins(20, 0, 20, 20);

    this->setStyleSheet(
                    "QWidget {"
                        "background-color: white;"
                    "}"
                    "QPushButton {"
                        "background-color: white;"
                        "border: 1px solid black;"
                        "border-radius: 2px;"
                        "padding: 10px;"
                        "font-size: 18px"
                    "}"
                    "QPushButton:hover {"
                        "background-color: white;"
                        "border: 2px solid black;"
                        "border-radius: 2px;"
                        "padding: 10px;"
                    "}"
                    "QLineEdit {"
                        "background-color: white;"
                        "border: 1px solid black;"
                        "padding: 10px;"
                        "font-size: 18px"
                    "}");
 
    QGroupBox *group = new QGroupBox(tr("Echo"));
        
    QLabel *title = new QLabel(this);
    title->setText("NetLogo to Pandora\nTranslator");
    title->setStyleSheet(   "font-size: 24px;"
                            "font-width: 700;");
    title->setAlignment(Qt::AlignCenter);
    title->setFont(titleFont);
    title->show();

    QLabel *upc = new QLabel(this);
    upc->setPixmap(QPixmap("upc.png"));
    upc->show();

    QLabel *bsc = new QLabel(this);
    bsc->setPixmap(QPixmap("bsc.png"));
    bsc->show();

    QPushButton *inputBtn = new QPushButton("Select NetLogo model file", this);
    modelFileTextBox = new QLineEdit;
    modelFileTextBox->setPlaceholderText("No model file selected");

    QPushButton *outputBtn = new QPushButton("Select output directory", this);
    outputDirectoryTextBox = new QLineEdit;
    outputDirectoryTextBox->setPlaceholderText("No output directory selected");

    QPushButton *translateBtn = new QPushButton("Translate to C++", this);
    translateBtn->setStyleSheet("background-color: #007AC1;"
                                "border-color: white;"
                                "border-radius: 4px;"
                                "color: white;"
                                "font-size: 22px;"
                                "font-width: bold;");

    QPushButton *translatePandoraBtn = new QPushButton("Translate to Pandora", this);
    translatePandoraBtn->setStyleSheet("background-color: #007AC1;"
                                "border-color: white;"
                                "border-radius: 4px;"
                                "color: white;"
                                "font-size: 22px;"
                                "font-width: bold;");

    QGridLayout *grid = new QGridLayout(this);

    grid->addWidget(title, 0, 0);
    grid->addWidget(upc, 0, 1);
    grid->addWidget(bsc, 0, 2);
    grid->addWidget(inputBtn, 1, 0);
    grid->addWidget(modelFileTextBox, 1, 1, 1, 2);
    grid->addWidget(outputBtn, 2, 0);
    grid->addWidget(outputDirectoryTextBox, 2, 1, 1, 2);
    grid->addWidget(translateBtn, 3, 1);
    grid->addWidget(translatePandoraBtn, 3, 2);
    group->setLayout(grid);

    setLayout(grid);  

    connect(inputBtn, &QPushButton::clicked, this, &MainWindow::OpenFile);
    connect(outputBtn, &QPushButton::clicked, this, &MainWindow::OpenDir);
    connect(translateBtn, &QPushButton::clicked, this, &MainWindow::Translate);
    connect(translatePandoraBtn, &QPushButton::clicked, this, &MainWindow::TranslatePandora);
}

void MainWindow::OpenFile() {
    
    inputFileName = QFileDialog::getOpenFileName(this,
        tr("Open model source"), "",
        tr("NetLogo Model (*.nlogo);;All Files (*)"));
    modelFileTextBox->setText(inputFileName);
}

void MainWindow::OpenDir() {
    
    outputDirectory = QFileDialog::getExistingDirectory(this,
        tr("Open output directory"), "",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    outputDirectoryTextBox->setText(outputDirectory);
}

void MainWindow::Translate() {
    QString program = QDir::currentPath() + "/generator";
    std::cout << program.toStdString() << std::endl;
    QStringList arguments;
    arguments << "cpp" << inputFileName << outputDirectory;
    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);
}

void MainWindow::TranslatePandora() {
    QString program = QDir::currentPath() + "/generator";
    std::cout << program.toStdString() << std::endl;
    QStringList arguments;
    arguments << "pandora" << inputFileName << outputDirectory;
    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);
}