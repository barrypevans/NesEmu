#include "PatternTableWidget.h"


PatternTableWidget::PatternTableWidget()
{
    m_pPixmapItem = new QGraphicsPixmapItem();
    m_pFrameBuffer = new QPixmap();
    m_pBackBuffer = new QPixmap();
    m_pImage = new QImage(256, 128, QImage::Format_RGB32);

    m_pPixmapItem->setPixmap(*m_pFrameBuffer);

    scene = new QGraphicsScene(this);
    scene->addItem(m_pPixmapItem);
    

    this->setScene(scene);
    m_rawTable1 = (uint32_t*)malloc(128 * 128 * 4);
    m_rawTable2 = (uint32_t*)malloc(128 * 128 * 4);
}

PatternTableWidget::~PatternTableWidget()
{
}

void PatternTableWidget::Render(Ppu2C02* pPpu)
{
    if (pPpu->m_frameCompleted)
    {
        pPpu->GetPatternTable(m_rawTable1, 0, m_paletteIndex);
        pPpu->GetPatternTable(m_rawTable2, 1, m_paletteIndex);

        for (int i = 0; i < 128; i++)
        {
            for (int j = 0; j < 128; j++)
            {
                m_pImage->setPixel(i, j, m_rawTable1[i + j * 128]);
            }
        }

        for (int i = 0; i < 128; i++)
        {
            for (int j = 0; j < 128; j++)
            {
                m_pImage->setPixel(i + 128, j,  m_rawTable2[i + j * 128]);
            }
        }

        m_pBackBuffer->convertFromImage(*m_pImage);
        
        QPixmap* pTemp = m_pBackBuffer;
        m_pBackBuffer = m_pFrameBuffer;
        m_pFrameBuffer = pTemp;
    }
}

void PatternTableWidget::UIRender()
{
    m_pPixmapItem->setPixmap(*m_pFrameBuffer);
    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void PatternTableWidget::keyPressEvent(QKeyEvent* ev)
{
    if (ev->key() == Qt::Key_P)
    {
        m_paletteIndex++;
        m_paletteIndex = m_paletteIndex % 16;
    }
}