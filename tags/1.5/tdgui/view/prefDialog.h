#ifndef PREFDIALOG_H
#define PREFDIALOG_H

#include "ui_preferences.h"
#include "tdParam.h"
#include <QDialog>

class prefDialog : public QDialog
{
  Q_OBJECT
public:
	prefDialog(tdParam *tdp, QWidget *parent=0);
	~prefDialog();
	void getInitValues();
private:
  Ui::Preferences pref_ui;
  tdParam *param;
public slots:
  void validChanges();
};

#endif /* PREFDIALOG_H */
