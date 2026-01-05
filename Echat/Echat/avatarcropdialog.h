#ifndef AVATARCROPDIALOG_H
#define AVATARCROPDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QMouseEvent>
#include <QRubberBand>

namespace Ui {
class AvatarCropDialog;
}

class AvatarCropDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AvatarCropDialog(QWidget *parent = nullptr);
    ~AvatarCropDialog();

    void setImage(const QPixmap &image);
    QPixmap getCroppedAvatar() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void onConfirmClicked();
    void onCancelClicked();

private:
    Ui::AvatarCropDialog *ui;
    QPixmap calculateCroppedImage() const;

    QPixmap m_originalImage;
    QPixmap m_croppedImage;
    QRubberBand *m_rubberBand;
    QPoint m_origin;
    QRect m_selectionRect;
    bool m_isSelecting;
};

#endif // AVATARCROPDIALOG_H
