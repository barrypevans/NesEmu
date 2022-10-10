#include "ScreenWidget.h"

ScreenWidget::ScreenWidget()
{
    m_pPixmapItem = new QGraphicsPixmapItem();
    m_pFrameBuffer = new QPixmap();
    m_pBackBuffer = new QPixmap();
    m_pImage = new QImage(256, 240, QImage::Format_RGB32);

    m_pPixmapItem->setPixmap(*m_pFrameBuffer);

    scene = new QGraphicsScene(this);
    scene->addItem(m_pPixmapItem);

    this->setScene(scene);
    m_rawTable1 = (uint32_t*)malloc(256 * 240 * 4);
    m_controllerValues = 0;
}

ScreenWidget::~ScreenWidget()
{

}

void ScreenWidget::Render(Ppu2C02* pPpu)
{
    
    if (pPpu->m_frameCompleted)
    {
        m_pBackBuffer->convertFromImage(*m_pImage);

        QPixmap* pTemp = m_pBackBuffer;
        m_pBackBuffer = m_pFrameBuffer;
        m_pFrameBuffer = pTemp;

       /* pPpu->GetNameTable(m_rawTable1, 0, 0);
        for (int i = 0; i < 256; i++)
        {
            for (int j = 0; j < 240; j++)
            {
                m_pImage->setPixel(i, j, m_rawTable1[i + j * 256]);
            }
        }*/
    }

    if (pPpu->m_scanline < 0 || pPpu->m_scanline >=240 || (pPpu->m_cycle - 2) > 255 || (pPpu->m_cycle - 2) < 0) return;

    m_pImage->setPixel(pPpu->m_cycle-2, pPpu->m_scanline, pPpu->m_bufferedPixel); 


  
}

void ScreenWidget::keyPressEvent(QKeyEvent* pEvent)
{
    if (pEvent->isAutoRepeat())
    {
        QGraphicsView::keyPressEvent(pEvent);
        return;
    }

    switch (pEvent->key()) {
        case Qt::Key::Key_A: m_controllerValues |= (0x01 << 1); break;
        case Qt::Key::Key_S:  m_controllerValues |= (0x01 << 2); break;
        case Qt::Key::Key_W:  m_controllerValues |= (0x01 << 3); break;
        case Qt::Key::Key_D:  m_controllerValues |= (0x01); break;
        case Qt::Key::Key_Z:  m_controllerValues |= (0x01 << 7); break; // A
        case Qt::Key::Key_X:  m_controllerValues |= (0x01 << 6); break; // B
        case Qt::Key::Key_Q:  m_controllerValues |= (0x01 << 5); break; // select
        case Qt::Key::Key_E:  m_controllerValues |= (0x01 << 4); break; // start
    }
    QGraphicsView::keyPressEvent(pEvent);
    qDebug() << "controller: " << QString("%1").arg(m_controllerValues, 8, 2, QChar('0'));
}

void ScreenWidget::keyReleaseEvent(QKeyEvent* pEvent)
{
    if (pEvent->isAutoRepeat()) 
    {
        QGraphicsView::keyPressEvent(pEvent);
        return;
    }

    switch (pEvent->key()) {
        case Qt::Key::Key_A: m_controllerValues &= ~(0x01 << 1); break;
        case Qt::Key::Key_S:  m_controllerValues &= ~(0x01 << 2); break;
        case Qt::Key::Key_W:  m_controllerValues &= ~(0x01 << 3); break;
        case Qt::Key::Key_D:  m_controllerValues &= ~(0x01); break;
        case Qt::Key::Key_Z:  m_controllerValues &= ~(0x01 << 7); break; // A
        case Qt::Key::Key_X:  m_controllerValues &= ~(0x01 << 6); break; // B
        case Qt::Key::Key_Q:  m_controllerValues &= ~(0x01 << 5); break; // select
        case Qt::Key::Key_E:  m_controllerValues &= ~(0x01 << 4); break; // start
    }
    QGraphicsView::keyPressEvent(pEvent);
   
    qDebug() << "controller: " << QString("%1").arg(m_controllerValues, 8, 2, QChar('0'));
}


void ScreenWidget::UIRender()
{
    m_pPixmapItem->setPixmap(*m_pFrameBuffer);
    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}


