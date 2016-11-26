#ifndef MESSAGEBOX
#define MESSAGEBOX
#include <QMessageBox>

class MessageBox : public QMessageBox
{
    Q_OBJECT
public:
    MessageBox(const QString &title,
               const QString &message,
               QWidget *parent = 0)
        : QMessageBox(parent)
    {
        setWindowTitle(title);
        setText(message);
        exec();
    }
};


#endif // MESSAGEBOX

