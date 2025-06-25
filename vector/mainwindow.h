#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRect>
#include <QPoint>
#include <QtWidgets>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_5_clicked(bool checked);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    Ui::MainWindow *ui;
    void on_pushButton_5_clicked();
    enum class ResizeEdge {
        None,
        TopLeft,
        Top,
        TopRight,
        Right,
        BottomRight,
        Bottom,
        BottomLeft,
        Left
    };
    bool m_drawing = false;  // 是否正在绘制矩形
    QRect m_rect;            // 存储绘制的矩形
    QPoint m_startPos;
    bool m_resizing = false; // 是否正在缩放矩形
    QPoint m_resizeStartPos; // 缩放开始时的鼠标位置
    QPoint m_rectAnchor;     // 矩形上与鼠标按下点对应的锚点，用于计算缩放
    double m_scaleFactor = 1.0;      // 缩放比例，初始为 1.0 表示不缩放
    QPoint m_scaleReferencePoint;    // 缩放参考点
    ResizeEdge m_resizeEdge = ResizeEdge::None;
};
#endif // MAINWINDOW_H
