#include "MainWindow.hpp"

int main(int argc, char *argv[]) {
    
  QApplication app(argc, argv);  
    
  MainWindow window;

  window.resize(850, 250);
  window.setWindowTitle("Translator");
  window.show();

  return app.exec();
}