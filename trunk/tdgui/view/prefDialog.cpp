#include "prefDialog.h"

prefDialog::prefDialog(tdParam *tdp, QWidget *parent) :QDialog(parent)
{
  param = tdp;
	pref_ui.setupUi(this);
	getInitValues();
	connect(this, SIGNAL(accepted()), this, SLOT(validChanges()));
}

prefDialog::~prefDialog() {
}

void prefDialog::getInitValues() {
  pref_ui.browser->setText(param->getBrowser().c_str());
  pref_ui.proxy_address->setText(param->getProxyAddress().c_str());
  pref_ui.proxy_port->setText(QString::number(param->getProxyPort()));
  if (param->getProxyType() == CURLPROXY_HTTP) {
    pref_ui.typeHTTP->setChecked(true);
    pref_ui.typeSOCKS5->setChecked(false);
  } else {
    pref_ui.typeHTTP->setChecked(false);
    pref_ui.typeSOCKS5->setChecked(true);
  }
  pref_ui.timeout->setText(QString::number(param->getTimeout()));
}

void prefDialog::validChanges() {
  param->setBrowser(pref_ui.browser->text().toStdString());
  param->setProxy(pref_ui.proxy_address->text().toStdString(),
                  pref_ui.proxy_port->text().toStdString(),
                  (pref_ui.typeSOCKS5->isChecked())?"SOCKS5":"HTTP");
  param->setTimeout(pref_ui.timeout->text().toStdString());
  param->commit();
}

