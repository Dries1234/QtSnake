#include "mainwindow.h"
#include "gamecontroller.h"
#include "qgraphicsitem.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QGridLayout>
#include <QSound>
#include <QStandardPaths>
GameController *game;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  game = new GameController();
  ui->setupUi(this);
  QPushButton *button = ui->colorPicker;
  QString resource = ":/media/color-picker.png";
  QPixmap pixIcon(resource);
  button->setIcon(QIcon(pixIcon));
  setButtonColor(button, game->color);
}

MainWindow::~MainWindow() {
  delete ui;
  delete game;
}

void MainWindow::on_colorPicker_clicked() {
  QColor color = QColorDialog::getColor();
  game->color = color;
  setButtonColor(ui->colorPicker, color);
}

void MainWindow::setButtonColor(QPushButton *button, QColor color) {
  QPalette palette = button->palette();
  palette.setColor(QPalette::Button, color);
  button->setAutoFillBackground(true);
  button->setPalette(palette);
  button->update();
}

void MainWindow::on_startGame_clicked() {
  QColor tempColor = game->color;
  game = new GameController();
  game->color = tempColor;
  game->filePath =
      QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))
          .filePath("highscore.txt");
  game->show();
  game->raise();
  game->setFocus();
  game->start();

  connect(game, &GameController::scoreSaved, this, &MainWindow::gameLost);
}

void MainWindow::gameLost() {
  QPointF middle;
  QGraphicsTextItem *diedText;
  QGraphicsTextItem *scoreText;
  QGraphicsTextItem *highScoreText;
  QGraphicsTextItem *currentHighScoreText;
  QPointF tempPos;

  game->scene = new QGraphicsScene(0, 0, 800, 600);
  game->setScene(game->scene);

  middle = game->scene->sceneRect().center();

  // you died text
  diedText = game->scene->addText("You died!");
  diedText->setTransformOriginPoint(diedText->boundingRect().center());
  diedText->setScale(4);
  diedText->setDefaultTextColor(Qt::black);
  tempPos = middle;
  tempPos.setX(middle.x() - (diedText->boundingRect().width() / 2));
  tempPos.setY(diedText->boundingRect().height() + 5);
  diedText->setPos(tempPos);

  // Score text
  scoreText =
      game->scene->addText("Your score: " + QString::number(game->score));
  scoreText->setTransformOriginPoint(scoreText->boundingRect().center());
  scoreText->setScale(4);
  scoreText->setDefaultTextColor(Qt::black);
  tempPos = middle;
  tempPos.setX(middle.x() - (scoreText->boundingRect().width()) / 2);
  tempPos.setY(scoreText->boundingRect().height() + 70);
  scoreText->setPos(tempPos);

  // new Highscore text
  if (game->newhigh) {
    QString audiofiles[2] = {":/media/hs1.wav", ":/media/hs2.wav"};
    QSound::play(audiofiles[rand() % 2]);
    highScoreText = game->scene->addText("HIGH SCORE!");
    highScoreText->setTransformOriginPoint(
        highScoreText->boundingRect().center());
    highScoreText->setScale(5);
    highScoreText->setDefaultTextColor(Qt::black);
    tempPos = middle;
    tempPos.setX(middle.x() - (highScoreText->boundingRect().width()) / 2);
    highScoreText->setPos(tempPos);
  }

  //current highscore text
  currentHighScoreText = game->scene->addText("Highscore: " + QString::number(game->highScore));
  currentHighScoreText->setTransformOriginPoint(currentHighScoreText->boundingRect().center());
  currentHighScoreText->setScale(2);
  currentHighScoreText->setDefaultTextColor(Qt::black);
  tempPos = middle;
  tempPos.setX(middle.x() - (currentHighScoreText->boundingRect().width())/2);
  tempPos.setY(tempPos.y() + 70);
  currentHighScoreText->setPos(tempPos);

}
