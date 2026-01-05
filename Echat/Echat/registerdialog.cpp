#include "registerdialog.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    // 初始化头像
    m_currentAvatar = QPixmap(); // 空头像

    // 更新头像显示
    updateAvatarDisplay();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_confirmButton_clicked()
{
    // 获取表单数据
    m_name = ui->name_edit->text();
    m_account = ui->account_edit->text();
    m_password = ui->password_edit->text();

    if(m_choice == 0){
        // 这里可以添加表单验证
        if(m_name.isEmpty() || m_account.isEmpty() || m_password.isEmpty()){
            CustomMessageBox::showInformation(this, "提醒", "账号注册信息填写不完整，请重新填写");
            return;
        }

        if(m_currentAvatar.isNull()){
            CustomMessageBox::showInformation(this, "提醒", "头像未选择，请选择");
            return;
        }

        // 账号长度验证
        if(m_account.length() < 4 || m_account.length() > 20){
            CustomMessageBox::showInformation(this, "注册提醒", "账号长度应在4-20个字符之间");
            return;
        }

        // 密码长度验证
        if(m_password.length() < 6){
            CustomMessageBox::showInformation(this, "注册提醒", "密码长度至少6个字符");
            return;
        }

    }else{
        // 这里可以添加表单验证
        if(m_name.isEmpty() || m_account.isEmpty()){
            CustomMessageBox::showInformation(this, "提醒", "账号注册信息填写不完整，请重新填写");
            return;
        }

        if(m_currentAvatar.isNull()){
            CustomMessageBox::showInformation(this, "提醒", "头像未选择，请选择");
            return;
        }

        // 账号长度验证
        if(m_account.length() < 4 || m_account.length() > 20){
            CustomMessageBox::showInformation(this, "注册提醒", "账号长度应在4-20个字符之间");
            return;
        }
    }

    // 获取优化后的头像数据
    m_avatarData = getOptimizedAvatarData();

    // 验证头像大小
    if (!validateAvatarSize(m_avatarData)) {
        return; // 用户选择取消或重新选择
    }

    // 使用 m_currentAvatar 作为用户头像

    // 执行注册逻辑...

    accept(); // 关闭对话框
}

void RegisterDialog::on_headimg_btn_clicked()
{
    // 1. 打开文件选择对话框
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "选择头像图片",
        "",
        "图片文件 (*.png *.jpg *.jpeg *.bmp *.gif)"
    );

    if (fileName.isEmpty()) {
        return; // 用户取消了选择
    }

    // 检查文件大小（提前检查避免加载大文件）
    QFileInfo fileInfo(fileName);
    qint64 fileSize = fileInfo.size();
    if (fileSize > 10 * 1024 * 1024) { // 10MB
        CustomMessageBox::showWarning(this, "文件过大",
            QString("选择的图片文件过大（%1），请选择小于10MB的图片文件。")
            .arg(formatFileSize(fileSize)));
        return;
    }

    // 2. 加载图片
    QPixmap originalImage(fileName);
    if (originalImage.isNull()) {
        QMessageBox::warning(this, "错误", "无法加载图片文件！");
        return;
    }

    // 优化头像
    m_currentAvatar = optimizeAvatar(originalImage);

//    // 3. 直接使用选中的图片，不进行裁剪
//    // 缩放图片到合适的大小（例如120x120）
//    m_currentAvatar = originalImage.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 4. 更新显示
    updateAvatarDisplay();

    // 预览时检查压缩后大小（可选，用于提前提示用户）
    QByteArray testData = getOptimizedAvatarData();
    if (!testData.isEmpty() && testData.size() > MAX_AVATAR_SIZE) {
        showSizeWarning(testData);
    }
}

void RegisterDialog::updateAvatarDisplay()
{
    if (m_currentAvatar.isNull()) {
        // 如果没有头像，显示默认图标或文字
        ui->headimg_btn->setIcon(QIcon()); // 清除图标
        ui->headimg_btn->setText("点击选择头像");
    } else {
        // 有头像，显示头像
        QIcon avatarIcon(m_currentAvatar);
        ui->headimg_btn->setIcon(avatarIcon);
        ui->headimg_btn->setIconSize(ui->headimg_btn->size());
        ui->headimg_btn->setText(""); // 清除文字
    }
}

QPixmap RegisterDialog::optimizeAvatar(const QPixmap &original)
{
    QPixmap processed = original;

    // 第一步：智能缩放
    // 如果图片尺寸太大，先缩放到合理大小
    int maxDimension = 120; // 最大尺寸
    if (processed.width() > maxDimension || processed.height() > maxDimension) {
        processed = processed.scaled(maxDimension, maxDimension,
                                   Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    // 第二步：格式优化
    // 转换为RGB格式减少数据量（移除Alpha通道）
    QImage image = processed.toImage();
    if (image.format() != QImage::Format_RGB888) {
        image = image.convertToFormat(QImage::Format_RGB888);
    }

    return QPixmap::fromImage(image);
}

QByteArray RegisterDialog::getOptimizedAvatarData()
{
    if (m_currentAvatar.isNull()) {
        return QByteArray();
    }

    QByteArray byteArray;
    QBuffer buffer(&byteArray);

    // 定义多种压缩策略
    struct CompressionStrategy {
        QString format;
        int quality;
        QString description;
    };

    QVector<CompressionStrategy> strategies = {
        {"JPEG", 85, "高质量"},
        {"JPEG", 70, "中等质量"},
        {"JPEG", 50, "标准质量"},
        {"JPEG", 30, "较低质量"},
        {"PNG", -1, "PNG格式"}  // -1 表示默认质量
    };

    QByteArray bestResult;
    QString bestStrategyDesc;

    for (const auto &strategy : strategies) {
        buffer.open(QIODevice::WriteOnly);
        byteArray.clear();

        bool saveSuccess = false;
        if (strategy.format == "JPEG") {
            saveSuccess = m_currentAvatar.save(&buffer, "JPEG", strategy.quality);
        } else {
            saveSuccess = m_currentAvatar.save(&buffer, "PNG");
        }

        buffer.close();

        if (saveSuccess) {
            qDebug() << QString("压缩策略: %1, 质量: %2, 大小: %3")
                        .arg(strategy.format)
                        .arg(strategy.quality)
                        .arg(formatFileSize(byteArray.size()));

            // 如果当前策略满足大小要求，直接返回
            if (byteArray.size() <= MAX_AVATAR_SIZE) {
                qDebug() << "找到满足条件的压缩方案:" << strategy.description;
                return byteArray;
            }

            // 记录最小的结果
            if (bestResult.isEmpty() || byteArray.size() < bestResult.size()) {
                bestResult = byteArray;
                bestStrategyDesc = strategy.description;
            }
        }
    }

    // 如果没有找到满足条件的策略，返回最小的结果
    if (!bestResult.isEmpty()) {
        qDebug() << "使用最小压缩结果:" << bestStrategyDesc
                 << "大小:" << formatFileSize(bestResult.size());
        return bestResult;
    }

    return QByteArray();
}

bool RegisterDialog::validateAvatarSize(const QByteArray &avatarData)
{
    if (avatarData.isEmpty()) {
        return true; // 没有头像也是有效的
    }

    if (avatarData.size() > MAX_AVATAR_SIZE) {
        return showSizeWarning(avatarData);
    }

    return true;
}


bool RegisterDialog::showSizeWarning(const QByteArray &avatarData)
{
    // 使用自定义消息框提供三个选项
    CustomMessageBox::showWarning(
        this,
        "头像文件过大",
         "请重新选择");

    // 清空当前头像，让用户重新选择
    m_currentAvatar = QPixmap();
    updateAvatarDisplay();
    on_headimg_btn_clicked(); // 重新打开选择对话框
    return false;
}

QString RegisterDialog::formatFileSize(qint64 bytes)
{
    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;

    if (bytes >= MB) {
        return QString("%1 MB").arg(QString::number(bytes * 1.0 / MB, 'f', 1));
    } else if (bytes >= KB) {
        return QString("%1 KB").arg(QString::number(bytes * 1.0 / KB, 'f', 1));
    } else {
        return QString("%1 字节").arg(bytes);
    }
}

void RegisterDialog::set_group_register()
{
    m_choice = 1;
    ui->label_5->hide();
    ui->password_edit->hide();
}

