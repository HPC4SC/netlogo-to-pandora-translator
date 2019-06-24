#include "MainWindow.hpp"
#include <QGridLayout>
#include <QFileDialog>
#include <QGroupBox>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent) {

    QFont font("Roboto");
    font.setStyleHint(QFont::Monospace);
    QApplication::setFont(font);

    this->setContentsMargins(20, 20, 20, 20);

    this->setStyleSheet("QPushButton {"
                        "background-color: white;"
                        "border: 1px solid black;"
                        "padding: 10px;"
                        "font-size: 18px"
                    "}"
                    "QPushButton:hover {"
                        "background-color: white;"
                        "border: 2px solid black;"
                        "padding: 10px;"
                    "}"
                    "QLineEdit {"
                        "background-color: white;"
                        "border: 1px solid black;"
                        "padding: 10px;"
                        "font-size: 18px"
                    "}");
 
    QGroupBox *group = new QGroupBox(tr("Echo"));
        
    QPushButton *inputBtn = new QPushButton("Select NetLogo model file", this);
    modelFileTextBox = new QLineEdit;
    modelFileTextBox->setPlaceholderText("No model file selected");

    QPushButton *outputBtn = new QPushButton("Select output directory", this);
    outputDirectoryTextBox = new QLineEdit;
    outputDirectoryTextBox->setPlaceholderText("No output directory selected");

    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(inputBtn, 0, 0);
    grid->addWidget(modelFileTextBox, 0, 1, 1, 2);
    grid->addWidget(outputBtn, 1, 0);
    grid->addWidget(outputDirectoryTextBox, 1, 1, 1, 2);
    group->setLayout(grid);

    setLayout(grid);  

    connect(inputBtn, &QPushButton::clicked, this, &MainWindow::OpenFile);
    connect(outputBtn, &QPushButton::clicked, this, &MainWindow::OpenFile);
}

void MainWindow::OpenFile() {
    
    inputFileName = QFileDialog::getOpenFileName(this,
        tr("Open Address Book"), "",
        tr("Address Book (*.abk);;All Files (*)"));
    modelFileTextBox->setText(inputFileName);
}

void MainWindow::OpenDir() {
    
    outputDirectory = QFileDialog::getOpenFileName(this,
        tr("Open Address Book"), "",
        tr("Address Book (*.abk);;All Files (*)"));
    outputDirectoryTextBox->setText(outputDirectory);
}