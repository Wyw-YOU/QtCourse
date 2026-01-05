#ifndef MYBUBBLE_H
#define MYBUBBLE_H

#include <QWidget>

namespace Ui {
class myBubble;
}

class myBubble : public QWidget
{
    Q_OBJECT

public:
    explicit myBubble(QWidget *parent = nullptr);
    ~myBubble();

    void setHeadImage(QString imgpath);
    QSize calculateTextSize() const;
    QSize calculateBubbleSize(QSize &text_size) const; // 新增：计算气泡大小
    void setContent(const QString &text);

private:
    Ui::myBubble *ui;
};

#endif // MYBUBBLE_H
