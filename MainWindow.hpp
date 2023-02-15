#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QVector>
#include <numeric>
#include <random>
#include <limits>
#include <cmath>
#include <algorithm>

#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);

  void calculate_data(const int num_of_numbers);
  void fill_vec_of_numbers(const int num_of_numbers);

  ~MainWindow();

private:
  Ui::MainWindow *ui;
  QVector<double> vec_of_numbers;
  QVector<double> vec_of_k;

  void draw_histogram(const int num_of_numbers);
  void draw_pdf(const QVector<double>& vec_of_f);
  void draw_idf(const QVector<double>& vec_of_big_f);
};
#endif // MAINWINDOW_HPP
