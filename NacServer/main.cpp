#include <QtGui/qapplication.h>
#include "dialog.h"
#include "server/KServer.h"
#include <pthread.h>

extern KServer newServer;

void *initialRsa(void *)
{
    newServer.generateRsaKey(); //Generate RSA key
    newServer.setRsaKeyN(newServer.getRsa().getN());
    newServer.setRsaKeyE(newServer.getRsa().getE());
    cout<<"N="<<newServer.getRsaKeyN()<<endl;
    cout<<"E="<<newServer.getRsaKeyE()<<endl;
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog *d=new Dialog;
    d->show();
    a.setStyle("plastique");
    pthread_t pt;
    pthread_create(&pt, NULL, initialRsa, NULL);
    a.exec();
    return 0;
}
