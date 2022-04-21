#include "ScreenWidget.h"

ScreenWidget::ScreenWidget()
{
    m_pPixmapItem = new QGraphicsPixmapItem();
    m_pFrameBuffer = new QPixmap();
    m_pBackBuffer = new QPixmap();
    m_pImage = new QImage(341, 261, QImage::Format_RGB32);

    m_pPixmapItem->setPixmap(*m_pFrameBuffer);

    scene = new QGraphicsScene(this);
    scene->addItem(m_pPixmapItem);

    this->setScene(scene);
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
    }

    if (pPpu->m_scanline < 0) return;

    m_pImage->setPixel(pPpu->m_cycle, pPpu->m_scanline, pPpu->m_bufferedPixel);
}

void ScreenWidget::UIRender()
{
    m_pPixmapItem->setPixmap(*m_pFrameBuffer);
    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}


