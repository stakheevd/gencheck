#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  for (int i = 10; i <= 10000; i*=10)
  {
    fill_vec_of_numbers(i);
    calculate_data(i);
    draw_histogram(i);
  }
}

void MainWindow::calculate_data(const int num_of_numbers)
{
  vec_of_k.clear();
  vec_of_k.resize(num_of_numbers - 1);

  // Calculate M
  double sum = std::accumulate(vec_of_numbers.begin(), vec_of_numbers.end(), 0.0);
  const double EXPECTED_VAL = sum / num_of_numbers;

  // Calculate dispersion and correlation coefficients
  double numerator_of_disp = 0;
  double denominator = 0;

  for (int k = 0; k < num_of_numbers; k++)
    denominator += std::pow((vec_of_numbers[k] - EXPECTED_VAL), 2);

  for (int j = 1; j <= num_of_numbers; j++)
  {
    numerator_of_disp += std::pow((vec_of_numbers[j - 1] - EXPECTED_VAL), 2) ;
    double numerator = 0;

    for (int k = 0; k < num_of_numbers - j; k++)
      numerator += (vec_of_numbers[k] - EXPECTED_VAL) * (vec_of_numbers[k + j] - EXPECTED_VAL);

    if (j < num_of_numbers)
      vec_of_k[j - 1] = numerator / denominator;
  }

  // Calculate Probability density function and integral distribution function
  if (num_of_numbers == 10000)
  {
    const int num_of_intervals = 100;
    const double step = 0.01;
    QVector<double> vec_of_f(num_of_intervals);
    QVector<double> vec_of_bf(num_of_intervals);

    for (int i = 1; i <= num_of_intervals; i++)
    {
      double previous_step = step * (i - 1);
      double current_step = step * i;

      int num_of_occur_f = std::count_if(vec_of_numbers.begin(), vec_of_numbers.end(), [&](double j) {
        return (j < current_step && j >= previous_step);
      });

      vec_of_f[i - 1] = num_of_occur_f / (vec_of_numbers.size() * step) ;

      double num_of_occur_bf = std::count_if(vec_of_numbers.begin(), vec_of_numbers.end(), [&](double j) {
        return (j < current_step);
      });

      vec_of_bf[i - 1] = num_of_occur_bf / vec_of_numbers.size();
    }

    draw_pdf(vec_of_f);
    draw_idf(vec_of_bf);
  }

  qDebug() << "M = " << EXPECTED_VAL;
  qDebug() << "D = " << (numerator_of_disp / num_of_numbers);
  qDebug() << "S = " << std::sqrt(numerator_of_disp / num_of_numbers);
  qDebug() << "----------------------------------";
}

void MainWindow::fill_vec_of_numbers(const int num_of_numbers)
{
  vec_of_numbers.clear();
  vec_of_numbers.resize(num_of_numbers);

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<> dist(0.0, std::nextafter(1.0, std::numeric_limits<double>::max()));

  std::generate(vec_of_numbers.begin(), vec_of_numbers.end(), [&]() mutable {return dist(rng);});
}

void MainWindow::draw_histogram(const int num_of_numbers)
{
  QVector<double> keyData(num_of_numbers - 1);
  std::iota(keyData.begin(), keyData.end(), 1);

  QCPBars *myBars;
  QCustomPlot* plot_ptr;

  switch(num_of_numbers)
  {
    case 10:
      plot_ptr = ui->plot_10;
      break;
    case 100:
      plot_ptr = ui->plot_100;
      break;
    case 1000:
      plot_ptr = ui->plot_1000;
      break;
    case 10000:
      plot_ptr = ui->plot_10000;
      break;
  }

  myBars = new QCPBars(plot_ptr->xAxis, plot_ptr->yAxis);
  myBars->setData(keyData, vec_of_k);

  plot_ptr->rescaleAxes();
  plot_ptr->replot();
}

void MainWindow::draw_pdf(const QVector<double>& vec_of_f)
{
  QVector<double> num_of_intervals(100);
  std::iota(num_of_intervals.begin(), num_of_intervals.end(), 1);

  QCPBars *pdf_ptr = new QCPBars(ui->plot_pdf->xAxis, ui->plot_pdf->yAxis);
  pdf_ptr->setData(num_of_intervals, vec_of_f);

  ui->plot_pdf->rescaleAxes();
  ui->plot_pdf->replot();
}

void MainWindow::draw_idf(const QVector<double>& vec_of_big_f)
{
  QVector<double> num_of_intervals(100);
  std::iota(num_of_intervals.begin(), num_of_intervals.end(), 1);

  QCPBars *idf_ptr = new QCPBars(ui->plot_idf->xAxis, ui->plot_idf->yAxis);
  idf_ptr->setData(num_of_intervals, vec_of_big_f);

  ui->plot_idf->rescaleAxes();
  ui->plot_idf->replot();
}

MainWindow::~MainWindow()
{
  delete ui;
}

