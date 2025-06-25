#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    m_drawing = false;
    m_resizing = false;
    ui->setupUi(this);
    m_scaleFactor = 1.0;
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_5_clicked(bool checked)
{
    m_drawing = true;
    m_resizing = false;
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (m_drawing) {
        m_startPos = event->pos();
        m_rect.setTopLeft(m_startPos);
        m_rect.setBottomRight(m_startPos);
    } else {
        int x = event->x();
        int y = event->y();
        int rectLeft = m_rect.left();
        int rectTop = m_rect.top();
        int rectRight = m_rect.right();
        int rectBottom = m_rect.bottom();
        int borderSize = 10; // 边框宽度，可调整

        if (x >= rectLeft - borderSize && x <= rectLeft + borderSize && y >= rectTop - borderSize && y <= rectBottom + borderSize) {
            m_resizing = true;
            m_resizeStartPos = event->pos();
            if (x <= rectLeft + borderSize && y <= rectTop + borderSize) {
                m_resizeEdge = ResizeEdge::TopLeft;
            } else if (x >= rectRight - borderSize && y <= rectTop + borderSize) {
                m_resizeEdge = ResizeEdge::TopRight;
            } else if (x >= rectRight - borderSize && y >= rectBottom - borderSize) {
                m_resizeEdge = ResizeEdge::BottomRight;
            } else if (x <= rectLeft + borderSize && y >= rectBottom - borderSize) {
                m_resizeEdge = ResizeEdge::BottomLeft;
            } else if (y <= rectTop + borderSize) {
                m_resizeEdge = ResizeEdge::Top;
            } else if (x >= rectRight - borderSize) {
                m_resizeEdge = ResizeEdge::Right;
            } else if (y >= rectBottom - borderSize) {
                m_resizeEdge = ResizeEdge::Bottom;
            } else if (x <= rectLeft + borderSize) {
                m_resizeEdge = ResizeEdge::Left;
            }
        }
    }
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_drawing) {
        QPoint endPos = event->pos();
        m_rect.setBottomRight(endPos);
        update();  // 触发重绘
    }else if (m_resizing) {
        QPoint delta = event->pos() - m_resizeStartPos;
        int newLeft = m_rect.left();
        int newTop = m_rect.top();
        int newRight = m_rect.right();
        int newBottom = m_rect.bottom();

        switch (m_resizeEdge) {
        case ResizeEdge::TopLeft:
            newLeft += delta.x();
            newTop += delta.y();
            break;
        case ResizeEdge::Top:
            newTop += delta.y();
            break;
        case ResizeEdge::TopRight:
            newRight += delta.x();
            newTop += delta.y();
            break;
        case ResizeEdge::Right:
            newRight += delta.x();
            break;
        case ResizeEdge::BottomRight:
            newRight += delta.x();
            newBottom += delta.y();
            break;
        case ResizeEdge::Bottom:
            newBottom += delta.y();
            break;
        case ResizeEdge::BottomLeft:
            newLeft += delta.x();
            newBottom += delta.y();
            break;
        case ResizeEdge::Left:
            newLeft += delta.x();
            break;
        default:
            break;
        }
        m_rect.setLeft(qMax(0, newLeft));
        m_rect.setTop(qMax(0, newTop));
        m_rect.setRight(qMax(newLeft, newRight));
        m_rect.setBottom(qMax(newTop, newBottom));
        QPoint newTopLeft = m_rect.topLeft() + delta;
        QPoint newBottomRight = m_rect.bottomRight() + delta;
        QPoint newReferencePoint = m_scaleReferencePoint + delta;
        // 计算相对于参考点的缩放比例
        double dx = (double)(newReferencePoint.x() - m_rect.left()) / (double)(m_scaleReferencePoint.x() - m_rect.left());
        double dy = (double)(newReferencePoint.y() - m_rect.top()) / (double)(m_scaleReferencePoint.y() - m_rect.top());
        m_scaleFactor = (dx + dy) / 2.0;

        // 计算缩放后的矩形
        QPoint topLeft = m_rect.topLeft() + (newReferencePoint - m_scaleReferencePoint);
        QSize size = m_rect.size() * m_scaleFactor;
        m_rect.setTopLeft(topLeft);
        m_rect.setSize(size);
        // 确保矩形大小不为负
        if (newTopLeft.x() < newBottomRight.x() && newTopLeft.y() < newBottomRight.y()) {
            m_rect.setTopLeft(newTopLeft);
            m_rect.setBottomRight(newBottomRight);
            update();
        }
        m_resizeStartPos = event->pos();
        m_scaleReferencePoint = newReferencePoint;
    }
    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_drawing) {
        m_drawing = false;
    }else if (m_resizing) {
        m_resizing = false;
        m_resizeEdge = ResizeEdge::None;
    }
    QMainWindow::mouseReleaseEvent(event);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    if (m_drawing || m_resizing) {
        QPainter painter(this);
        painter.drawRect(m_rect);
    }
}
