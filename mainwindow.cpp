#include "mainwindow.h"
#include "imageprocessingservice.h"
#include "qbuffer.h"
#include "qevent.h"

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent),
    selectButton(new QToolButton(this)),
    deleteButton(new QToolButton(this)),
    saveButton(new QToolButton(this)),
    imageLabel(new QLabel(this)),
    layout(new QVBoxLayout()),
    buttonLayout(new QHBoxLayout()),
    leftSideLayout(new QVBoxLayout()),
    rightSideLayout(new QVBoxLayout()),
    defaultRadioButton(new QRadioButton("Default",this)),
    edgeDetectionRadioButton(new QRadioButton("Edge Detection", this)),
    th1Slider(new QSlider(Qt::Horizontal, this)),
    grayscaleRadioButton(new QRadioButton("Gray Scale", this)),
    brightnessCheckBox(new QCheckBox("Brightness", this)),
    contrastCheckBox(new QCheckBox("Contrast", this)),
    blurCheckBox(new QCheckBox("Blur", this)),
    thresholdRadioButton(new QRadioButton("Threshold", this)),
    histogramRadioButton(new QRadioButton("Histogram", this)),
    brightnessSlider(new QSlider(Qt::Horizontal, this)),
    contrastSlider(new QSlider(Qt::Horizontal, this)),
    blurSlider(new QSlider(Qt::Horizontal, this)),
    thresholdSlider(new QSlider(Qt::Horizontal, this)),
    loadedImage(),
    loadedImagePath(),
    edgesImage(),
    grayscaleImage()

{

    // Build MySQL database connection
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("images");
    db.setUserName("root");
    db.setPassword("password");
    if (!db.open()) {
        QMessageBox::critical(this, tr("Database Error"), db.lastError().text());
    }

    resize(800, 600);
    this->setStyleSheet("background-color: #333; color: white;");
    imageLabel->setStyleSheet("background-color: #333;");
    selectButton->setStyleSheet("background-color: #333; color: white;");
    deleteButton->setStyleSheet("background-color: #333; color: white;");
    saveButton->setStyleSheet("background-color: #333; color: white;");

    selectButton->setText("Select Image");
    deleteButton->setText("Delete Image");
    saveButton->setText("Save Image");

    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setStyleSheet("border: 2px solid white; background-color: #333;");

    disableImageControls();
    resetControls();

    brightnessSlider->setRange(0, 200);
    contrastSlider->setRange(0,100);
    blurSlider->setRange(1, 50);
    thresholdSlider->setRange(0, 255);
    th1Slider->setRange(0,1000);
    brightnessSlider->setValue(100);
    contrastSlider->setValue(100);
    blurSlider->setValue(1);
    thresholdSlider->setValue(128);
    th1Slider->setValue(500);
    brightnessSlider->setFixedWidth(100);
    contrastSlider->setFixedWidth(100);
    blurSlider->setFixedWidth(100);
    thresholdSlider->setFixedWidth(100);
    th1Slider->setFixedWidth(100);

    //Connect signals and slots
    connect(selectButton, &QToolButton::clicked, this, &MainWindow::onLoadImage);
    connect(deleteButton, &QToolButton::clicked, this, &MainWindow::deleteImage);
    connect(saveButton, &QToolButton::clicked, this, &MainWindow::saveImage);
    connect(defaultRadioButton, &QRadioButton::clicked, this, &MainWindow::setDefaultImage);
    connect(edgeDetectionRadioButton, &QRadioButton::toggled, this, &MainWindow::onRadioButtonToggled);
    connect(grayscaleRadioButton, &QRadioButton::toggled, this, &MainWindow::onRadioButtonToggled);
    connect(brightnessCheckBox, &QCheckBox::toggled, this, &MainWindow::onRadioButtonToggled);
    connect(contrastCheckBox, &QCheckBox::toggled, this, &MainWindow::onRadioButtonToggled);
    connect(blurCheckBox, &QCheckBox::toggled, this, &MainWindow::onRadioButtonToggled);
    connect(thresholdRadioButton, &QRadioButton::toggled, this, &MainWindow::onRadioButtonToggled);
    connect(histogramRadioButton, &QRadioButton::toggled, this, &MainWindow::onRadioButtonToggled);
    connect(brightnessSlider, &QSlider::valueChanged, this, &MainWindow::onSliderValueChanged);
    connect(contrastSlider, &QSlider::valueChanged, this, &MainWindow::onSliderValueChanged);
    connect(blurSlider, &QSlider::valueChanged, this, &MainWindow::onSliderValueChanged);
    connect(thresholdSlider, &QSlider::valueChanged, this, &MainWindow::onSliderValueChanged);
    connect(th1Slider, &QSlider::valueChanged, this, &MainWindow::onSliderValueChanged);

    // Layouts
    buttonLayout->addWidget(selectButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);

    leftSideLayout->addWidget(defaultRadioButton);
    leftSideLayout->addWidget(edgeDetectionRadioButton);
    leftSideLayout->addWidget(th1Slider);
    leftSideLayout->addWidget(grayscaleRadioButton);
    leftSideLayout->addWidget(histogramRadioButton);
    leftSideLayout->addWidget(thresholdRadioButton);
    leftSideLayout->addWidget(thresholdSlider);

    rightSideLayout->addWidget(brightnessCheckBox);
    rightSideLayout->addWidget(brightnessSlider);
    rightSideLayout->addWidget(contrastCheckBox);
    rightSideLayout->addWidget(contrastSlider);
    rightSideLayout->addWidget(blurCheckBox);
    rightSideLayout->addWidget(blurSlider);

    QHBoxLayout *spacerLayout = new QHBoxLayout();
    spacerLayout->addLayout(leftSideLayout);
    spacerLayout->addLayout(rightSideLayout);

    layout->addLayout(buttonLayout);
    layout->addWidget(imageLabel);
    layout->addLayout(spacerLayout);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    //Clean
    delete selectButton;
    delete deleteButton;
    delete saveButton;
    delete imageLabel;
    delete layout;
    delete buttonLayout;
    delete leftSideLayout;
    delete rightSideLayout;
    delete edgeDetectionRadioButton;
    delete grayscaleRadioButton;
    delete brightnessCheckBox;
    delete contrastCheckBox;
    delete blurCheckBox;
    delete thresholdRadioButton;
    delete histogramRadioButton;
    delete brightnessSlider;
    delete contrastSlider;
    delete blurSlider;
    delete thresholdSlider;
    delete th1Slider;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateImage();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        if (!urlList.isEmpty()) {
            QString fileName = urlList.first().toLocalFile();
            defaultRadioButton->setChecked(true);
            loadImage(fileName);
            enableImageControls(true);
            brightnessSlider->setValue(100);
            contrastSlider->setValue(100);
            blurSlider->setValue(1);
            thresholdSlider->setValue(128);
            th1Slider->setValue(500);
            brightnessCheckBox->setChecked(false);
            contrastCheckBox->setChecked(false);
            blurCheckBox->setChecked(false);
        }
    }
}

void MainWindow::onLoadImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.jpg *.jpeg)"));
    if (!fileName.isEmpty()) {
        defaultRadioButton->setChecked(true);
        loadImage(fileName);
        enableImageControls(true);
        resetControls();
    }
}

void MainWindow::deleteImage()
{
    imageLabel->clear();
    loadedImage = QImage();
    disableImageControls();
    loadedImagePath.clear();
    brightnessSlider->setValue(100);
    contrastSlider->setValue(100);
    blurSlider->setValue(1);
    th1Slider->setValue(500);
    resetControls();
    defaultRadioButton->setChecked(true);
}

void MainWindow::resetControls()
{
    //Reset the RadioButtons
    edgeDetectionRadioButton->setChecked(false);
    grayscaleRadioButton->setChecked(false);
    thresholdRadioButton->setChecked(false);
    brightnessCheckBox->setChecked(false);
    contrastCheckBox->setChecked(false);
    blurCheckBox->setChecked(false);

    //Reset the Sliders
    brightnessSlider->setEnabled(false);
    contrastSlider->setEnabled(false);
    blurSlider->setEnabled(false);
    thresholdSlider->setEnabled(false);
    th1Slider->setEnabled(false);
    brightnessSlider->setValue(100);
    contrastSlider->setValue(100);
    blurSlider->setValue(1);
    thresholdSlider->setValue(128);
    th1Slider->setValue(500);
}

void MainWindow::saveImage()
{
    if (loadedImage.isNull()) {
        QMessageBox::warning(this, tr("Save Image"), tr("There is no image to save."));
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("Image Files (*.jpg *.jpeg)"));
    if (fileName.isEmpty()) {
        return;
    }

    QImage saveImage = loadedImage;
    QStringList appliedOperations;

    try {
        // Check and apply basic operations
        if (edgeDetectionRadioButton->isChecked()) {
            saveImage = performEdgeDetection(saveImage, th1Slider->value());
            appliedOperations << "Edge Detection";
        } else if (grayscaleRadioButton->isChecked()) {
            saveImage = convertToGrayscale(saveImage);
            appliedOperations << "Gray Scale";
        } else if (thresholdRadioButton->isChecked()) {
            saveImage = applyThreshold(saveImage, thresholdSlider->value());
            appliedOperations << "Threshold";
        } else if (histogramRadioButton->isChecked()) {
            saveImage = drawHistogram(saveImage);
            appliedOperations << "Histogram";
        }

        // Apply other procedures
        if (brightnessCheckBox->isChecked()) {
            saveImage = adjustBrightness(saveImage, brightnessSlider->value());
            appliedOperations << "Brightness";
        }

        if (contrastCheckBox->isChecked()) {
            saveImage = adjustContrast(saveImage, contrastSlider->value());
            appliedOperations << "Contrast";
        }

        if (blurCheckBox->isChecked()) {
            saveImage = applyBlur(saveImage, blurSlider->value());
            appliedOperations << "Blur";
        }

        if (!saveImage.isNull() && !saveImage.save(fileName)) {
            QMessageBox::warning(this, tr("Save Image"), tr("Failed to save image."));
        } else {
            if (!saveImageToDatabase(fileName, saveImage, appliedOperations)) {
                QMessageBox::warning(this, tr("Save Image"), tr("Failed to save image to the database."));
            }
        }
    } catch (const cv::Exception &ex) {
        QMessageBox::critical(this, tr("Error"), QString("An unexpected error occurred."));
    }
}

void MainWindow::onRadioButtonToggled(bool checked)
{
    Q_UNUSED(checked);

    //Enable/disable sliders based on selected radio button or checkbox
    brightnessSlider->setEnabled(brightnessCheckBox->isChecked());
    contrastSlider->setEnabled(contrastCheckBox->isChecked());
    blurSlider->setEnabled(blurCheckBox->isChecked());
    thresholdSlider->setEnabled(thresholdRadioButton->isChecked());
    th1Slider->setEnabled(edgeDetectionRadioButton->isChecked());

    //Only reset threshold slider value if threshold radio button is checked and was not previously checked
    static bool wasThresholdChecked = false;
    if (thresholdRadioButton->isChecked() && !wasThresholdChecked) {
        thresholdSlider->setValue(128);
    }
    wasThresholdChecked = thresholdRadioButton->isChecked();

    //Update displayed image based on radio button selection
    updateImage();
}


void MainWindow::onCheckBoxToggled(bool checked)
{
    Q_UNUSED(checked);
    brightnessSlider->setEnabled(brightnessCheckBox->isChecked());
    contrastSlider->setEnabled(contrastCheckBox->isChecked());
    blurSlider->setEnabled(blurCheckBox->isChecked());

    // Update displayed image based on checkbox selection
    updateImage();
}

void MainWindow::onSliderValueChanged(int value)
{
    Q_UNUSED(value);

    // Update displayed image based on slider value changes
    updateImage();
}

void MainWindow::onValueChanged(int value)
{
    updateImage();
}

void MainWindow::loadImage(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::warning(this, tr("Load Image"), tr("Failed to load image."));
        return;
    }
    loadedImage = newImage;
    loadedImagePath = fileName;
    updateImage();
}

void MainWindow::updateImage()
{
    if (loadedImage.isNull()) {
        return;
    }

    try {
        QImage processedImage = loadedImage.copy();

        // Check and apply basic operations first
        if (edgeDetectionRadioButton->isChecked()) {
            processedImage = performEdgeDetection(processedImage, th1Slider->value());
        } else if (grayscaleRadioButton->isChecked()) {
            processedImage = convertToGrayscale(processedImage);
        } else if (thresholdRadioButton->isChecked()) {
            processedImage = applyThreshold(processedImage, thresholdSlider->value());
        } else if (histogramRadioButton->isChecked()) {
            // Check if the image is a color image before drawing the histogram
            if (processedImage.format() == QImage::Format_Grayscale8) {
                QMessageBox::warning(this, tr("Error"), tr("This image cannot be processed."));
                defaultRadioButton->setChecked(true);
                return;
            }
            processedImage = drawHistogram(processedImage);
        }

        // The other procedures
        if (brightnessCheckBox->isChecked()) {
            processedImage = adjustBrightness(processedImage, brightnessSlider->value());
        }

        if (contrastCheckBox->isChecked()) {
            processedImage = adjustContrast(processedImage, contrastSlider->value());
        }

        if (blurCheckBox->isChecked()) {
            processedImage = applyBlur(processedImage, blurSlider->value());
        }

        // Show rendered image in QLabel
        imageLabel->setPixmap(QPixmap::fromImage(processedImage).scaled(imageLabel->size(), Qt::KeepAspectRatio));
    } catch (const cv::Exception& ex) {
        QMessageBox::critical(this, tr("Error"), QString("This image cannot be processed."));
        defaultRadioButton->setChecked(true);
    }
}

void MainWindow::enableImageControls(bool enable)
{
    defaultRadioButton->setEnabled(enable);
    edgeDetectionRadioButton->setEnabled(enable);
    grayscaleRadioButton->setEnabled(enable);
    brightnessCheckBox->setEnabled(enable);
    contrastCheckBox->setEnabled(enable);
    blurCheckBox->setEnabled(enable);
    thresholdRadioButton->setEnabled(enable);
    saveButton->setEnabled(enable);
    histogramRadioButton->setEnabled(enable);
}

void MainWindow::disableImageControls()
{
    enableImageControls(false);
    brightnessSlider->setEnabled(false);
    contrastSlider->setEnabled(false);
    blurSlider->setEnabled(false);
    thresholdSlider->setEnabled(false);
    th1Slider->setEnabled(false);
}

void MainWindow::setDefaultImage()
{
    if (loadedImage.isNull()) {
        return;
    }

    QImage processedImage = loadedImage.copy();
    if (brightnessCheckBox->isChecked()) {
        processedImage = adjustBrightness(processedImage, brightnessSlider->value());
    }

    if (contrastCheckBox->isChecked()) {
        processedImage = adjustContrast(processedImage, contrastSlider->value());
    }
    if (blurCheckBox->isChecked()) {
        processedImage = applyBlur(processedImage, blurSlider->value());
    }

    imageLabel->setPixmap(QPixmap::fromImage(processedImage).scaled(imageLabel->size(), Qt::KeepAspectRatio));
}


QImage MainWindow::performEdgeDetection(const QImage &image, int th1)
{
    return ImageProcessingService::performEdgeDetection(image, th1);
}

QImage MainWindow::convertToGrayscale(const QImage &image)
{
    return ImageProcessingService::convertToGrayscale(image);
}

QImage MainWindow::applyThreshold(const QImage &image, int thresholdValue)
{
    return ImageProcessingService::applyThreshold(image, thresholdValue);
}

QImage MainWindow::adjustBrightness(const QImage &image, int brightnessValue)
{
    return ImageProcessingService::adjustBrightness(image, brightnessValue);
}

QImage MainWindow::adjustContrast(const QImage &image, int contrastValue)
{
    return ImageProcessingService::adjustContrast(image, contrastValue);
}

QImage MainWindow::applyBlur(const QImage &image, int blurValue)
{
    return ImageProcessingService::applyBlur(image, blurValue);
}

QImage MainWindow::drawHistogram(const QImage &image)
{
    // Check if the image is grayscale
    if (image.format() == QImage::Format_Grayscale8) {
        QMessageBox::warning(this, tr("Error"), tr("This image cannot be processed."));
        return QImage();
    }

    return ImageProcessingService::drawHistogram(image);
}


bool MainWindow::saveImageToDatabase(const QString &fileName, const QImage &image, const QStringList &operations)
{
    if (!db.isOpen() && !db.open()) {
        QMessageBox::critical(this, tr("Database Error"), db.lastError().text());
        return false;
    }

    // Create the images table if it doesn't exist
    QSqlQuery query;
    bool tableCreated = query.exec("CREATE TABLE IF NOT EXISTS images ("
                                   "id INT AUTO_INCREMENT PRIMARY KEY, "
                                   "name VARCHAR(255), "
                                   "operations TEXT, "
                                   "image LONGBLOB, "
                                   "date DATETIME)");

    if (!tableCreated) {
        QMessageBox::critical(this, tr("Database Error"), query.lastError().text());
        return false;
    }

    // Prepare the SQL query to insert the image data
    query.prepare("INSERT INTO images (name, operations, image, date) VALUES (:name, :operations, :image, :date)");
    query.bindValue(":name", QFileInfo(fileName).fileName());
    query.bindValue(":operations", operations.join(", "));
    query.bindValue(":image", imageToByteArray(image));
    query.bindValue(":date", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

    // Execute the query and check for errors
    if (!query.exec()) {
        QMessageBox::critical(this, tr("Database Error"), query.lastError().text());
        return false;
    }

    return true;
}


// Convert the Qimage to cv::Mat
cv::Mat MainWindow::QImageToMat(const QImage &image)
{
    return ImageProcessingService::QImageToMat(image);
}

// Convert the cv::Mat to Qimage
QImage MainWindow::MatToQImage(const cv::Mat &mat)
{
    return ImageProcessingService::MatToQImage(mat);
}

QByteArray MainWindow::imageToByteArray(const QImage &image)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    return byteArray;
}
