
#ifndef HWPLUGININTERFACE_H
#define HWPLUGININTERFACE_H

#include <QWidget>
#include <QWidget>
#include <QtPlugin>

class HWPluginInterface : public QWidget
{

Q_OBJECT

public:
	virtual QVariant * configuration() const = 0;
	virtual void setConfiguration(QVariant * conf=0) = 0;

	virtual void setStandAlone(bool standAlone=true) = 0;
	virtual bool standAlone() const = 0;

	virtual bool isBusy() = 0;

public slots:
	virtual void reprogramDevice() = 0;
	virtual void compilationComplete(bool success=true) = 0;

	virtual void setProgramFileName(QString filename) = 0;
	virtual void abort() = 0;

signals:
	void reprogramComplete();
	void requestCompilation();
	void requestProgramFileName();
	void openHexEditor(QString filename);
};

Q_DECLARE_INTERFACE(HWPluginInterface, "com.moravia-microsystems.mds.HWPluginInterface/0.1");

#endif // HWPLUGININTERFACE_H
