#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Подключение сигналов от кнопок к соответствующим слотам
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_startButton_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_pauseButton_clicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::on_nextButton_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::on_prevButton_clicked);


    // Инициализация плеера
    player = new QMediaPlayer(this);

    // Связывание сигналов плеера со слотами слайдера
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::on_durationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::on_positionChanged);
    connect(player, &QMediaPlayer::mediaChanged, this, &MainWindow::on_mediaChanged);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    // Открываем диалоговое окно выбора папки с песнями
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Open Folder"), QDir::homePath());

    // Получаем список файлов в выбранной папке
    QDir directory(folderPath);
    QStringList files = directory.entryList(QStringList() << "*.mp3" << "*.wav", QDir::Files);

    // Создаем плейлист на основе файлов
    playlist.clear();
    for (const QString &file : files) {
        playlist.append(folderPath + "/" + file);
    }

    // Если плейлист не пустой, начинаем воспроизведение первой песни
    if (!playlist.isEmpty()) {
        currentTrackIndex = 0; // Устанавливаем индекс текущего трека в начало плейлиста
        player->setMedia(QUrl::fromLocalFile(playlist.first()));
        player->play();
    }
}

void MainWindow::on_nextButton_clicked()
{
    // Проверяем, есть ли еще треки в плейлисте после текущего
    if (currentTrackIndex < playlist.size() - 1) {
        currentTrackIndex++;
        player->setMedia(QUrl::fromLocalFile(playlist[currentTrackIndex]));
        player->play();
    }
}

void MainWindow::on_pauseButton_clicked()
{
    // Если медиа воспроизводится, останавливаем воспроизведение
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
        ui->pushButton_2->setText("■"); // Изменяем текст кнопки на "■"
    } else { // Если медиа приостановлено, возобновляем воспроизведение
        player->play();
        ui->pushButton_2->setText("∎∎"); // Изменяем текст кнопки на "∎∎"
    }
}

void MainWindow::on_durationChanged(qint64 duration)
{
    ui->horizontalSlider->setMaximum(duration); // Установка максимального значения слайдера равным длительности песни
}

void MainWindow::on_positionChanged(qint64 position)
{
    ui->horizontalSlider->setValue(position); // Установка текущего значения слайдера равным текущей позиции воспроизведения
}
void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    player->setPosition(position); // Установка позиции воспроизведения по значению слайдера
}
void MainWindow::on_mediaChanged(const QMediaContent &media)
{
    QUrl url = media.canonicalUrl();
    QString fileName = QFileInfo(url.path()).fileName(); // Извлечение имени файла из URL
    ui->label->setText(fileName); // Установка имени файла в QLabel
}
void MainWindow::on_prevButton_clicked()
{
    // Проверяем, есть ли треки перед текущим в плейлисте
    if (currentTrackIndex > 0) {
        currentTrackIndex--;
        player->setMedia(QUrl::fromLocalFile(playlist[currentTrackIndex]));
        player->play();
    }
}

