#include "avatarcropdialog.h"
#include "ui_avatarcropdialog.h"

AvatarCropDialog::AvatarCropDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AvatarCropDialog)
{
    ui->setupUi(this);

    // 连接信号槽 - 假设按钮对象名已在UI中设置
    connect(ui->confirmButton, &QPushButton::clicked,
            this, &AvatarCropDialog::onConfirmClicked);
    connect(ui->cancelButton, &QPushButton::clicked,
            this, &AvatarCropDialog::onCancelClicked);
}

AvatarCropDialog::~AvatarCropDialog()
{
    delete ui;
}

void AvatarCropDialog::setImage(const QPixmap &image)
{
    m_originalImage = image;

    if (ui->label) {
        QPixmap scaledImage = m_originalImage.scaled(
            ui->label->width(),
            ui->label->height(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        );
        ui->label->setPixmap(scaledImage);
    }
}

QPixmap AvatarCropDialog::getCroppedAvatar() const
{
    return m_croppedImage;
}

void AvatarCropDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !m_originalImage.isNull()) {
        m_origin = event->pos();
        if (ui->label && ui->label->geometry().contains(m_origin)) {
            if (!m_rubberBand) {
                m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
            }
            m_rubberBand->setGeometry(QRect(m_origin, QSize()));
            m_rubberBand->show();
            m_isSelecting = true;
        }
    }
}

void AvatarCropDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isSelecting && m_rubberBand) {
        m_rubberBand->setGeometry(QRect(m_origin, event->pos()).normalized());
    }
}

void AvatarCropDialog::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isSelecting && m_rubberBand) {
        m_selectionRect = m_rubberBand->geometry();
        m_isSelecting = false;
    }
}

void AvatarCropDialog::onConfirmClicked()
{
    m_croppedImage = calculateCroppedImage();
    if (!m_croppedImage.isNull()) {
        accept();
    } else {
        reject();
    }
}

void AvatarCropDialog::onCancelClicked()
{
    reject();
}

QPixmap AvatarCropDialog::calculateCroppedImage() const
{
    if (m_selectionRect.isEmpty() || m_originalImage.isNull()) {
        return QPixmap();
    }

    if (!ui->label) {
        return QPixmap();
    }

    // 获取显示的图片
    const QPixmap *displayedPixmap = ui->label->pixmap();
    if (!displayedPixmap || displayedPixmap->isNull()) {
        return QPixmap();
    }

    // 计算标签在对话框中的位置
    QRect labelRect = ui->label->geometry();

    // 计算实际显示的图片区域（可能不是整个label，因为有保持宽高比）
    QSize scaledSize = displayedPixmap->size();
    QPoint imageOffset((labelRect.width() - scaledSize.width()) / 2,
                      (labelRect.height() - scaledSize.height()) / 2);

    // 调整选择区域到图片坐标系
    QRect imageSelectionRect(
        m_selectionRect.x() - labelRect.x() - imageOffset.x(),
        m_selectionRect.y() - labelRect.y() - imageOffset.y(),
        m_selectionRect.width(),
        m_selectionRect.height()
    );

    // 确保选择区域在图片范围内
    imageSelectionRect = imageSelectionRect.intersected(QRect(0, 0, scaledSize.width(), scaledSize.height()));

    if (imageSelectionRect.isEmpty()) {
        return QPixmap();
    }

    // 计算缩放比例
    double scaleX = m_originalImage.width() / (double)scaledSize.width();
    double scaleY = m_originalImage.height() / (double)scaledSize.height();

    // 映射回原始图片坐标
    QRect sourceRect(
        imageSelectionRect.x() * scaleX,
        imageSelectionRect.y() * scaleY,
        imageSelectionRect.width() * scaleX,
        imageSelectionRect.height() * scaleY
    );

    sourceRect = sourceRect.intersected(m_originalImage.rect());

    if (sourceRect.isEmpty()) {
        return QPixmap();
    }

    // 裁剪并缩放图片
    QPixmap croppedImage = m_originalImage.copy(sourceRect);
    return croppedImage.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}
