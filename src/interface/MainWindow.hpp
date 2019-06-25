#pragma once

#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

class MainWindow : public QWidget {
    
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);

    private slots:
        void OpenFile();
        void OpenDir();
        void Translate();

    private:
        QString inputFileName;
        QString outputDirectory;
        QLineEdit *modelFileTextBox;
        QLineEdit *outputDirectoryTextBox;

};